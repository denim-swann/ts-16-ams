// Team Swinburne 2016

#include "HeartbeatDispatcher.hpp"

#include "../service/HeartbeatMessageService.hpp"
#include "../net/CANTransceiver.hpp"

osPriority HeartbeatDispatcher::thread_priority = osPriorityNormal;
uint32_t HeartbeatDispatcher::thread_period = 1000; // ms

uint16_t HeartbeatDispatcher::heartbeat_counter = 0;

static DigitalOut led1(LED1);
static DigitalOut led2(LED2);

void HeartbeatDispatcher::initialise()
{
    heartbeat_counter = 0;
}

void HeartbeatDispatcher::procedure()
{
	led1 = !led1;
	led2 = false;
    HeartbeatMessageService<CANTransceiver>::send(heartbeat_counter);
    heartbeat_counter++;
}
