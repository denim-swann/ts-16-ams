// Team Swinburne 2016

#ifndef __AMS_STATE_CONTROLLER_HPP__
#define __AMS_STATE_CONTROLLER_HPP__

#include "../core/SporadicTask.hpp"

#define SIGNAL_STATE_CONTROLLER_INTERRUPT       (1 << 1)

#define STATE_MACHINE_FREQ_MILLISEC             (1000 / 10) // 10Hz, in millisec
#define PRECHARGE_MINIMUM_TIME_MILLISEC         (1000 * 5)  // 3 sec

typedef enum {
    scsNonResettableLatchingError = 0, // hard error, hardware error etc
    scsLatchingError = 1, // soft error, out of sequence CAN messages, etc, enable cycle resets
    scsInitialising = 2,
    scsWaitingForInternalSafety = 3,
    scsWaitingForTractiveLoop = 4,
    scsWaitingForShutdownCircuit = 5,
    scsWaitingForPrechargeActivation = 6,
    scsStartedPrecharge = 7,
    scsPrechargeMinimumTimeExceeded = 8,
    scsTractiveSystemRaceReady = 9
} state_controller_state_t;

typedef enum {
    sceCANBusIntegrityError = 1,
    sceStackPlausibilityError = 2,
    sceHardwareOvercurrentError = 3,
    sceInsulationMonitoringDeviceError = 4,
    sceBatteryManagementSystemError = 5,
    sceContactorHardwareFaultError = 6,
    sceContactorSequenceError = 7,
    sceDriverEnablesTractiveSystem = 10,
    sceDriverDeactivatesTractiveSystem = 11
} state_controller_event_t;

struct StateController : public SporadicTask<StateController>
{
    friend class BaseTask<StateController>;
    
protected:
    static osPriority thread_priority;
    
    static RtosTimer precharge_timer;
    static void precharge_timer_fire();

    static bool process_error_states();
    static bool process_waiting_states();
    static bool process_activated_states();
    static void state_machine_process_step();

public:
    static volatile state_controller_state_t current_state;
    static Mutex current_state_lock;
    
    static bool tractive_power_enable_bit;
    static Mutex tractive_power_enable_lock;

    static state_controller_state_t get_state() { return current_state; }
    static bool get_tractive_power_enable() { return tractive_power_enable_bit; }
    static void push_tractive_enable(bool new_val);

    static void initialise();
    static void notify_event(state_controller_event_t ev);
    static void transition_state(state_controller_state_t state);
    static void procedure();
};

#endif // __AMS_STATE_CONTROLLER_HPP__
