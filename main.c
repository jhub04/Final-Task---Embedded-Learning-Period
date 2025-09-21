#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>
#include "definitions.h"                // SYS function prototypes

#define RX_BUFFER_SIZE 10
#define LED_ON    LED_Clear
#define LED_OFF   LED_Set

char messageStart[] = "****  USART echo demo: Non-blocking Transfer with the interrupt  ****\r\n\
**** Type 10 characters. The received characters are echoed back, and the LED is toggled ****\r\n";
char receiveBuffer[RX_BUFFER_SIZE] = {0};
char echoBuffer[RX_BUFFER_SIZE+4] = {0};
char messageError[] = "**** USART error occurred ****\r\n";

static bool errorStatus = false;
static bool writeStatus = false;
static bool readStatus = false;


void APP_WriteCallback(uintptr_t context)
{
    writeStatus = true;
}

void APP_ReadCallback(uintptr_t context)
{
    if(SERCOM2_USART_ErrorGet() != USART_ERROR_NONE)
    {
        /* ErrorGet clears errors, set error flag to notify console */
        errorStatus = true;
    }
    else
    {
        readStatus = true;
    }
}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    /* Register callback functions and send start message */
    SERCOM2_USART_WriteCallbackRegister(APP_WriteCallback, 0);
    SERCOM2_USART_ReadCallbackRegister(APP_ReadCallback, 0);
    SERCOM2_USART_Write(&messageStart[0], sizeof(messageStart));

    while ( true )
    {
        if(errorStatus == true)
        {
            /* Send error message to console */
            errorStatus = false;
            SERCOM2_USART_Write(&messageError[0], sizeof(messageError));
        }
        else if(readStatus == true)
        {
            /* Echo back received buffer and Toggle LED */
            readStatus = false;

            echoBuffer[0] = '\n';
            echoBuffer[1] = '\r';
            memcpy(&echoBuffer[2], receiveBuffer,sizeof(receiveBuffer));
            echoBuffer[RX_BUFFER_SIZE+2] = '\n';
            echoBuffer[RX_BUFFER_SIZE+3] = '\r';

            SERCOM2_USART_Write(&echoBuffer[0], sizeof(echoBuffer));
            LED_Toggle();
        }
        else if(writeStatus == true)
        {
            /* Submit buffer to read user data */
            writeStatus = false;
            SERCOM2_USART_Read(&receiveBuffer[0], sizeof(receiveBuffer));
        }
        else
        {
            /* Repeat the loop */
            ;
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


