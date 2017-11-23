// Team Swinburne 2016

#include "ContactorInfoDispatcher.hpp"

#include "../controller/ContactorController.hpp"
#include "../service/ContactorInfoMessageService.hpp"
#include "../net/CANTransceiver.hpp"

osPriority ContactorInfoDispatcher::thread_priority = osPriorityNormal;
uint32_t ContactorInfoDispatcher::thread_period = 200; // ms

void ContactorInfoDispatcher::initialise()
{
    // Empty
}

void ContactorInfoDispatcher::procedure()
{
    ContactorInfoMessageService<CANTransceiver>::send
    	( ContactorController::pc_coil_status()
    	, ContactorController::pc_feedback_status()
    	, ContactorController::pc_hwerr_status()
    	, ContactorController::run_coil_status()
    	, ContactorController::run_feedback_status()
    	, ContactorController::run_hwerr_status() );
}
