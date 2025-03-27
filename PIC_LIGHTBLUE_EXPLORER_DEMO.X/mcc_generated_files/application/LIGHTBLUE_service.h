/** \file   LIGHTBLUE_service.h
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
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
// APP Dependencies
#include "../rn487x/rn487x.h"
#include "../rn487x/rn487x_interface.h"
#include "BMA253_accel.h"
#include "MCP9844_temp_sensor.h"
#include "../pin_manager.h"
#include "../drivers/uart.h"

#ifndef LIGHTBLUE_SERVICE_H
#define	LIGHTBLUE_SERVICE_H

/**
 \ingroup LIGHTBLUE
 \brief  Public function used for initialization of RN487X module conditions. \n
 Application configures the GPIO state & value used for the demonstration. 
 \return void \n
 */
void LIGHTBLUE_Initialize(void);

void setProtocol_Features(void);
void LIGHTBLUE_Hardware_Interrupt();
/**
 \ingroup LIGHTBLUE
 \brief  Public function used to issue Temperature Sensor data to the Light Blue application \n
 This function will request temperature data from the MCP9844, format it according to the Light Blue
 protocol requirements using 'T' for the ID, Temperature data is sent as a 16-bit hex value 
 representing the celsius value; fahrenheit is converted through the BLE application. 
 Actual Temperature value is provided as described below:
     b15        b14             b13           b12    b11    b10   b9    b8
 Ta vs TCrit  Ta vs TUpper   Ta vs TLower    Sign    2^7    2^6   2^5   2^4
     b7         b6              b5             b4     b3     b2    b1    b0 
     2^3        2^2             2^1           2^0    2^-1   2^-2  2^-3  2^-4
 \return void \n
 */
void LIGHTBLUE_TemperatureSensor(void);
/**
 \ingroup LIGHTBLUE
 \brief  Public function used to issue Acceleration Sensor data to the Light Blue application \n
 This function will request X, Y, Z data from the BMA253, format it according to the Light Blue
 protocol requirements using 'A' for the ID, data is sent as a 16-bit hex values 
 representing the signed acceleration data in 12-bit form with top 4 bits cleared to 0's.
 Actual value is provided as described below:
     b15    b14     b13     b12    b11    b10   b9      b8
     0      0       0       0      Sign   1g    500mg   250mg
     b7     b6      b5      b4     b3     b2    b1      b0 
    125mg   62.5mg  31.2mg  15.6mg 7.8mg  3.9mg 1.9mg  0.97mg
 \return void \n
 */
void LIGHTBLUE_AccelSensor(void);
/**
 \ingroup LIGHTBLUE
 \brief  Public function used to issue Push button state to the Light Blue application \n
 This function will read the SW0 state, format it according to the Light Blue
 protocol requirements using 'P' for the ID, data is sent as a 8-bit hex value 
 representing the button pressed state.
0 - Pressed
1- Released
 \return void \n
 */
void LIGHTBLUE_PushButton(void);
/**
 \ingroup LIGHTBLUE
 \brief  Public function used to issue LED state to the Light Blue application \n
 This function will read the (leds) state, format it according to the Light Blue
 protocol requirements using 'L' for the ID, data is sent as a 8-bit hex value 
 representing the LEDs status. DATA LED is always connected to the MCU and is read
through the LAT state, the ERR LED is controlled by the RN487X module; and requires
status request through GPIO register on the PIC board; ERR is connected to the MCU on
the AVR design. 
0 - ON
1- OFF
 \return void \n
 */
void LIGHTBLUE_LedState(void);
/**
 \ingroup LIGHTBLUE
 \brief  Public function used to issue Light Blue application protocol version \n
The protocol version can be used to alter behavior, or presentation through the
 BLE application. Therefore this function is used to inform the application what
 protocol format to be expected from the end-device. The packet is formated using
 the 'V' for the ID. 
E.G. "1.0.0"
 \return void \n
 */
void LIGHTBLUE_SendProtocolVersion(void);
/**
 \ingroup LIGHTBLUE
 \brief  Public function used to issue serial messages to the Light Blue application \n
This function will receive string message via the CDC terminal when CONNECTED, 
format it according to the Light Blue protocol requirements using 'S' for the ID, 
data is sent represented as Character's. Serial Message entered through the 
terminal are sent upon reception through the message parser. No interrogation of
string or data is performed by the end-device. 
 \return void \n
 */
void LIGHTBLUE_SendSerialData(char* serialData);
/**
 \ingroup LIGHTBLUE
 \brief  Public function used to parse message received from the Light Blue application \n
This function will receive byte by byte the string message captured by the RN487X 
formated between the '[' Start and ']' End Characters it according to the Light Blue protocol. 
The ID passed will be stored into packetID and is used to determine method of
packet data capture. Based upon the requested action. The processing of requested
action is performed upon parsing. Occurs through PerformAction function.
Packet ID Options:
'L' : LEDs Status Update
'S' : Serial Data to be bridged through CDC
'R' : Error has occurred; LED action will be requested
 \return void \n
 */
/**
\ingroup LIGHTBLUE
\def Hex(x) 
 * Macro used to MASK upper Nibble and extract HEX value of lower Nibble
 \param[in] x - 8bit unsigned char \n
 \return MASKED (4) bit value \n
 \retval MASKED lower Nibble Character \n
 */
#define Hex(x)                      (_hex[(x) & 0xF])
/**
\ingroup LIGHTBLUE
\def Ascii2Decimal(c) 
 * Macro used to MASK upper Nibble and extract HEX value of lower Nibble
 \param[in] x - 8bit unsigned char \n
 \return 8bit decimal value converted from 'char' ANCI Byte \n
 \retval decimal value of ancii character \n
 */
#define Ascii2Decimal(c)            (((c) <= '9') ? (c) - '0' : (c & 0x5f) - 'A' + 10)
/**
\ingroup LIGHTBLUE
\def DataLedOn() 
 * Macro used to turn ON the DATA LED. This will configured the connected pin
 * to be driven in the direction which turns the LED ON.
 * This LED is ACTIVE_LOW \n
 \return void \n
 */
#define DataLedOn()                 DATA_LED_SetLow()
/**
\ingroup LIGHTBLUE
\def DataLedOff() 
 * Macro used to turn OFF the DATA LED. This will configured the connected pin
 * to be driven in the direction which turns the LED OFF.
 * This LED is ACTIVE_LOW \n
 \return void \n
 */
#define DataLedOff()                DATA_LED_SetHigh()
/**
\ingroup LIGHTBLUE
\def START_BYTE
 * Macro used to add the character used as the Starting BYTE used for framing the
 LightBlue Application Packets. \n
 \return void \n
 */
#define START_BYTE                  ('[')
/**
\ingroup LIGHTBLUE
\def TERMINATION_BYTE
 * Macro used to add the character used as the Terminating BYTE used for framing the
 LightBlue Application Packets. \n
 \return void \n
 */
#define TERMINATION_BYTE            (']')
/**
\ingroup LIGHTBLUE
\def ERROR_LED_VALUE
 * Macro used to access the representation of the ERROR LED State.
 * On the PIC this is through the GPIO bitMap exchanged with the RN487X module.
 * On the AVR this is connected to a pin through the MCU device. \n
 \return void \n
 */
#define ERROR_LED_VALUE             (bitMap.ioStateBitMap.p2_2_state)
/**
\ingroup LIGHTBLUE
\def CMD_MODE_DELAY_TIME
 * Macro used to configure the MCUs BLOCKING delay length (mS) between MCU and
 * RN487X module communication. This delay is required for processing of some
 * requested actions. \n
 \return void \n
 */
#define CMD_MODE_DELAY_TIME         (20)
/**
\ingroup LIGHTBLUE
\def LED_OFF_STATE
 * Macro used to indicate the Board Specific Package (BSP) setting for the LED
 * bias configuration, determined on the ACTIVE_STATE tied High/Low. 
 * On this board, the LED is ACTIVE LOW, so HIGH will be the OFF_STATE \n
 \return void \n
 */
#define LED_OFF_STATE               (0x01)
/**
\ingroup LIGHTBLUE
\def NOT_PRESSED_STATE
 * Macro used to indicate the Board Specific Package (BSP) setting for the BUTTON
 * bias configuration, determined on the ACTIVE_STATE pull up/down.
 * On this board, the BUTTON (switch) is ACTIVE LOW, so HIGH will be the NOT PRESSED \n
 \return void \n
 */
#define NOT_PRESSED_STATE           (0x01)
/**
 * \ingroup LIGHTBLUE
 * \def PushButtonGetValue()
 * Macro used to define the BT_MODE_GetValue() function for use in Lightblue.
 * Push button is tied to both the Microcontroller and BLE module. To Lightblue
 * it is the Push Button.
 * \return Push button value
 */
#define PushButtonGetValue()        BT_MODE_GetValue()
/**
\ingroup LIGHTBLUE
\def LIGHTBLUE_OFF
 * Macro used to MASK the value of the LED.
 * This is a MASK used during processing of the Light related packets. \n
 \return void \n
 */
#define LIGHTBLUE_OFF               (0x00)
/**
\ingroup LIGHTBLUE
\def DATA_LED_IDENTIFIER
 * Macro used to MASK the value of the DATA LED.
 * This is a MASK used during processing of the Light related packets. \n
 \return void \n
 */
#define DATA_LED_IDENTIFIER         (0x00)
/**
\ingroup LIGHTBLUE
\def ERROR_LED_IDENTIFIER
 * Macro used to MASK the value of the ERROR LED. 
 * This is a MASK used during processing of the Light related packets. \n
 \return void \n
 */
#define ERROR_LED_IDENTIFIER        (0x10)
/**
\ingroup LIGHTBLUE
\def NIBBLE_MASK
 * Macro used to MASK the value of the LEDs as they are being toggle. 
 * This is a MASK used during processing of the Light related packets. \n
 \return void \n
 */
#define NIBBLE_MASK                 (0x01)

/**
 \ingroup LIGHTBLUE
 *! \struct PROTOCOL_PACKET_TYPES_t
 * A struct used to store possible Light Blue Protocol Packet ID types.
Specific processing of task coorilated to packet type handled in \ref 
LIGHTBLUE_PerformAction private function. Use of types is handled in packet
format functions used to specific application transmission features. 
 */
typedef enum {
    PROTOCOL_VERSION_ID                 = 'V',
    LED_STATE_ID                        = 'L',
    BUTTON_STATE_ID                     = 'P',
    TEMPERATURE_DATA_ID                 = 'T',
    ACCEL_DATA_ID                       = 'X',
    SERIAL_DATA_ID                      = 'S',
    ERROR_ID                            = 'R',
    UI_CONFIG_DATA_ID                   = 'U',
    ACC_FLAT_STATE_ID                   = 'F',
    THERMOCOUPLE_TEMPERATURE_ID         = 'K',
    RESET_REQUEST_ID                    = 'O',
    BUZZ_REQUEST_ID                     = 'B',
    ALERT_REQUEST_ID                    = 'A',
    HARDWARE_INTERRUPT_REQUEST_ID       = 'H'        
} PROTOCOL_PACKET_TYPES_t;

/**
 \ingroup LIGHTBLUE
 *! \struct PACKET_PARSER_STATE_t
 * A struct used to manage the Packet Processor State upon reception of a framed
message from the RN487X device. These states are used to extract packet data in
the \ref LIGHTBLUE_PerformAction task processing function. 
 */
typedef enum {
    IDLE = 0,
    SEQUENCE_NUMBER = 1,
    PACKET_ID = 2,
    PAYLOAD_SIZE_0 = 3,
    PAYLOAD_SIZE_1 = 4,
    PAYLOAD_0 = 5,
    PAYLOAD_1 = 6

} PACKET_PARSER_STATE_t;

const char * const protocol_version_number = "1.1.0"; /**< Local Const Variable used to represent Light Blue Protocol version used by application */
static char _hex[] = "0123456789ABCDEF"; /**< Local Variable used for Masking a Hex value result */
static uint8_t sequenceNumber = 0; /**< Local Variable used to keep track of the number of TRANSMIT packets sent from device*/
static volatile rn487x_gpio_bitmap_t bitMap; /**< Local Variable used managing stored state of GPIO pin controlled by RN487X */

/**
 \ingroup LIGHTBLUE
 \brief  Private function to send a properly formated Light Blue Application Packet  \n
This function is used to properly format the Light Blue Packet into the desire format.
Format is: [0]    [1]     [2]  [ 3 + 4 ]    [5 -> nSize]  [n+5]
            [    SeqID   Type  Payload Size    Payload       ]
   [0]: '['              : Start Bracket Character
   [1]: '#'              : (0-F) Hex value used for Sequence ID used to maintain track of Rx/Tx Order
   [2]: 'Type ID'        : (V,L,P,T,X,S,R,U) Char used to Identify Data/Packet Type
 [3+4]: 'Payload Size'   : Size of Data Payload expected starting with Byte 4 --> N
[5->n]: 'Payload'        : Data of Payload
 [n+5]: ']'              : End Bracket Character
 \return void \n
 */
static void LIGHTBLUE_SendPacket(char packetID, char* payload);
/**
 \ingroup LIGHTBLUE
 \brief  Private function to split a Word (16bit) value into (2) (8bit) Byte values.  \n
This function is used break a (4) digit, 16bit value into pieces and load it back into
the payload as a 'char' representation of the number presented as a hex value. This is later
broken down into Nibble format through the SplitByte code. 
 \return void \n
 */
static void LIGHTBLUE_SplitWord(char* payload, int16_t value);
/**
 \ingroup LIGHTBLUE
 \brief  Private function to split a Byte (8bit) value into (2) (4bit) Nibble values.  \n
This function is used break a (2) digit, 8bit value into pieces and load it back into
the payload as a 'char' representation of the number presented as a hex value. 
 \return void \n
 */
static void LIGHTBLUE_SplitByte(char* payload, int8_t value);
/**
 \ingroup LIGHTBLUE
 \brief  Private function used to request status of PUSH BUTTON reading the PORT value. \n
This function is used to read PORT value read from pin connected to PUSH BUTTON masked against NOT_PRESSED state.
 \return uint8_t BUTTON STATUS - Status 0 (Pressed) | 1 (Release) \n
 \retval Status 0 (OFF) | 1 (ON) \n
 */
static uint8_t LIGHTBLUE_GetButtonValue(void);

/**
 \ingroup LIGHTBLUE
 \brief  Private function used to request status of ACCELEROMETER reading the PORT value. \n
 */

static uint8_t LIGHTBLUE_GetAccState(void);


/**
 \ingroup LIGHTBLUE
 \brief  Private function used to request status of DATA LED from reading the PORT value. \n
This function is used to read PORT value from pin connected to LED masked against OFF state.
 \return uint8_t DATA LED - Status 0 (OFF) | 1 (ON) \n
 \retval Status 0 (OFF) | 1 (ON) \n
 */
static uint8_t LIGHTBLUE_GetDataLedValue(void);

/**
 \ingroup LIGHTBLUE
 \brief  Private function used to request status of ERROR LED from local maintain variable. \n
This function is used to return ERROR LED status as the END-DEVICE knows it masked against OFF state.
 \return uint8_t ERROR LED - Status 0 (OFF) | 1 (ON) \n
 \retval Status 0 (OFF) | 1 (ON) \n
 */
static uint8_t LIGHTBLUE_GetErrorLedValue(void);
/**
 \ingroup LIGHTBLUE
 \brief  Private function used to request action from the RN487X module \n
This function is used to enter device into Command Mode, Update GPIO State, and return to Data Mode.
 \param[in] value - bool Set ERROR LED State as ON  \n
 \return void \n
 */
static void LIGHTBLUE_SetErrorLedValue(bool value);
/**
 \ingroup LIGHTBLUE
 \brief  Private function used process requested actions from the Light Blue application \n
This function is used to update the state of the ERROR LED which is controlled via the
 * RN487X through the GPIO command on the PIC platform. On the AVR board this is MCU (LAT) controlled.
 \return void \n
 */
static void LIGHTBLUE_UpdateErrorLed(void);
/**
 \ingroup LIGHTBLUE
 \brief  Private function used process requested actions from the Light Blue application \n
This function inspect the Packet Type ID to determine coordinated action.
The responsibilities include LED State Control, and Serial Message Bridge.
For Serial Message Bridge, the string 'message' is iterated through byte by byte.
Supported Type ID Options:
'L' : LEDs Status Update
'S' : Serial Data to be bridged through CDC
 \param[in] id - char representing the Type ID ('L' || 'S')  \n
 \param[in] data - 8bit unsigned value; (- - - - - - ERR DATA), (Single Char from String)\n
 \return void \n
 */
static void LIGHTBLUE_PerformAction(char id, uint8_t data);

void LIGHTBLUE_ParseIncomingPacket(char receivedByte);

void LIGHTBLUE_AccState(void); 
void LIGHTBLUE_Send_Thermocouple(uint8_t* temperature);

typedef union {
    struct {
        unsigned ACC_FLAT_STATE             : 1;
        unsigned THERMOCOUPLE_TEMPERATURE   : 1;
        unsigned BUZZ_REQUEST               : 1;
        unsigned ALERT_REQUEST              : 1;
        unsigned HARDWARE_INTERRUPT_REQUEST : 1;
        unsigned LED_STATE                  : 1;
        unsigned RESET_REQUEST              : 1;
        unsigned SERIAL_DATA                : 1;
    };
    uint8_t FeatureBits;
}FeatureBits_t;

static FeatureBits_t FeatureBits = { .FeatureBits = 0 };

#define ACKNOWLEDGED_ACC_FLAT_STATE_SetHigh()                           (FeatureBits.ACC_FLAT_STATE = 1) 
#define ACKNOWLEDGED_ACC_FLAT_STATE_SetLow()                            (FeatureBits.ACC_FLAT_STATE = 0) 
#define ACKNOWLEDGED_ACC_FLAT_STATE_Toggle()                            (FeatureBits.ACC_FLAT_STATE = ~FeatureBits.ACC_FLAT_STATE)
#define ACKNOWLEDGED_ACC_FLAT_STATE_GetValue()                          (FeatureBits.ACC_FLAT_STATE)
#define ACKNOWLEDGED_ACC_FLAT_STATE_Is_High()                           (FeatureBits.ACC_FLAT_STATE == 1)

#define ACKNOWLEDGED_THERMOCOUPLE_TEMPERATURE_SetHigh()                 (FeatureBits.THERMOCOUPLE_TEMPERATURE = 1) 
#define ACKNOWLEDGED_THERMOCOUPLE_TEMPERATURE_SetLow()                  (FeatureBits.THERMOCOUPLE_TEMPERATURE = 0) 
#define ACKNOWLEDGED_THERMOCOUPLE_TEMPERATURE_Toggle()                  (FeatureBits.THERMOCOUPLE_TEMPERATURE = ~FeatureBits.THERMOCOUPLE_TEMPERATURE)
#define ACKNOWLEDGED_THERMOCOUPLE_TEMPERATURE_GetValue()                (FeatureBits.THERMOCOUPLE_TEMPERATURE)
#define ACKNOWLEDGED_THERMOCOUPLE_TEMPERATURE_Is_High()                 (FeatureBits.THERMOCOUPLE_TEMPERATURE == 1)

#define ACKNOWLEDGED_BUZZ_REQUEST_SetHigh()                             (FeatureBits.BUZZ_REQUEST = 1) 
#define ACKNOWLEDGED_BUZZ_REQUEST_SetLow()                              (FeatureBits.BUZZ_REQUEST = 0) 
#define ACKNOWLEDGED_BUZZ_REQUEST_Toggle()                              (FeatureBits.BUZZ_REQUEST = ~FeatureBits.BUZZ_REQUEST)
#define ACKNOWLEDGED_BUZZ_REQUEST_GetValue()                            (FeatureBits.BUZZ_REQUEST)
#define ACKNOWLEDGED_BUZZ_REQUEST_Is_High()                             (FeatureBits.BUZZ_REQUEST == 1)

#define ACKNOWLEDGED_ALERT_REQUEST_SetHigh()                            (FeatureBits.ALERT_REQUEST = 1) 
#define ACKNOWLEDGED_ALERT_REQUEST_SetLow()                             (FeatureBits.ALERT_REQUEST = 0) 
#define ACKNOWLEDGED_ALERT_REQUEST_Toggle()                             (FeatureBits.ALERT_REQUEST = ~FeatureBits.ALERT_REQUEST)
#define ACKNOWLEDGED_ALERT_REQUEST_GetValue()                           (FeatureBits.ALERT_REQUEST)
#define ACKNOWLEDGED_ALERT_REQUEST_Is_High()                            (FeatureBits.ALERT_REQUEST == 1)

#define ACKNOWLEDGED_HARDWARE_INTERRUPT_REQUEST_SetHigh()               (FeatureBits.HARDWARE_INTERRUPT_REQUEST = 1) 
#define ACKNOWLEDGED_HARDWARE_INTERRUPT_REQUEST_SetLow()                (FeatureBits.HARDWARE_INTERRUPT_REQUEST = 0) 
#define ACKNOWLEDGED_HARDWARE_INTERRUPT_REQUEST_Toggle()                (FeatureBits.HARDWARE_INTERRUPT_REQUEST = ~FeatureBits.HARDWARE_INTERRUPT_REQUEST)
#define ACKNOWLEDGED_HARDWARE_INTERRUPT_REQUEST_GetValue()              (FeatureBits.HARDWARE_INTERRUPT_REQUEST)
#define ACKNOWLEDGED_HARDWARE_INTERRUPT_REQUEST_Is_High()               (FeatureBits.HARDWARE_INTERRUPT_REQUEST == 1)

#define ACKNOWLEDGED_SERIAL_DATA_SetHigh()                              (FeatureBits.SERIAL_DATA = 1) 
#define ACKNOWLEDGED_SERIAL_DATA_SetLow()                               (FeatureBits.SERIAL_DATA = 0) 
#define ACKNOWLEDGED_SERIAL_DATA_Toggle()                               (FeatureBits.SERIAL_DATA = ~FeatureBits.SERIAL_DATA)
#define ACKNOWLEDGED_SERIAL_DATA_GetValue()                             (FeatureBits.SERIAL_DATA)
#define ACKNOWLEDGED_SERIAL_DATA_Is_High()                              (FeatureBits.SERIAL_DATA == 1)

#define ACKNOWLEDGED_RESET_REQUEST_SetHigh()                            (FeatureBits.RESET_REQUEST = 1) 
#define ACKNOWLEDGED_RESET_REQUEST_SetLow()                             (FeatureBits.RESET_REQUEST = 0) 
#define ACKNOWLEDGED_RESET_REQUEST_Toggle()                             (FeatureBits.RESET_REQUEST = ~FeatureBits.RESET_REQUEST)
#define ACKNOWLEDGED_RESET_REQUEST_GetValue()                           (FeatureBits.RESET_REQUEST)
#define ACKNOWLEDGED_RESET_REQUEST_Is_High()                            (FeatureBits.RESET_REQUEST == 1)

#define ACKNOWLEDGED_LED_STATE_SetHigh()                                (FeatureBits.LED_STATE = 1) 
#define ACKNOWLEDGED_LED_STATE_SetLow()                                 (FeatureBits.LED_STATE = 0) 
#define ACKNOWLEDGED_LED_STATE_Toggle()                                 (FeatureBits.LED_STATE = ~FeatureBits.LED_STATE)
#define FACKNOWLEDGED_LED_STATE_GetValue()                              (FeatureBits.LED_STATE)
#define ACKNOWLEDGED_LED_STATE_REQUEST_Is_High()                        (FeatureBits.LED_STATE == 1)


static FeatureBits_t FEATURE_ENABLEDBits= { .FeatureBits = 0 };

#define FEATURE_ENABLED_ACC_FLAT_STATE_SetHigh()                   (FEATURE_ENABLEDBits.ACC_FLAT_STATE = 1) 
#define FEATURE_ENABLED_ACC_FLAT_STATE_SetLow()                    (FEATURE_ENABLEDBits.ACC_FLAT_STATE = 0) 
#define FEATURE_ENABLED_ACC_FLAT_STATE_Toggle()                    (FEATURE_ENABLEDBits.ACC_FLAT_STATE = ~FEATURE_ENABLEDBits.ACC_FLAT_STATE)
#define FEATURE_ENABLED_ACC_FLAT_STATE_GetValue()                  (FEATURE_ENABLEDBits.ACC_FLAT_STATE)
#define FEATURE_ENABLED_ACC_FLAT_STATE_Is_High()                   (FEATURE_ENABLEDBits.ACC_FLAT_STATE == 1)

#define FEATURE_ENABLED_THERMOCOUPLE_TEMPERATURE_SetHigh()         (FEATURE_ENABLEDBits.THERMOCOUPLE_TEMPERATURE = 1) 
#define FEATURE_ENABLED_THERMOCOUPLE_TEMPERATURE_SetLow()          (FEATURE_ENABLEDBits.THERMOCOUPLE_TEMPERATURE = 0) 
#define FEATURE_ENABLED_THERMOCOUPLE_TEMPERATURE_Toggle()          (FEATURE_ENABLEDBits.THERMOCOUPLE_TEMPERATURE = ~FEATURE_ENABLEDBits.THERMOCOUPLE_TEMPERATURE)
#define FEATURE_ENABLED_THERMOCOUPLE_TEMPERATURE_GetValue()        (FEATURE_ENABLEDBits.THERMOCOUPLE_TEMPERATURE)
#define FEATURE_ENABLED_THERMOCOUPLE_TEMPERATURE_Is_High()         (FEATURE_ENABLEDBits.THERMOCOUPLE_TEMPERATURE == 1)

#define FEATURE_ENABLED_BUZZ_REQUEST_SetHigh()                     (FEATURE_ENABLEDBits.BUZZ_REQUEST = 1) 
#define FEATURE_ENABLED_TBUZZ_REQUEST_SetLow()                     (FEATURE_ENABLEDBits.BUZZ_REQUEST = 0) 
#define FEATURE_ENABLED_BUZZ_REQUEST_Toggle()                      (FEATURE_ENABLEDBits.BUZZ_REQUEST = ~FEATURE_ENABLEDBits.BUZZ_REQUEST)
#define FEATURE_ENABLED_BUZZ_REQUEST_GetValue()                    (FEATURE_ENABLEDBits.BUZZ_REQUEST)
#define FEATURE_ENABLED_BUZZ_REQUEST_Is_High()                     (FEATURE_ENABLEDBits.BUZZ_REQUEST == 1)

#define FEATURE_ENABLED_ALERT_REQUEST_SetHigh()                    (FEATURE_ENABLEDBits.ALERT_REQUEST = 1) 
#define FEATURE_ENABLED_ALERT_REQUEST_SetLow()                     (FEATURE_ENABLEDBits.ALERT_REQUEST = 0) 
#define FEATURE_ENABLED_ALERT_REQUEST_Toggle()                     (FEATURE_ENABLEDBits.ALERT_REQUEST = ~FEATURE_ENABLEDBits.ALERT_REQUEST)
#define FEATURE_ENABLED_ALERT_REQUEST_GetValue()                   (FEATURE_ENABLEDBits.ALERT_REQUEST)
#define FEATURE_ENABLED_ALERT_REQUEST_Is_High()                    (FEATURE_ENABLEDBits.ALERT_REQUEST == 1)

#define FEATURE_ENABLED_HARDWARE_INTERRUPT_REQUEST_SetHigh()       (FEATURE_ENABLEDBits.HARDWARE_INTERRUPT_REQUEST = 1) 
#define FEATURE_ENABLED_HARDWARE_INTERRUPT_REQUEST_SetLow()        (FEATURE_ENABLEDBits.HARDWARE_INTERRUPT_REQUEST = 0) 
#define FEATURE_ENABLED_HARDWARE_INTERRUPT_REQUEST_Toggle()        (FEATURE_ENABLEDBits.HARDWARE_INTERRUPT_REQUEST = ~FEATURE_ENABLEDBits.HARDWARE_INTERRUPT_REQUEST)
#define FEATURE_ENABLED_HARDWARE_INTERRUPT_REQUEST_GetValue()      (FEATURE_ENABLEDBits.HARDWARE_INTERRUPT_REQUEST)
#define FEATURE_ENABLED_HARDWARE_INTERRUPT_REQUEST_Is_High()       (FEATURE_ENABLEDBits.HARDWARE_INTERRUPT_REQUEST == 1)

#define FEATURE_ENABLED_SERIAL_DATA_SetHigh()                      (FEATURE_ENABLEDBits.SERIAL_DATA = 1) 
#define FEATURE_ENABLED_SERIAL_DATA_SetLow()                       (FEATURE_ENABLEDBits.SERIAL_DATA = 0) 
#define FEATURE_ENABLED_SERIAL_DATA_Toggle()                       (FEATURE_ENABLEDBits.SERIAL_DATA = ~FEATURE_ENABLEDBits.SERIAL_DATA)
#define FEATURE_ENABLED_SERIAL_DATA_GetValue()                     (FEATURE_ENABLEDBits.SERIAL_DATA)
#define FEATURE_ENABLED_SERIAL_DATA_Is_High()                      (FEATURE_ENABLEDBits.SERIAL_DATA == 1)

#define FEATURE_ENABLED_RESET_REQUEST_SetHigh()                    (FEATURE_ENABLEDBits.RESET_REQUEST = 1) 
#define FEATURE_ENABLED_RESET_REQUEST_SetLow()                     (FEATURE_ENABLEDBits.RESET_REQUEST = 0) 
#define FEATURE_ENABLED_RESET_REQUEST_Toggle()                     (FEATURE_ENABLEDBits.RESET_REQUEST = ~FEATURE_ENABLEDBits.RESET_REQUEST)
#define FEATURE_ENABLED_RESET_REQUEST_GetValue()                   (FEATURE_ENABLEDBits.RESET_REQUEST)
#define FEATURE_ENABLED_RESET_REQUEST_Is_High()                    (FEATURE_ENABLEDBits.RESET_REQUEST == 1)

#define FEATURE_ENABLED_LED_STATE_SetHigh()                        (FEATURE_ENABLEDBits.LED_STATE = 1) 
#define FEATURE_ENABLED_LED_STATE_SetLow()                         (FEATURE_ENABLEDBits.LED_STATE = 0) 
#define FEATURE_ENABLED_LED_STATE_Toggle()                         (FEATURE_ENABLEDBits.LED_STATE = ~FEATURE_ENABLEDBits.LED_STATE)
#define FEATURE_ENABLED_LED_STATE_GetValue()                       (FEATURE_ENABLEDBits.LED_STATE)
#define FEATURE_ENABLED_LED_STATE_REQUEST_Is_High()                (FEATURE_ENABLEDBits.LED_STATE == 1)

#endif	/* LIGHTBLUE_SERVICE_H */

