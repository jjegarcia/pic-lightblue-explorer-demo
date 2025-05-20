#include "../main.h"
#ifndef SERVICES_H
#define SERVICES_H
typedef union {
    struct {
        unsigned ACC_FLAT_STATE             : 1;
        unsigned THERMOCOUPLE_TEMPERATURE   : 1;
        unsigned HARDWARE_INTERRUPT_REQUEST : 1;
        unsigned TEMPERATURE_SENSOR         : 1;
        unsigned ACCELEROMETER_SENSOR       : 1;
        unsigned PUSH_BUTTON                : 1;
        unsigned LED_STATE                  : 1;
        unsigned SEND_PROTOCOL_VERSION      : 1;
        unsigned MIRROW_SERIAL              : 1;
    };
    uint8_t ServiceBits;
}ServiceBits_t;

volatile ServiceBits_t SERVICE = { .ServiceBits = 0 };
#define SERVICE_CLEAR_ALL (SERVICE.ServiceBits=0)

#define SERVICE_ACC_FLAT_STATE_SetHigh()                                (SERVICE.ACC_FLAT_STATE = 1)
#define SERVICE_ACC_FLAT_STATE_SetLow()                                 (SERVICE.ACC_FLAT_STATE = 0)
#define SERVICE_ACC_FLAT_STATE_Toggle()                                 (SERVICE.ACC_FLAT_STATE = ~SERVICE.ACC_FLAT_STATE)
#define SERVICE_ACC_FLAT_STATE_GetValue()                               (SERVICE.ACC_FLAT_STATE)
#define SERVICE_ACC_FLAT_STATE_Is_High()                                (SERVICE.ACC_FLAT_STATE == 1)

#define SERVICE_THERMOCOUPLE_TEMPERATURE_SetHigh()                      (SERVICE.THERMOCOUPLE_TEMPERATURE = 1)
#define SERVICE_THERMOCOUPLE_TEMPERATURE_SetLow()                       (SERVICE.THERMOCOUPLE_TEMPERATURE = 0)
#define SERVICE_THERMOCOUPLE_TEMPERATURE_Toggle()                       (SERVICE.THERMOCOUPLE_TEMPERATURE = ~SERVICE.THERMOCOUPLE_TEMPERATURE)
#define SERVICE_THERMOCOUPLE_TEMPERATURE_GetValue()                     (SERVICE.THERMOCOUPLE_TEMPERATURE)
#define SERVICE_THERMOCOUPLE_TEMPERATURE_Is_High()                      (SERVICE.THERMOCOUPLE_TEMPERATURE == 1)

#define SERVICE_HARDWARE_INTERRUPT_REQUEST_SetHigh()                    (SERVICE.HARDWARE_INTERRUPT_REQUEST = 1)
#define SERVICE_HARDWARE_INTERRUPT_REQUEST_SetLow()                     (SERVICE.HARDWARE_INTERRUPT_REQUEST = 0)
#define SERVICE_HARDWARE_INTERRUPT_REQUEST_Toggle()                     (SERVICE.HARDWARE_INTERRUPT_REQUEST = ~SERVICE.HARDWARE_INTERRUPT_REQUEST)
#define SERVICE_HARDWARE_INTERRUPT_REQUEST_GetValue()                   (SERVICE.HARDWARE_INTERRUPT_REQUEST)
#define SERVICE_HARDWARE_INTERRUPT_REQUEST_Is_High()                    (SERVICE.HARDWARE_INTERRUPT_REQUEST == 1)

#define SERVICE_TEMPERATURE_SENSOR_SetHigh()                            (SERVICE.TEMPERATURE_SENSOR = 1)
#define SERVICE_TEMPERATURE_SENSOR_SetLow()                             (SERVICE.TEMPERATURE_SENSOR = 0)
#define SERVICE_TEMPERATURE_SENSOR_Toggle()                             (SERVICE.TEMPERATURE_SENSOR = ~SERVICE.TEMPERATURE_SENSOR)
#define SERVICE_TEMPERATURE_SENSOR_GetValue()                           (SERVICE.TEMPERATURE_SENSOR)
#define SERVICE_TEMPERATURE_SENSOR_Is_High()                            (SERVICE.TEMPERATURE_SENSOR == 1)

#define SERVICE_ACCELEROMETER_SENSOR_SetHigh()                          (SERVICE.ACCELEROMETER_SENSOR = 1)
#define SERVICE_ACCELEROMETER_SENSOR_SetLow()                           (SERVICE.ACCELEROMETER_SENSOR = 0)
#define SERVICE_ACCELEROMETER_SENSOR_Toggle()                           (SERVICE.ACCELEROMETER_SENSOR = ~SERVICE.ACCELEROMETER_SENSOR)
#define SERVICE_ACCELEROMETER_SENSOR_GetValue()                         (SERVICE.ACCELEROMETER_SENSOR)
#define SERVICE_ACCELEROMETER_SENSOR_Is_High()                          (SERVICE.ACCELEROMETER_SENSOR == 1)

#define SERVICE_PUSH_BUTTON_SetHigh()                                   (SERVICE.PUSH_BUTTON = 1)
#define SERVICE_PUSH_BUTTON_SetLow()                                    (SERVICE.PUSH_BUTTON = 0)
#define SERVICE_PUSH_BUTTON_Toggle()                                    (SERVICE.PUSH_BUTTON = ~SERVICE.PUSH_BUTTON)
#define SERVICE_PUSH_BUTTON_GetValue()                                  (SERVICE.PUSH_BUTTON)
#define SERVICE_PUSH_BUTTON_Is_High()                                   (SERVICE.PUSH_BUTTON == 1)

#define SERVICE_LED_STATE_SetHigh()                                     (SERVICE.LED_STATE = 1)
#define SERVICE_LED_STATE_SetLow()                                      (SERVICE.LED_STATE = 0)
#define SERVICE_LED_STATE_Toggle()                                      (SERVICE.LED_STATE = ~SERVICE.LED_STATE)
#define SERVICE_LED_STATE_GetValue()                                    (SERVICE.LED_STATE)
#define SERVICE_LED_STATE_Is_High()                                     (SERVICE.LED_STATE == 1)

#define SERVICE_SEND_PROTOCOL_VERSION_SetHigh()                         (SERVICE.SEND_PROTOCOL_VERSION = 1)
#define SERVICE_SEND_PROTOCOL_VERSION_SetLow()                          (SERVICE.SEND_PROTOCOL_VERSION = 0)
#define SERVICE_SEND_PROTOCOL_VERSION_Toggle()                          (SERVICE.SEND_PROTOCOL_VERSION = ~SERVICE.SEND_PROTOCOL_VERSION)
#define FSERVICE_SEND_PROTOCOL_VERSION_GetValue()                       (SERVICE.SEND_PROTOCOL_VERSION)
#define SERVICE_SEND_PROTOCOL_VERSION_REQUEST_Is_High()                 (SERVICE.SEND_PROTOCOL_VERSION == 1)

#define SERVICE_MIRROW_SERIAL_SetHigh()                                 (SERVICE.MIRROW_SERIAL = 1)
#define SERVICE_MIRROW_SERIAL_SetLow()                                  (SERVICE.MIRROW_SERIAL = 0)
#define SERVICE_MIRROW_SERIAL_Toggle()                                  (SERVICE.MIRROW_SERIAL = ~SERVICE.MIRROW_SERIAL)
#define SERVICE_MIRROW_SERIAL_GetValue()                                (SERVICE.MIRROW_SERIAL)
#define SERVICE_MIRROW_SERIAL_REQUEST_Is_High()                         (SERVICE.MIRROW_SERIAL == 1)

void acc_flat_state();
void thermocouple_temperature();
void hardware_interrupt_request();
void temperature_sensor();
void accelerometer_sensor();
void push_button();
void led_state();
void send_protocol_version();
void intiliase_services();
void mirror_serial();
void spool_ble_tx();
void spool_ble_rx();
void flush_serial_to_ble();
void flush_ble_to_serial();
#endif
