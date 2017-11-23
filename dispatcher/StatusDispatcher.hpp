// Team Swinburne 2016

#ifndef __AMS_STATUS_DISPATCHER_HPP__
#define __AMS_STATUS_DISPATCHER_HPP__

#include "../core/CyclicTask.hpp"

#define STATUS_ERROR_INSULATION_FAULT           (1 << 0)
#define STATUS_ERROR_AMS                        (1 << 1)
#define STATUS_ERROR_STACK_UNDERVOLTAGE         (1 << 2)
#define STATUS_ERROR_STACK_OVERVOLTAGE          (1 << 3)
#define STATUS_ERROR_SERVICE_DISCONNECTS        (1 << 4)
#define STATUS_ERROR_HARDWARE_OVERCURRENT       (1 << 5)
#define STATUS_INFO_5KW_FLAG                    (1 << 6)
#define STATUS_BMS_CELL_OVERVOLTAGE             (1 << 7)
#define STATUS_BMS_CELL_UNDERVOLTAGE            (1 << 8)
#define STATUS_BMS_CELL_OVERTEMP                (1 << 9)
#define STATUS_BMS_CELL_UNDERTEMP               (1 << 10)
#define STATUS_BMS_CELL_OPEN_CIRCUIT            (1 << 11)
#define STATUS_INFO_PRECHARGE_OUTOFRANGE		(1 << 12)
#define STATUS_INFO_PRECHARGE_99				(1 << 13)
#define STATUS_INFO_AIR_POWER_AVAILABLE			(1 << 14)

struct StatusDispatcher : public CyclicTask<StatusDispatcher>
{
    static osPriority thread_priority;
    static uint32_t thread_period;

    static void initialise();
    static uint16_t generate_status_mask();
    static void procedure();
};

#endif // __AMS_HEARTBEAT_DISPATCHER_HPP__
