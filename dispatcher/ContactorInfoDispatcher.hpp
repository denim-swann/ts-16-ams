// Team Swinburne 2016

#ifndef __AMS_CONTACTOR_INFO_DISPATCHER_HPP__
#define __AMS_CONTACTOR_INFO_DISPATCHER_HPP__

#include "../core/CyclicTask.hpp"

struct ContactorInfoDispatcher : public CyclicTask<ContactorInfoDispatcher>
{
    static osPriority thread_priority;
    static uint32_t thread_period;

    static void initialise();
    static void procedure();
};

#endif // __AMS_CONTACTOR_INFO_DISPATCHER_HPP__
