// Team Swinburne 2016

#ifndef __AMS_CONTACTOR_INFO_MESSAGE_SERVICE_HPP__
#define __AMS_CONTACTOR_INFO_MESSAGE_SERVICE_HPP__

#include "../net/ICANTransceiver.hpp"
#include "../net/CANIdentifiers.hpp"

#include <stdint.h>

#pragma pack(push, 1)
union mm_can_contactor_info_t
{
    struct {
        unsigned neg_driven : 1;
        unsigned neg_feedback : 1;
        unsigned neg_hwerr : 1;
        unsigned pos_driven : 1;
        unsigned pos_feedback : 1;
        unsigned pos_hwerr : 1;
    };
    char as_bytes[];
};
typedef int __static_assert_contactor_info_size[sizeof(mm_can_contactor_info_t) == 1 ? 1 : -1];
#pragma pack(pop)

template <typename Transceiver>
struct ContactorInfoMessageService
{
    static void send(bool neg_driven, bool neg_feedback, bool neg_hwerr, bool pos_driven, bool pos_feedback, bool pos_hwerr)
    {
        mm_can_contactor_info_t cinfo;
        cinfo.neg_driven = neg_driven;
        cinfo.neg_feedback = neg_feedback;
        cinfo.neg_hwerr = neg_hwerr;
        cinfo.pos_driven = pos_driven;
        cinfo.pos_feedback = pos_feedback;
        cinfo.pos_hwerr = pos_hwerr;
        Transceiver::write(CANMessage(CANID_AMS_CONTACTOR_INFO, cinfo.as_bytes, sizeof(cinfo)));
    }
};

#endif // __AMS_CONTACTOR_INFO_MESSAGE_SERVICE_HPP__
