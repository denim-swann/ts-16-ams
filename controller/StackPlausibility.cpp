// Team Swinburne 2016

#include "StackPlausibility.hpp"

#include "StateController.hpp"

osPriority StackPlausibility::thread_priority = osPriorityNormal;

InterruptIn StackPlausibility::battery_undervoltage(STATUS_BATTERY_UNDERVOLTAGE_PIN);
InterruptIn StackPlausibility::battery_overvoltage(STATUS_BATTERY_OVERVOLTAGE_PIN);
InterruptIn StackPlausibility::service_disconnects(STATUS_SERVICE_DISCONNECT_PIN);
InterruptIn StackPlausibility::precharge_oor(STATUS_PCOOR_PIN);
InterruptIn StackPlausibility::precharge_99(STATUS_PCUNFIN_PIN);
InterruptIn StackPlausibility::contactor_power(STATUS_RELAY_POWER_DETECT_PIN);

void StackPlausibility::initialise()
{
    service_disconnects.rise(&disconnect_interrupt);
    battery_undervoltage.fall(&buv_interrupt);
    battery_overvoltage.fall(&bov_interrupt);
    precharge_oor.fall(&pc_oor_interrupt);
    precharge_99.fall(&pc_99_interrupt);
    contactor_power.rise(&contactor_power_lost_interrupt);
}

void StackPlausibility::buv_interrupt()
{
    m_thread.signal_set(SIGNAL_BATTERY_UNDERVOLTAGE);
}

void StackPlausibility::bov_interrupt()
{
    m_thread.signal_set(SIGNAL_BATTERY_OVERVOLTAGE);
}

void StackPlausibility::disconnect_interrupt()
{
    m_thread.signal_set(SIGNAL_SERVICE_DISCONNECTS_REMOVED);
}

void StackPlausibility::pc_oor_interrupt()
{
    m_thread.signal_set(SIGNAL_PCOOR_UNASSERT);
}

void StackPlausibility::pc_99_interrupt()
{
    m_thread.signal_set(SIGNAL_PC99_UNASSERT);
}

void StackPlausibility::contactor_power_lost_interrupt()
{
    m_thread.signal_set(SIGNAL_CONTACTOR_POWER_LOST);
}

bool StackPlausibility::undervoltage_status()
{
    return battery_undervoltage;
}

bool StackPlausibility::overvoltage_status()
{
    return battery_overvoltage;
}

bool StackPlausibility::disconnects_status()
{
    return !service_disconnects;
}

bool StackPlausibility::precharge_oor_status()
{
    return precharge_oor;
}

bool StackPlausibility::precharge_99_status()
{
    return precharge_99;
}

bool StackPlausibility::contactor_power_status()
{
    return !contactor_power;
}

bool StackPlausibility::is_plausible()
{
    return /*undervoltage_status() && overvoltage_status() && disconnects_status()*/ true;
}

void StackPlausibility::procedure()
{
    for (;;) {
        osEvent evt = Thread::signal_wait(0);
        if (evt.status == osEventSignal) {
            if (!is_plausible()) {
                // TODO: this can only communicate a plausibility error ...
                StateController::notify_event(sceStackPlausibilityError);
            }
        }
    }
}
