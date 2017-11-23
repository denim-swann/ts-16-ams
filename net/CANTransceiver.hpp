// Team Swinburne 2016

#ifndef __AMS_CAN_TRANSCEIVER_HPP__
#define __AMS_CAN_TRANSCEIVER_HPP__

#include "../Pinout.hpp"
#include "../core/SporadicTask.hpp"
#include "../net/ICANTransceiver.hpp"

#define CAN_TRANSCIEVER_BUFFER_SIZE         (32)
#define CAN_TRANSCIEVER_FREQUENCY           (500000)

struct CANTransceiver : public SporadicTask<CANTransceiver>, public ICANTransceiver<CANTransceiver>
{
    static osPriority thread_priority;

    static CAN driver;
    static Mutex driver_lock;

    static Mail<CANMessage, CAN_TRANSCIEVER_BUFFER_SIZE> message_box;
    static CANMessage *next_message;

    static void initialise();
    static void rx_interrupt();
    static uint8_t rderrors();
    static uint8_t tderrors();
    static int write(CANMessage msg);
    static void procedure();
    static void dispatch_message(CANMessage &msg);
};

#endif // __AMS_CAN_TRANSCEIVER_HPP__
