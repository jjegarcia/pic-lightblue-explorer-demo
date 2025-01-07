
/** MACRO used to reference Periodic Timer overflow flag Set. 
 *  This is used by the application to have a semi-accurate 
 *  periodic task execution rate. 
 *  Strict adherence to time interval is not required.
 */
#define TIMER_FLAG_SET()                (TMR0_HasOverflowOccured())
/** MACRO used to reset the Periodic Timer overflow flag.
 *  This is used by the application to reload the semi-accurate
 *  periodic task execution.
 *  The rate allows for a (100%) drift prior to error
 *  Is susceptible to effect by extended BLE communication. 
 */
#define RESET_TIMER_INTERRUPT_FLAG      (PIR0bits.TMR0IF = 0)
/** MACRO used to configure the application used buffer sizes.
 *  This is used by the application for communication buffers.
 */
#define MAX_BUFFER_SIZE                 (80)

static char statusBuffer[MAX_BUFFER_SIZE];      /**< Status Buffer instance passed to RN487X drive used for Asynchronous Message Handling (see *asyncBuffer in rn487x.c) */
static char lightBlueSerial[MAX_BUFFER_SIZE];   /**< Message Buffer used for CDC Serial communication when connected. Terminated by \r, \n, MAX character Passes messages to BLE for transmisison. */
static uint8_t serialIndex;                     /**< Local index value for serial communication buffer. */


void runProtocol(void);