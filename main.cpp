// Team Swinburne 2016

#include "CANTransceiver.hpp"
#include "StateController.hpp"
#include "StackPlausibility.hpp"
#include "BMSController.hpp"
#include "ContactorController.hpp"
#include "HeartbeatDispatcher.hpp"
#include "StatusDispatcher.hpp"
#include "ContactorInfoDispatcher.hpp"

#include "mbed.h"

#ifndef MBED_TESTSUITE

int main()
{
    CANTransceiver::create();
    StateController::create();
    StackPlausibility::create();
    BMSController::create();
    ContactorController::create();
    HeartbeatDispatcher::create();
    StatusDispatcher::create();
    ContactorInfoDispatcher::create();

    for (;;) {
        Thread::wait(osWaitForever);
    }
}

#endif // MBED_TESTSUITE
