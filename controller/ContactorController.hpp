// Team Swinburne 2016

#ifndef __AMS_CONTACTOR_CONTROLLER_HPP__
#define __AMS_CONTACTOR_CONTROLLER_HPP__

#include "../core/CyclicTask.hpp"

#define PRECHARGE_TIME_PULLIN_MS                (300)
#define PRECHARGE_TIME_PULLOUT_MS               (300)
#define RUN_TIME_PULLIN_MS                      (PRECHARGE_TIME_PULLIN_MS)
#define RUN_TIME_PULLOUT_MS                     (PRECHARGE_TIME_PULLOUT_MS)

#define HVC_MASK_PRECHARGE                      (1 << 2)
#define HVC_MASK_RUN                            (1 << 3)

typedef enum {
    hvcInitialising = 0,
    hvcLatchingMechanicalError = 1,
    hvcAllOpen = 2,
    hvcPrecharge = HVC_MASK_PRECHARGE,
    hvcHighCurrentRun = HVC_MASK_PRECHARGE | HVC_MASK_RUN
} contactor_state_t;

// Monitors contactors and pushes a hardware error if the feedback don't match
// after XXX period of time.
// Throws error if the program does not close the contactors in the order expected.

struct ContactorController : public CyclicTask<ContactorController>
{
    friend class BaseTask<ContactorController>;
    friend class CyclicTask<ContactorController>;

protected:
    static osPriority thread_priority;
    static uint32_t thread_period;
    static DigitalOut precharge_contactor_coil; // AIR-
    static DigitalIn precharge_contactor_feedback;
    static DigitalOut run_contactor_coil; // AIR+
    static DigitalIn run_contactor_feedback;

    static RtosTimer precharge_error_timer;
    static bool precharge_fireonce;
    static bool precharge_hardware_error;
    static RtosTimer run_error_timer;
    static bool run_fireonce;
    static bool run_hardware_error;
    static void precharge_toggle_cb();
    static void run_toggle_cb();

    static contactor_state_t current_state;
    static Mutex current_state_lock;

public:
    static void push_state(contactor_state_t new_state);

    static bool pc_coil_status();
    static bool pc_feedback_status();
    static bool pc_hwerr_status();

    static bool run_coil_status();
    static bool run_feedback_status();
    static bool run_hwerr_status();

    static void initialise();
    static void procedure();
};

#endif // __AMS_CONTACTOR_CONTROLLER_HPP__
