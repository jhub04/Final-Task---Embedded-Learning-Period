#include "interrupts.h"
#include "plib_tcc0.h"


/* Object to hold callback function and context */
volatile static TCC_CALLBACK_OBJECT TCC0_CallbackObj;

/* Initialize TCC module */
void TCC0_PWMInitialize(void)
{
    /* Reset TCC */
    TCC0_REGS->TCC_CTRLA = TCC_CTRLA_SWRST_Msk;
    while((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_SWRST_Msk) == TCC_SYNCBUSY_SWRST_Msk)
    {
        /* Wait for sync */
    }
    /* Clock prescaler */
    TCC0_REGS->TCC_CTRLA = TCC_CTRLA_PRESCALER_DIV1 
                            | TCC_CTRLA_PRESCSYNC_PRESC ;
    TCC0_REGS->TCC_WEXCTRL = TCC_WEXCTRL_OTMX(0UL);
    /* Dead time configurations */
    TCC0_REGS->TCC_WEXCTRL |= TCC_WEXCTRL_DTIEN0_Msk | TCC_WEXCTRL_DTIEN1_Msk | TCC_WEXCTRL_DTIEN2_Msk | TCC_WEXCTRL_DTIEN3_Msk
 	 	 | TCC_WEXCTRL_DTLS(128UL) | TCC_WEXCTRL_DTHS(128UL);

    TCC0_REGS->TCC_WAVE = TCC_WAVE_WAVEGEN_DSBOTTOM;


    /* Configure duty cycle values */
    TCC0_REGS->TCC_CC[0] = 0U;
    TCC0_REGS->TCC_CC[1] = 0U;
    TCC0_REGS->TCC_CC[2] = 0U;
    TCC0_REGS->TCC_CC[3] = 0U;
    TCC0_REGS->TCC_CC[4] = 0U;
    TCC0_REGS->TCC_CC[5] = 0U;
    TCC0_REGS->TCC_PER = 5999U;


    TCC0_REGS->TCC_INTENSET = TCC_INTENSET_OVF_Msk;

    while (TCC0_REGS->TCC_SYNCBUSY != 0U)
    {
        /* Wait for sync */
    }
}


/* Start the PWM generation */
void TCC0_PWMStart(void)
{
    TCC0_REGS->TCC_CTRLA |= TCC_CTRLA_ENABLE_Msk;
    while((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk) == TCC_SYNCBUSY_ENABLE_Msk)
    {
        /* Wait for sync */
    }
}

/* Stop the PWM generation */
void TCC0_PWMStop (void)
{
    TCC0_REGS->TCC_CTRLA &= ~TCC_CTRLA_ENABLE_Msk;
    while((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk) == TCC_SYNCBUSY_ENABLE_Msk)
    {
        /* Wait for sync */
    }
}

/* Configure PWM period */
bool TCC0_PWM24bitPeriodSet (uint32_t period)
{
    bool status = false;
    if ((TCC0_REGS->TCC_STATUS & (TCC_STATUS_PERBUFV_Msk)) == 0U)
    {
        TCC0_REGS->TCC_PERBUF = period & 0xFFFFFFU;
        status = true;
    }    
    return status;
}


/* Read TCC period */
uint32_t TCC0_PWM24bitPeriodGet (void)
{
    while ((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_PER_Msk) == TCC_SYNCBUSY_PER_Msk)
    {
        /* Wait for sync */
    }
    return (TCC0_REGS->TCC_PER & 0xFFFFFFU);
}

/* Configure dead time */
void TCC0_PWMDeadTimeSet (uint8_t deadtime_high, uint8_t deadtime_low)
{
    TCC0_REGS->TCC_WEXCTRL &= ~(TCC_WEXCTRL_DTHS_Msk | TCC_WEXCTRL_DTLS_Msk);
    TCC0_REGS->TCC_WEXCTRL |= TCC_WEXCTRL_DTHS((uint32_t)deadtime_high) | TCC_WEXCTRL_DTLS((uint32_t)deadtime_low);
}

bool TCC0_PWMPatternSet(uint8_t pattern_enable, uint8_t pattern_output)
{
    bool status = false;
    if ((TCC0_REGS->TCC_STATUS & (TCC_STATUS_PATTBUFV_Msk)) == 0U)
    {
        TCC0_REGS->TCC_PATTBUF = (uint16_t)(pattern_enable | ((uint32_t)pattern_output << 8U));
        status = true;
    }   
    return status; 
}



/* Get the current counter value */
uint32_t TCC0_PWM24bitCounterGet( void )
{
    /* Write command to force COUNT register read synchronization */
    TCC0_REGS->TCC_CTRLBSET |= (uint8_t)TCC_CTRLBSET_CMD_READSYNC;

    while((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_CTRLB_Msk) == TCC_SYNCBUSY_CTRLB_Msk)
    {
        /* Wait for Write Synchronization */
    }

    while((TCC0_REGS->TCC_CTRLBSET & TCC_CTRLBSET_CMD_Msk) != 0U)
    {
        /* Wait for CMD to become zero */
    }

    /* Read current count value */
    return TCC0_REGS->TCC_COUNT;
}

/* Set the counter*/
void TCC0_PWM24bitCounterSet (uint32_t countVal)
{
    TCC0_REGS->TCC_COUNT = countVal & 0xFFFFFFU;
    while ((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_COUNT_Msk) == TCC_SYNCBUSY_COUNT_Msk)
    {
        /* Wait for sync */
    }
}

/* Enable forced synchronous update */
void TCC0_PWMForceUpdate(void)
{
    TCC0_REGS->TCC_CTRLBSET |= (uint8_t)TCC_CTRLBCLR_CMD_UPDATE;
    while ((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_CTRLB_Msk) == TCC_SYNCBUSY_CTRLB_Msk)
    {
        /* Wait for sync */
    }
}

/* Enable the period interrupt - overflow or underflow interrupt */
void TCC0_PWMPeriodInterruptEnable(void)
{
    TCC0_REGS->TCC_INTENSET = TCC_INTENSET_OVF_Msk;
}

/* Disable the period interrupt - overflow or underflow interrupt */
void TCC0_PWMPeriodInterruptDisable(void)
{
    TCC0_REGS->TCC_INTENCLR = TCC_INTENCLR_OVF_Msk;
}

 /* Register callback function */
void TCC0_PWMCallbackRegister(TCC_CALLBACK callback, uintptr_t context)
{
    TCC0_CallbackObj.callback_fn = callback;
    TCC0_CallbackObj.context = context;
}

/* Interrupt Handler */
void __attribute__((used)) TCC0_OTHER_Handler(void)
{
    uint32_t status;
    /* Additional local variable to prevent MISRA C violations (Rule 13.x) */
    uintptr_t context;
    context = TCC0_CallbackObj.context;            
    status = (TCC0_REGS->TCC_INTFLAG & 0xFFFFU);
    /* Clear interrupt flags */
    TCC0_REGS->TCC_INTFLAG = 0xFFFFU;
    (void)TCC0_REGS->TCC_INTFLAG;
    if (TCC0_CallbackObj.callback_fn != NULL)
    {
        TCC0_CallbackObj.callback_fn(status, context);
    }

}