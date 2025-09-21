#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

/***************************************
 * Check PWM outputs on pins
 * Channel 0 PWMH - PC04
 * Channel 0 PWML - PD11
 * Channel 1 PWMH - PD08
 * Channel 1 PWML - PB17
 * Channel 2 PWMH - PB14
 * Channel 2 PWML - PC22
***************************************/

/* Duty cycle increment value */
#define DUTY_INCREMENT (10U)

/* Save PWM period */
static uint32_t period;

/* This function is called after TCC period event */
void TCC_PeriodEventHandler(uint32_t status, uintptr_t context)
{
    /* duty cycle values */
    static uint32_t duty0 = 0U;
    static uint32_t duty1 = 800U;
    static uint32_t duty2 = 1600U;

    TCC0_PWM24bitDutySet(TCC0_CHANNEL0, duty0);
    TCC0_PWM24bitDutySet(TCC0_CHANNEL1, duty1);
    TCC0_PWM24bitDutySet(TCC0_CHANNEL2, duty2);
    
    /* Increment duty cycle values */
    duty0 += DUTY_INCREMENT;
    duty1 += DUTY_INCREMENT;
    duty2 += DUTY_INCREMENT;
    
    if (duty0 > period)
        duty0 = 0U;
    if (duty1 > period)
        duty1 = 0U;
    if (duty2 > period)
        duty2 = 0U;

}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
        /* Register callback function for period event */
    TCC0_PWMCallbackRegister(TCC_PeriodEventHandler, (uintptr_t)NULL);
    
    /* Read the period */
    period = TCC0_PWM24bitPeriodGet();
    
    /* Start PWM*/
    TCC0_PWMStart();

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}