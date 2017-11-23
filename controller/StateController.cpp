// Team Swinburne 2016

#include "StateController.hpp"

#include "ContactorController.hpp"
#include "BMSController.hpp"
#include "StackPlausibility.hpp"
#include "InsulationMonitoringDevice.hpp"

osPriority StateController::thread_priority = osPriorityRealtime;

volatile state_controller_state_t StateController::current_state = scsInitialising;
Mutex StateController::current_state_lock;

bool StateController::tractive_power_enable_bit = false;
Mutex StateController::tractive_power_enable_lock;

RtosTimer StateController::precharge_timer(Callback<void()>(StateController::precharge_timer_fire), osTimerOnce);

void StateController::initialise()
{
    current_state_lock.lock();
    transition_state(scsWaitingForInternalSafety);
    current_state_lock.unlock();
}

void StateController::precharge_timer_fire()
{
    current_state_lock.lock();
    if (current_state == scsStartedPrecharge) {
        transition_state(scsPrechargeMinimumTimeExceeded);
    }
    current_state_lock.unlock();
}

void StateController::push_tractive_enable(bool new_val)
{
    bool transition = false;
    
    tractive_power_enable_lock.lock();
    if (new_val != tractive_power_enable_bit) { // fall
        transition = true;
    }
    tractive_power_enable_bit = new_val;
    tractive_power_enable_lock.unlock();
    
    // Fire transitioning event
    if (transition) {
        notify_event(new_val ? sceDriverEnablesTractiveSystem : sceDriverDeactivatesTractiveSystem);
    }
}

void StateController::notify_event(state_controller_event_t ev)
{
    current_state_lock.lock();

    switch (ev) {
        case sceCANBusIntegrityError:
        case sceStackPlausibilityError:
        case sceHardwareOvercurrentError:
        case sceInsulationMonitoringDeviceError:
        case sceBatteryManagementSystemError:
        case sceContactorHardwareFaultError:
        case sceContactorSequenceError:
            if (current_state > scsWaitingForInternalSafety) {
                // UPDATE current_state
                transition_state(scsWaitingForInternalSafety);
            }
            m_thread.signal_set(SIGNAL_STATE_CONTROLLER_INTERRUPT);
            break;

        case sceDriverDeactivatesTractiveSystem:
            // Deactivating the TS removes a re-settable latching error and restarts the checking process
            if (current_state == scsLatchingError || current_state >= scsWaitingForInternalSafety) {
                // UPDATE current_state
                transition_state(scsWaitingForInternalSafety);
            }
            m_thread.signal_set(SIGNAL_STATE_CONTROLLER_INTERRUPT);
            break;
            
        case sceDriverEnablesTractiveSystem:
            // Do nothing, just trigger update
            m_thread.signal_set(SIGNAL_STATE_CONTROLLER_INTERRUPT);
            break;
    }

    current_state_lock.unlock();
}

// PRE CONDITION: holding lock for current_state
void StateController::transition_state(state_controller_state_t new_state)
{
    switch (new_state) {
        case scsInitialising:
        case scsNonResettableLatchingError:
        case scsLatchingError:
        case scsWaitingForInternalSafety:
        case scsWaitingForTractiveLoop:
        case scsWaitingForShutdownCircuit:
        case scsWaitingForPrechargeActivation:
            ContactorController::push_state(hvcAllOpen);
            precharge_timer.stop();
            break;

        case scsStartedPrecharge:
            ContactorController::push_state(hvcPrecharge);
            precharge_timer.start(PRECHARGE_MINIMUM_TIME_MILLISEC);
            break;

        case scsPrechargeMinimumTimeExceeded:
            // Do nothing
            break;

        case scsTractiveSystemRaceReady:
            ContactorController::push_state(hvcHighCurrentRun);
            break;
    }
    
    current_state = new_state;
}

// CAN signal is stored in this class
// On the edge of CAN signal changing to 0 from 1, fire the event
// On the edge of CAN signal changing from 1 to 0, fire a different event
//     (but does nothing, just invokes state machine refresh)

// INFO: holds lock for state from state_machine_process_step
bool StateController::process_error_states()
{
    // Do nothing
    return true;
}

// INFO: holds lock for state from state_machine_process_step
bool StateController::process_waiting_states()
{
    if (!BMSController::bms_status() || !StackPlausibility::is_plausible()) {
        transition_state(scsWaitingForInternalSafety);
        return false;
    }
    if (!InsulationMonitoringDevice::imd_status() || !StackPlausibility::disconnects_status()) {
        transition_state(scsWaitingForTractiveLoop);
        return false;
    }
    if (!StackPlausibility::contactor_power_status()) {
        transition_state(scsWaitingForShutdownCircuit);
        return false;
    }
    
    return true;
}

// INFO: holds lock for state from state_machine_process_step
bool StateController::process_activated_states()
{
    if (!BMSController::bms_status() || !StackPlausibility::is_plausible()) {
        transition_state(scsLatchingError);
        return false;
    }
    if (!InsulationMonitoringDevice::imd_status() || !StackPlausibility::disconnects_status()) {
        transition_state(scsLatchingError);
        return false;
    }
    if (!StackPlausibility::contactor_power_status()) {
        transition_state(scsLatchingError);
        return false;
    }
    
    // TODO hwoc
    // TODO pc99
    // TODO pcoor
    return true;
}

void StateController::state_machine_process_step()
{
    current_state_lock.lock();
    bool ok;

    // error states
    switch (current_state) {
        case scsInitialising:
            // Do nothing
            break;

        case scsNonResettableLatchingError:
        case scsLatchingError:
            process_error_states();
            break;

        case scsWaitingForInternalSafety:
        case scsWaitingForTractiveLoop:
        case scsWaitingForShutdownCircuit:
            ok = process_waiting_states();
            if (tractive_power_enable_bit) { // TODO: this should use a getter!
                transition_state(scsLatchingError);
            } else if (ok) {
                transition_state(scsWaitingForPrechargeActivation);
            }
            break;

        case scsWaitingForPrechargeActivation:
            ok = process_waiting_states();
            if (ok && tractive_power_enable_bit) { // TODO: this should use a getter!
                transition_state(scsStartedPrecharge);
            }
            break;

        case scsStartedPrecharge:
            process_activated_states();
            // Time proceed happens in timer callback
            break;

        case scsPrechargeMinimumTimeExceeded:
            process_activated_states();
            // check digital inputs and proceed
            if (StackPlausibility::precharge_99_status()
                && StackPlausibility::precharge_oor_status()) {
                transition_state(scsTractiveSystemRaceReady);
            } else {
                transition_state(scsLatchingError); // TODO: just wait longer?
            }
            break;

        case scsTractiveSystemRaceReady:
            process_activated_states();
            break;
    }

    current_state_lock.unlock();
}

void StateController::procedure()
{
    for (;;) {
        m_thread.signal_wait(SIGNAL_STATE_CONTROLLER_INTERRUPT, STATE_MACHINE_FREQ_MILLISEC);
        state_machine_process_step();
    }
}
