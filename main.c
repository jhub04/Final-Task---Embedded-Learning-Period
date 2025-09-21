#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>
#include "definitions.h"                // SYS function prototypes
#include <stdio.h>


#define RX_BUFFER_SIZE 2
#define LED_ON LED_Clear
#define LED_OFF LED_Set

/* Duty cycle increment value */
#define DUTY_INCREMENT (10U)

char messageStart[] = "**** USART?PWM demo (SAME54 + SERCOM2 + TCC0) ****\r\n"
"Type a number 0..100 to set duty %% on TCC0 CH0.\r\n"
"Example: 37<Enter>  -> sets duty to 37%%\r\n\r\n";

char receiveBuffer[RX_BUFFER_SIZE] = {0};
char echoBuffer[RX_BUFFER_SIZE+4] = {0};
char messageError[] = "**** USART error occurred ****\r\n";

static bool errorStatus = false;
static bool writeStatus = false;
static bool readStatus = false;


/* Save PWM period */
static uint32_t period;


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


/* Set percentage of channel 0*/
void set_tcc_ch0_percentage(int percentage)  
{
    uint32_t duty = (period * percentage) / 100U;
    TCC0_PWM24bitDutySet(TCC0_CHANNEL0, duty);
}

int extract_percentage(const char* receiveBuffer)
{
    int pct = atoi(receiveBuffer);
   
    if (pct < 0) pct = 0;
    if (pct > 100) pct = 100;
    
    return pct;
    
}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    /* USART Setup */
    SERCOM2_USART_WriteCallbackRegister(APP_WriteCallback, 0);
    SERCOM2_USART_ReadCallbackRegister(APP_ReadCallback, 0);
    SERCOM2_USART_Write(&messageStart[0], sizeof(messageStart));
    
    /* TCC Setup */
    period = TCC0_PWM24bitPeriodGet();
    TCC0_PWMStart();
    set_tcc_ch0_percentage(0);

    while ( true )
    {
        if (errorStatus == true) {
            /* Send error message to console */
            errorStatus = false;
            SERCOM2_USART_Write(&messageError[0], sizeof(messageError));
        }
        else if (readStatus == true) 
        {
            readStatus = false;
            
            echoBuffer[0] = '\n';
            echoBuffer[1] = '\r';
            
            memcpy(&echoBuffer[2], receiveBuffer,sizeof(receiveBuffer));
            echoBuffer[RX_BUFFER_SIZE+2] = '\n';
            echoBuffer[RX_BUFFER_SIZE+3] = '\r';
            
            
           
            int percentage = extract_percentage(receiveBuffer);
            set_tcc_ch0_percentage(percentage);
            
            snprintf(&echoBuffer[2], sizeof(echoBuffer), "%d" ,percentage);
            
            SERCOM2_USART_Write(&echoBuffer[0], sizeof(echoBuffer));
            LED_Toggle();
            
            
            
        } 
        else if (writeStatus == true) 
        {
            writeStatus = false;
            SERCOM2_USART_Read(&receiveBuffer[0], sizeof(receiveBuffer));
        } 
        else {
            SYS_Tasks ( );
        }
      
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}