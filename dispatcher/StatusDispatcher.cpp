// Team Swinburne 2016

#include "StatusDispatcher.hpp"

#include "../service/StatusMessageService.hpp"
#include "../net/CANTransceiver.hpp"
#include "../controller/InsulationMonitoringDevice.hpp"
#include "../controller/StackPlausibility.hpp"
#include "../controller/StateController.hpp"
#include "../controller/BMSController.hpp"

osPriority StatusDispatcher::thread_priority = osPriorityNormal;
uint32_t StatusDispatcher::thread_period = 75; // ms

void StatusDispatcher::initialise()
{
    // Empty
}

uint16_t StatusDispatcher::generate_status_mask()
{
    uint16_t status_mask = 0;

    if (!InsulationMonitoringDevice::imd_status()) {
        status_mask |= STATUS_ERROR_INSULATION_FAULT;
    }
    if (!StackPlausibility::undervoltage_status()) {
        status_mask |= STATUS_ERROR_STACK_UNDERVOLTAGE;
    }
    if (!StackPlausibility::overvoltage_status()) {
        status_mask |= STATUS_ERROR_STACK_OVERVOLTAGE;
    }
    if (!StackPlausibility::disconnects_status()) {
        status_mask |= STATUS_ERROR_SERVICE_DISCONNECTS;
    }
    if (StackPlausibility::precharge_oor_status()) {
        status_mask |= STATUS_INFO_PRECHARGE_OUTOFRANGE;
    }
    if (StackPlausibility::precharge_99_status()) {
        status_mask |= STATUS_INFO_PRECHARGE_99;
    }
    if (StackPlausibility::contactor_power_status()) {
        status_mask |= STATUS_INFO_AIR_POWER_AVAILABLE;
    }
    
    // TODO HWOC
    // TODO 5kW
    
    if (!BMSController::bms_status()) {
        status_mask |=
            ( STATUS_BMS_CELL_OVERVOLTAGE
            | STATUS_BMS_CELL_UNDERVOLTAGE
            | STATUS_BMS_CELL_UNDERTEMP
            | STATUS_BMS_CELL_OVERTEMP
            | STATUS_BMS_CELL_OPEN_CIRCUIT);
    }

    if ((status_mask &
        ( STATUS_ERROR_STACK_UNDERVOLTAGE
        | STATUS_ERROR_STACK_OVERVOLTAGE
        | STATUS_ERROR_SERVICE_DISCONNECTS
        | STATUS_ERROR_HARDWARE_OVERCURRENT
        | STATUS_BMS_CELL_OVERVOLTAGE
        | STATUS_BMS_CELL_UNDERVOLTAGE
        | STATUS_BMS_CELL_UNDERTEMP
        | STATUS_BMS_CELL_OVERTEMP
        | STATUS_BMS_CELL_OPEN_CIRCUIT)) != 0) {
        // AMS error light is a union of many error kinds
        status_mask |= STATUS_ERROR_AMS;
    }

    return status_mask;
}

void StatusDispatcher::procedure()
{
    uint16_t status_mask = generate_status_mask();
    uint16_t current_state = StateController::get_state();
    StatusMessageService<CANTransceiver>::send(status_mask, current_state);
}
