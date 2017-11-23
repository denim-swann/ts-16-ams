// Team Swinburne 2016

#ifndef __AMS_STATUS_MESSAGE_SERVICE_HPP__
#define __AMS_STATUS_MESSAGE_SERVICE_HPP__

#include "../net/ICANTransceiver.hpp"
#include "../net/CANIdentifiers.hpp"

#include <stdint.h>

#pragma pack(push, 1)
union mm_can_status_t
{
    struct {
        uint16_t status_mask;
        uint8_t reserved;
        uint8_t current_state;
    };
    char as_bytes[];
};
typedef int __static_assert_status_size[sizeof(mm_can_status_t) == 4 ? 1 : -1];
#pragma pack(pop)

template <typename Transceiver>
struct StatusMessageService
{
    static void send(uint16_t status_mask, uint8_t current_state)
    {
        mm_can_status_t smsg;
        smsg.status_mask = status_mask;
        smsg.current_state = current_state;
        Transceiver::write(CANMessage(CANID_AMS_STATUS, smsg.as_bytes, sizeof(smsg)));
    }
};

#endif // __AMS_STATUS_MESSAGE_SERVICE_HPP__
