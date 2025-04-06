#include "services.h"
void service_pushed(void) {
    if (FEATURE_ENABLED_HARDWARE_INTERRUPT_REQUEST_Is_High()) {
        if (PUSHED_INTERRUPT_Is_High()) {
            LIGHTBLUE_Hardware_Interrupt();
            PUSHED_INTERRUPT_SetLow();
        }
    }
}

void service_acceleremoterInterrupt(void) {
    if (FEATURE_ENABLED_ACC_FLAT_STATE_Is_High()) {
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

void service_thermocouple(void) {
    if (FEATURE_ENABLED_THERMOCOUPLE_TEMPERATURE_Is_High()) {
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