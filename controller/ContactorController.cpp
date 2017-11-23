// Team Swinburne 2016

#include "ContactorController.hpp"

#include "../Pinout.hpp"
#include "StateController.hpp"

osPriority ContactorController::thread_priority = osPriorityNormal;
uint32_t ContactorController::thread_period = 500; // ms

contactor_state_t ContactorController::current_state = hvcInitialising;
Mutex ContactorController::current_state_lock;

DigitalOut ContactorController::precharge_contactor_coil(OUTPUT_PRECHARGE_ENABLE_PIN, false);
DigitalIn ContactorController::precharge_contactor_feedback(PRECHARGE_AUX_FEEDBACK_PIN);
DigitalOut ContactorController::run_contactor_coil(OUTPUT_RUN_ENABLE_PIN, false);
DigitalIn ContactorController::run_contactor_feedback(RUN_AUX_FEEDBACK_PIN);

RtosTimer ContactorController::precharge_error_timer(Callback<void()>(ContactorController::precharge_toggle_cb), osTimerOnce);
bool ContactorController::precharge_fireonce = false;
bool ContactorController::precharge_hardware_error = false;
RtosTimer ContactorController::run_error_timer(Callback<void()>(ContactorController::run_toggle_cb), osTimerOnce);
bool ContactorController::run_fireonce = false;
bool ContactorController::run_hardware_error = false;

void ContactorController::initialise()
{
    current_state_lock.lock();
    current_state = hvcAllOpen;
    current_state_lock.unlock();

    precharge_fireonce = false;
    precharge_hardware_error = false;
    run_fireonce = false;
    run_hardware_error = false;
}

bool ContactorController::pc_coil_status()
{
    return precharge_contactor_coil;
}

bool ContactorController::pc_feedback_status()
{
    return precharge_contactor_feedback;
}

bool ContactorController::pc_hwerr_status()
{
    return precharge_hardware_error;
}

bool ContactorController::run_coil_status()
{
    return run_contactor_coil;
}

bool ContactorController::run_feedback_status()
{
    return run_contactor_feedback;
}

bool ContactorController::run_hwerr_status()
{
    return run_hardware_error;
}

// Callback: XXX time after precharge coil changes state
void ContactorController::precharge_toggle_cb()
{
    precharge_fireonce = true;
    procedure(); // Trigger check immediately
}

// Callback: XXX time after run coil changes state
void ContactorController::run_toggle_cb()
{
    run_fireonce = true;
    procedure(); // Trigger check immediately
}

void ContactorController::push_state(contactor_state_t new_state)
{
    bool raise_sequence_error = true;
    current_state_lock.lock();

    switch (new_state) {
        case hvcLatchingMechanicalError:
            current_state = hvcLatchingMechanicalError;
            raise_sequence_error = false;
            break;

        case hvcAllOpen:
            // All n/e states can immediately tansition to open, otherwise error
            if (current_state >= hvcAllOpen) {
                current_state = hvcAllOpen;
                raise_sequence_error = false;
            }
            break;

        case hvcPrecharge:
            // The open state can transition to precharging, however transitioning
            // back to precharge while running could damage the system.
            if (current_state == hvcAllOpen) {
                current_state = hvcPrecharge;
                raise_sequence_error = false;
            }
            break;

        case hvcHighCurrentRun:
            // Only the precharging state can transition to run
            if (current_state == hvcPrecharge) {
                current_state = hvcHighCurrentRun;
                raise_sequence_error = false;
            }
            break;

        default:
            // Hold sequence error flag
            break;
    }

    if (raise_sequence_error) {
        StateController::notify_event(sceContactorSequenceError);

        // If the system is initialising or there is a latching mechanical error
        // we don't want to clear it at this point accidentally, so we must check.
        if (current_state > hvcAllOpen) {
            current_state = hvcAllOpen;
        }
    }

    // Old states
    bool precharge_coil_prime = precharge_contactor_coil;
    bool run_coil_prime = run_contactor_coil;

    // Sync new states
    precharge_contactor_coil = current_state & HVC_MASK_PRECHARGE;
    run_contactor_coil = current_state & HVC_MASK_RUN;

    // Save time if toggle occured in prime
    if (precharge_coil_prime != precharge_contactor_coil) {
        uint32_t millisec = precharge_contactor_coil ? PRECHARGE_TIME_PULLIN_MS : PRECHARGE_TIME_PULLOUT_MS;
        precharge_fireonce = false;
        precharge_error_timer.start(millisec);
    }
    if (run_coil_prime != run_contactor_coil) {
        uint32_t millisec = run_contactor_coil ? RUN_TIME_PULLIN_MS : RUN_TIME_PULLOUT_MS;
        run_fireonce = false;
        run_error_timer.start(millisec);
    }

    current_state_lock.unlock();
}

void ContactorController::procedure()
{
    /*
    // Save the errors for a CAN bit
    precharge_hardware_error = precharge_fireonce
        && (precharge_contactor_coil != pc_feedback_status());
    run_hardware_error = run_fireonce
        && (run_contactor_coil != run_feedback_status());

    // Notify the state controller whom may initiate a shutdown
    if (precharge_hardware_error) {
        push_state(hvcLatchingMechanicalError);
        StateController::notify_event(sceContactorHardwareFaultError);
    }
    if (run_hardware_error) {
        push_state(hvcLatchingMechanicalError);
        StateController::notify_event(sceContactorHardwareFaultError);
    } */
}
