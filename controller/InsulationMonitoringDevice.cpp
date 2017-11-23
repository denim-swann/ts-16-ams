// Team Swinburne 2016

#include "InsulationMonitoringDevice.hpp"
#include "StateController.hpp"

osPriority InsulationMonitoringDevice::thread_priority = osPriorityNormal;

DigitalIn InsulationMonitoringDevice::_imd_signal(STATUS_IMD_PIN, PullUp); // pull only
InterruptIn InsulationMonitoringDevice::imd_signal(STATUS_IMD_PIN);

void InsulationMonitoringDevice::initialise()
{
    imd_signal.fall(&imd_interrupt);
}

void InsulationMonitoringDevice::imd_interrupt()
{
    m_thread.signal_set(SIGNAL_INSULATION_FAULT);
}

bool InsulationMonitoringDevice::imd_status()
{
    return !imd_signal;
}

void InsulationMonitoringDevice::procedure()
{
    for (;;) {
        osEvent evt = Thread::signal_wait(0);
        if (evt.status == osEventSignal) {
            if (!imd_status()) {
                StateController::notify_event(sceInsulationMonitoringDeviceError);
            }
        }
    }
}
