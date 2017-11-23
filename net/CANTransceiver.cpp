// Team Swinburne 2016

#include "CANTransceiver.hpp"

#include "CANIdentifiers.hpp"
#include "StateController.hpp"
#include "BMSController.hpp"

osPriority CANTransceiver::thread_priority = osPriorityAboveNormal;

CAN CANTransceiver::driver(CAN_TRANSCIEVER_RXD_PIN, CAN_TRANSCIEVER_TXD_PIN);
Mutex CANTransceiver::driver_lock;

Mail<CANMessage, CAN_TRANSCIEVER_BUFFER_SIZE> CANTransceiver::message_box;
CANMessage *CANTransceiver::next_message = NULL;

void CANTransceiver::initialise()
{
    driver.frequency(CAN_TRANSCIEVER_FREQUENCY);
    driver.attach(&CANTransceiver::rx_interrupt, CAN::RxIrq);
}

void CANTransceiver::rx_interrupt()
{
    static CANMessage trashcan;
    bool messages_remaining = true;

    do {
        if (next_message == NULL) {
            next_message = message_box.alloc();
        }

        if (next_message != NULL) {
            messages_remaining = driver.read(*next_message);
            if (messages_remaining != 0) {
                message_box.put(next_message);
                next_message = NULL;
            }
        } else {
            messages_remaining = driver.read(trashcan);
        }
    }
    while (messages_remaining);
}

uint8_t CANTransceiver::rderrors()
{
    return driver.rderror();
}

uint8_t CANTransceiver::tderrors()
{
    return driver.tderror();
}

int CANTransceiver::write(CANMessage msg)
{
    int ret;
    driver_lock.lock();
    ret = driver.write(msg);
    driver_lock.unlock();
    return ret;
}

void CANTransceiver::procedure()
{
    for (;;) {
        osEvent evt = message_box.get();
        if (evt.status == osEventMail) {
            CANMessage *msg = (CANMessage*)evt.value.p;
            if (msg != NULL) {
                dispatch_message(*msg);
                message_box.free(msg);
            }
        }
    }
}

void CANTransceiver::dispatch_message(CANMessage &msg)
{
    switch (msg.id) {
        case CANID_AMS_COMMAND_MESSAGE:
            if (msg.len >= 1) {
                bool en_tr = msg.data[0] & 1;
                bool dis_cnv = msg.data[0] & 2;
                StateController::push_tractive_enable(en_tr);
                BMSController::set_conversions_enable(!dis_cnv);
            }
            break;
    }
}
