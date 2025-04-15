#include "services.h"

void intiliase_services(void) {
    SERVICE_ACC_FLAT_STATE_SetHigh();
    SERVICE_THERMOCOUPLE_TEMPERATURE_SetHigh();
    SERVICE_HARDWARE_INTERRUPT_REQUEST_SetHigh();
}

void acc_flat_state() {
    if (SERVICE_ACC_FLAT_STATE_Is_High()) {
        if (ACC_INTERRUPT_Is_High()) {
            ACC_INTERRUPT_SetLow();
            ACC_INTERRUPT_FLAT_SetHigh();
            flats++;
            if (flats > 1) {
                ACC_INTERRUPT_FLAT_SetLow();
                LIGHTBLUE_AccState();
                flats = 0;
            }
        }
    }
}

void thermocouple_temperature() {
    if (SERVICE_THERMOCOUPLE_TEMPERATURE_Is_High()) {
        static uint8_t data[4];
        SPI_SS_EXT_DEVICE_SetLow();
        if (SPI2_Open(0)) {
            sendSpiReadRequest = false;
            SPI2_ReadBlock(data, 4);
            SPI_SS_EXT_DEVICE_SetHigh();
            LIGHTBLUE_Send_Thermocouple(data);
            SPI2_Close();
        }
    }
}

void hardware_interrupt_request() {
    if (SERVICE_HARDWARE_INTERRUPT_REQUEST_Is_High()) {
        if (PUSHED_INTERRUPT_Is_High()) {
            LIGHTBLUE_Hardware_Interrupt();
            PUSHED_INTERRUPT_SetLow();
        }
    }
}

void temperature_sensor() {
    if (SERVICE_TEMPERATURE_SENSOR_Is_High()) {
        LIGHTBLUE_TemperatureSensor();
    }
}

void accelerometer_sensor() {
    if (SERVICE_ACCELEROMETER_SENSOR_Is_High()) {
        LIGHTBLUE_AccelSensor();
    }
}

void push_button() {
    if (SERVICE_PUSH_BUTTON_Is_High()) {
        LIGHTBLUE_PushButton();
    }
}

void led_state() {
    if (SERVICE_LED_STATE_Is_High()) {
        LIGHTBLUE_LedState();
    }
}

void send_protocol_version() {
    if (SERVICE_SEND_PROTOCOL_VERSION_REQUEST_Is_High()) {
        LIGHTBLUE_SendProtocolVersion();
    }
}

void mirror_serial() {
    if (SERVICE_MIRROW_SERIAL_REQUEST_Is_High()) {
        while (RN487X_DataReady()) {
            LIGHTBLUE_ParseIncomingPacket(RN487X_Read());
        }
        while (uart[UART_CDC].DataReady()) {
            lightBlueSerial[serialIndex] = uart[UART_CDC].Read();
            if ((lightBlueSerial[serialIndex] == '\r')
                    || (lightBlueSerial[serialIndex] == '\n')
                    || (serialIndex == (sizeof (lightBlueSerial) - 1))) {
                lightBlueSerial[serialIndex] = '\0';
                LIGHTBLUE_SendSerialData(lightBlueSerial);
                serialIndex = 0;
            } else {
                serialIndex++;
            }
        }
    }
}

void spool_ble_rx() {
    while (RN487X_DataReady()) {
        uart[UART_CDC].Write(RN487X_Read());
    }
}

void spool_ble_tx() {
    while (uart[UART_CDC].DataReady()) {
        RN487X.Write(uart[UART_CDC].Read());
    }
}

