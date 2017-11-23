// Team Swinburne 2016

#include "BMSController.hpp"

#include "Pinout.hpp"

osPriority BMSController::thread_priority = osPriorityRealtime;
uint32_t BMSController::thread_period = 100; // ms

DigitalIn BMSController::bmsOk(BMS_CONTROLLER_SCK_PIN, PullDown); // Temporary use of this pin for signalling OK
DigitalOut BMSController::performConversions(BMS_CONTROLLER_SS_PIN); // Temporary use of this pin for silencing
DigitalOut BMSController::amsOk(OUTPUT_AMS_OK_PIN, PullDown);

void BMSController::initialise()
{
    amsOk = false;
    performConversions = true;
}

bool BMSController::bms_status()
{
    return bmsOk;
}

void BMSController::set_conversions_enable(bool en)
{
    performConversions = en;
}

void BMSController::procedure()
{
    amsOk = bms_status();
}
