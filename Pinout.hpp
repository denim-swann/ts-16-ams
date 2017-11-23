// Team Swinburne 2016

#ifndef __AMS_PINOUT_HPP__
#define __AMS_PINOUT_HPP__

#define BMS_CONTROLLER_MOSI_PIN           (p5)
#define BMS_CONTROLLER_MISO_PIN           (p6)
#define BMS_CONTROLLER_SCK_PIN            (p7)
#define BMS_CONTROLLER_SS_PIN             (p11)

#define CAN_TRANSCIEVER_RXD_PIN           (p9)
#define CAN_TRANSCIEVER_TXD_PIN           (p10)

#define STATUS_PCOOR_PIN                  (p27)
#define STATUS_PCUNFIN_PIN                (p28)
#define STATUS_BATTERY_UNDERVOLTAGE_PIN   (p30)
#define STATUS_BATTERY_OVERVOLTAGE_PIN    (p29)
#define STATUS_RELAY_POWER_DETECT_PIN     (p18)
#define STATUS_SERVICE_DISCONNECT_PIN     (p13)
#define STATUS_HARDWARE_OVERCURRENT_PIN   (p12)
#define STATUS_IMD_PIN                    (p8)

#define OUTPUT_RUN_ENABLE_PIN             (p16)
#define RUN_AUX_FEEDBACK_PIN              (p20)
#define OUTPUT_PRECHARGE_ENABLE_PIN       (p17)
#define PRECHARGE_AUX_FEEDBACK_PIN        (p19)

#define OUTPUT_AMS_OK_PIN                 (p14)

#endif // __AMS_PINOUT_HPP__
