// Team Swinburne 2016

#ifndef __AMS_STACK_PLAUSIBILITY_HPP__
#define __AMS_STACK_PLAUSIBILITY_HPP__

#include "../Pinout.hpp"
#include "../core/SporadicTask.hpp"

#define SIGNAL_BATTERY_UNDERVOLTAGE             (1 << 0)
#define SIGNAL_BATTERY_OVERVOLTAGE              (1 << 1)
#define SIGNAL_SERVICE_DISCONNECTS_REMOVED      (1 << 2)
#define SIGNAL_PCOOR_UNASSERT                   (1 << 3)
#define SIGNAL_PC99_UNASSERT                    (1 << 4)
#define SIGNAL_CONTACTOR_POWER_LOST             (1 << 5)

struct StackPlausibility : public SporadicTask<StackPlausibility>
{
    friend BaseTask<StackPlausibility>;
    
protected:
    static osPriority thread_priority;
    static InterruptIn battery_undervoltage;
    static InterruptIn battery_overvoltage;
    static InterruptIn service_disconnects;
    static InterruptIn precharge_oor;
    static InterruptIn precharge_99;
    static InterruptIn contactor_power;

    static void buv_interrupt();
    static void bov_interrupt();
    static void disconnect_interrupt();
    static void pc_oor_interrupt();
    static void pc_99_interrupt();
    static void contactor_power_lost_interrupt();

public:
    static bool undervoltage_status();
    static bool overvoltage_status();
    static bool disconnects_status();
    static bool precharge_oor_status();
    static bool precharge_99_status();
    static bool contactor_power_status();
    static bool is_plausible();

    static void initialise();
    static void procedure();
};

#endif // __AMS_STACK_PLAUSIBILITY_HPP__
