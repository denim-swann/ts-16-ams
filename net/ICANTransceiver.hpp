// Team Swinburne 2016

#ifndef __AMS_INTERFACE_CAN_TRANSCEIVER_HPP__
#define __AMS_INTERFACE_CAN_TRANSCEIVER_HPP__

#include "mbed.h"

template <class T>
struct ICANTransceiver
{
    static uint8_t rderrors()
    {
        return T::rderrors();
    }

    static uint8_t tderrors()
    {
        return T::tderrors();
    }

    static int write(CANMessage msg)
    {
        return T::write(msg);
    }
};

#endif // __AMS_INTERFACE_CAN_TRANSCEIVER_HPP__
