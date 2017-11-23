// Team Swinburne 2016

#ifndef __AMS_INSULATION_MONITORING_DEVICE_HPP__
#define __AMS_INSULATION_MONITORING_DEVICE_HPP__

#include "../Pinout.hpp"
#include "../core/SporadicTask.hpp"

#define SIGNAL_INSULATION_FAULT                 (1 << 0)

struct InsulationMonitoringDevice : public SporadicTask<InsulationMonitoringDevice>
{
protected:
    static osPriority thread_priority;
    static DigitalIn _imd_signal;
    static InterruptIn imd_signal;
    static InterruptIn battery_overvoltage;
    static InterruptIn service_disconnects;

    static void imd_interrupt();

public:
    static bool imd_status();

    static void initialise();
    static void procedure();
};

#endif // __AMS_INSULATION_MONITORING_DEVICE_HPP__
