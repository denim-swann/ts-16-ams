// Team Swinburne 2016

#ifndef __AMS_HEARTBEAT_MESSAGE_SERVICE_HPP__
#define __AMS_HEARTBEAT_MESSAGE_SERVICE_HPP__

#include "../net/ICANTransceiver.hpp"
#include "../net/CANIdentifiers.hpp"

#include <stdint.h>

#pragma pack(push, 1)
union mm_can_heartbeat_t
{
    struct {
        uint16_t counter;
        uint16_t rxerrors;
        uint16_t txerrors;
    };
    char as_bytes[];
};
typedef int __static_assert_heartbeat_size[sizeof(mm_can_heartbeat_t) == 6 ? 1 : -1];
#pragma pack(pop)

template <typename Transceiver>
struct HeartbeatMessageService
{
    static void send(uint32_t counter)
    {
        mm_can_heartbeat_t hb;
        hb.counter = counter;
        hb.rxerrors = Transceiver::rderrors();
        hb.txerrors = Transceiver::tderrors();
        Transceiver::write(CANMessage(CANID_AMS_HEARTBEAT, hb.as_bytes, sizeof(hb)));
    }
};

#endif // __AMS_HEARTBEAT_MESSAGE_SERVICE_HPP__
