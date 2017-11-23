// Team Swinburne 2016

#ifndef __AMS_HEARTBEAT_DISPATCHER_HPP__
#define __AMS_HEARTBEAT_DISPATCHER_HPP__

#include "../core/CyclicTask.hpp"

struct HeartbeatDispatcher : public CyclicTask<HeartbeatDispatcher>
{
    static osPriority thread_priority;
    static uint32_t thread_period;

    static uint16_t heartbeat_counter;

    static void initialise();
    static void procedure();
};

#endif // __AMS_HEARTBEAT_DISPATCHER_HPP__
