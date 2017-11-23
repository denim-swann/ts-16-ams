// Team Swinburne 2016

#ifndef __AMS_BATTERY_CONTROLLER_HPP__
#define __AMS_BATTERY_CONTROLLER_HPP__

#include "../core/CyclicTask.hpp"

struct BMSController : public CyclicTask<BMSController>
{
    static osPriority thread_priority;
    static uint32_t thread_period;

protected:
    static DigitalIn bmsOk;
    static DigitalOut performConversions;
    static DigitalOut amsOk;
    
public:
    static bool bms_status();
    static void set_conversions_enable(bool enable);

    static void initialise();
    static void procedure();
};

#endif // __AMS_BATTERY_CONTROLLER_HPP__
