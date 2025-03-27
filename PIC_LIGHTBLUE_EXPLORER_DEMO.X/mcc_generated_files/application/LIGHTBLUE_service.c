/** \file   LIGHTBLUE_service.c
 *  \brief  This file contains APIs to access features for the Light Blue Explorer Example
 */
/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
 */

#include "LIGHTBLUE_service.h"

void LIGHTBLUE_Initialize(void) {
    bitMap.ioBitMap.gpioBitMap = 0x01;
    bitMap.ioStateBitMap.gpioStateBitMap = 0x01;
}

void LIGHTBLUE_TemperatureSensor(void) {
    char payload[5];
    int16_t temperature;

    *payload = '\0';
    MCP9844_GetTemperatureValue(&temperature);

    LIGHTBLUE_SplitWord(payload, temperature);

    LIGHTBLUE_SendPacket(TEMPERATURE_DATA_ID, payload);
}

void LIGHTBLUE_Send_Thermocouple(uint8_t* temperature) {
    char payload[10];
    *payload = '\0';
    for (int i = 0; i < 4; i++) {
        LIGHTBLUE_SplitByte(payload,*temperature++);
    }
    LIGHTBLUE_SendPacket(THERMOCOUPLE_TEMPERATURE_ID, payload);
}

void LIGHTBLUE_AccelSensor(void) {
    char payload[13];
    BMA253_ACCEL_DATA_t accelData;

    *payload = '\0';
    BMA253_GetAccelDataXYZ(&accelData);
    // Masking to ensure top nibble is always 0 as light blue expects
    // Exception may occur when highest byte is not 0
    LIGHTBLUE_SplitWord(payload, (accelData.x & 0x0FFF));
    LIGHTBLUE_SplitWord(payload, (accelData.y & 0x0FFF));
    LIGHTBLUE_SplitWord(payload, (accelData.z & 0x0FFF));

    LIGHTBLUE_SendPacket(ACCEL_DATA_ID, payload);
}

void LIGHTBLUE_PushButton(void) {
    char payload[3];
    uint8_t button = LIGHTBLUE_GetButtonValue();

    *payload = '\0';
    LIGHTBLUE_SplitByte(payload, button);

    LIGHTBLUE_SendPacket(BUTTON_STATE_ID, payload);
}

void LIGHTBLUE_AccState(void) {
    char payload[3];
    uint8_t acc = LIGHTBLUE_GetAccState();

    *payload = '\0';
    LIGHTBLUE_SplitByte(payload, acc);
    
    LIGHTBLUE_SendPacket(ACC_FLAT_STATE_ID, payload);
}

void LIGHTBLUE_LedState(void) {
    char payload[3];
    uint8_t led;

    led = DATA_LED_IDENTIFIER + LIGHTBLUE_GetDataLedValue();

    *payload = '\0';
    LIGHTBLUE_SplitByte(payload, led);

    LIGHTBLUE_SendPacket(LED_STATE_ID, payload);

    led = ERROR_LED_IDENTIFIER + LIGHTBLUE_GetErrorLedValue();
    *payload = '\0';
    LIGHTBLUE_SplitByte(payload, led);

    LIGHTBLUE_SendPacket(LED_STATE_ID, payload);
}

void LIGHTBLUE_SendProtocolVersion(void) {
    char payload[19];
    uint8_t value;
    uint8_t dataIndex;

    *payload = '\0';

    for (dataIndex = 0; dataIndex < strlen(protocol_version_number); dataIndex++) {
        value = protocol_version_number[dataIndex];
        LIGHTBLUE_SplitByte(payload, value);
    }

    LIGHTBLUE_SendPacket(PROTOCOL_VERSION_ID, payload);
}

void LIGHTBLUE_SendSerialData(char* serialData) {
    uint8_t length = strlen(serialData) * 2;

    RN487X.Write(START_BYTE);
    RN487X.Write(Hex(sequenceNumber++));
    RN487X.Write(SERIAL_DATA_ID);
    RN487X.Write(Hex(length >> 4));
    RN487X.Write(Hex(length));
    while (*serialData) {
        RN487X.Write(Hex(*serialData >> 4));
        RN487X.Write(Hex(*serialData++));
    }
    RN487X.Write(TERMINATION_BYTE);
}

void LIGHTBLUE_ParseIncomingPacket(char receivedByte) {
    static PACKET_PARSER_STATE_t parserState = IDLE;
    static uint8_t length = 0;
    static uint16_t data = 0;
    static char packetID = '\0';

    switch (parserState) {
        case SEQUENCE_NUMBER:
            //ignore sequence
            parserState = PACKET_ID;
            break;
        case PACKET_ID:
            packetID = receivedByte;
            parserState = PAYLOAD_SIZE_0;
            break;
        case PAYLOAD_SIZE_0:
            length = Ascii2Decimal(receivedByte);
            parserState = PAYLOAD_SIZE_1;
            break;
        case PAYLOAD_SIZE_1:
            length = (length << 4) + Ascii2Decimal(receivedByte);
            parserState = PAYLOAD_0;
            break;
        case PAYLOAD_0:
            data = Ascii2Decimal(receivedByte);
            length--;
            if (length == 0) {
                parserState = IDLE;
            } else {
                parserState = PAYLOAD_1;
            }
            break;
        case PAYLOAD_1:
            data = (data << 4) + Ascii2Decimal(receivedByte);
            LIGHTBLUE_PerformAction(packetID, data);
            length--;
            if (length == 0) {
                parserState = IDLE;
            } else {
                parserState = PAYLOAD_0;
            }
            break;
        case IDLE:
        default:
            if (receivedByte == START_BYTE) {
                parserState = SEQUENCE_NUMBER;
            }
            break;
    }
}

static void LIGHTBLUE_SendPacket(char packetID, char* payload) {
    RN487X.Write(START_BYTE);
    RN487X.Write(Hex(sequenceNumber++));
    RN487X.Write(packetID);
    RN487X.Write(Hex(strlen(payload) >> 4));
    RN487X.Write(Hex(strlen(payload)));
    while (*payload) {
        RN487X.Write((*(uint8_t *) payload++));
    }
    RN487X.Write(TERMINATION_BYTE);
}

static void LIGHTBLUE_SplitWord(char* payload, int16_t value) {
    LIGHTBLUE_SplitByte(payload, value);
    LIGHTBLUE_SplitByte(payload, value >> 8);
}

static void LIGHTBLUE_SplitByte(char* payload, int8_t value) {
    payload += strlen(payload);
    *payload++ = Hex(value >> 4);
    *payload++ = Hex(value);
    *payload = '\0';
}

static uint8_t LIGHTBLUE_GetButtonValue(void) {
    return NOT_PRESSED_STATE - PushButtonGetValue(); // This is forcing proper data for LightBlue
}

static uint8_t LIGHTBLUE_GetAccState(void) {
    return accelerometerInterruptBits.AccelerometerInterruptBits;
}

static uint8_t LIGHTBLUE_GetDataLedValue(void) {
    return LED_OFF_STATE - DATA_LED_GetValue(); // This is forcing proper data for LightBlue
}

static uint8_t LIGHTBLUE_GetErrorLedValue(void) {
    return LED_OFF_STATE - ERROR_LED_VALUE;
}

static void LIGHTBLUE_SetErrorLedValue(bool value) {
    ERROR_LED_VALUE = LED_OFF_STATE - value;
    LIGHTBLUE_UpdateErrorLed();
}

static void LIGHTBLUE_UpdateErrorLed(void) {
    RN487X_EnterCmdMode();
    RN487X.DelayMs(CMD_MODE_DELAY_TIME);
    RN487X_SetOutputs(bitMap);
    RN487X_EnterDataMode();
}

static void LIGHTBLUE_PerformAction(char id, uint8_t data) {
    uint8_t led;

    switch (id) {
        case LED_STATE_ID:
            led = (data >> 4) & NIBBLE_MASK;
            if (led == DATA_LED_IDENTIFIER) {
                if ((data & NIBBLE_MASK) == LIGHTBLUE_OFF) {
                    DataLedOff();
                } else {
                    DataLedOn();
                }
            } else {
                LIGHTBLUE_SetErrorLedValue(data & NIBBLE_MASK);
            }
            break;
        case RESET_REQUEST_ID:
            RESET();
            break;
        case SERIAL_DATA_ID:
            uart[UART_CDC].Write(data); // echo out the terminal for now
            break;
        case ACC_FLAT_STATE_ID:
            ACKNOWLEDGED_ACC_FLAT_STATE_SetLow();
            break;
        case THERMOCOUPLE_TEMPERATURE_ID:
            ACKNOWLEDGED_THERMOCOUPLE_TEMPERATURE_SetLow();
            break;
        case BUZZ_REQUEST_ID:
            ACKNOWLEDGED_BUZZ_REQUEST_SetLow();
            break;
        case ALERT_REQUEST_ID:
            ACKNOWLEDGED_ALERT_REQUEST_SetLow();
            break;
        case HARDWARE_INTERRUPT_REQUEST_ID:
            ACKNOWLEDGED_HARDWARE_INTERRUPT_REQUEST_SetLow();
            break;
        default:
            break;
    }
}