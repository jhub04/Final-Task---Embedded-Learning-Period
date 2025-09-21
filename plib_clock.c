#include "plib_clock.h"
#include "sam.h"

static void OSCCTRL_Initialize(void)
{
    /****************** XOSC1 Initialization **********************************/
    /* Configure External Oscillator (used as FDPLL0 reference) */
    OSCCTRL_REGS->OSCCTRL_XOSCCTRL[1] =
        OSCCTRL_XOSCCTRL_STARTUP(0U) |
        OSCCTRL_XOSCCTRL_IMULT(4U)   |
        OSCCTRL_XOSCCTRL_IPTAT(3U)   |
        OSCCTRL_XOSCCTRL_XTALEN_Msk  |
        OSCCTRL_XOSCCTRL_ENABLE_Msk;

    while((OSCCTRL_REGS->OSCCTRL_STATUS & OSCCTRL_STATUS_XOSCRDY1_Msk) != OSCCTRL_STATUS_XOSCRDY1_Msk)
    {
        /* Wait for XOSC1 ready */
    }
}

static void OSC32KCTRL_Initialize(void)
{
    /* RTC uses internal 32K source (unchanged) */
    OSC32KCTRL_REGS->OSC32KCTRL_RTCCTRL = OSC32KCTRL_RTCCTRL_RTCSEL(0U);
}

static void FDPLL0_Initialize(void)
{
    /****************** DPLL0 Initialization **********************************/
    /* Use XOSC1 as reference (REFCLK=3) with DIV=5 as in USART example */
    OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLCTRLB =
        OSCCTRL_DPLLCTRLB_FILTER(0U) |
        OSCCTRL_DPLLCTRLB_LTIME(0x0U)|
        OSCCTRL_DPLLCTRLB_REFCLK(3U) |
        OSCCTRL_DPLLCTRLB_DIV(5U);

    /* Multiply to achieve desired GCLK source (LDR=119, LDRFRAC=0) */
    OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLRATIO =
        OSCCTRL_DPLLRATIO_LDRFRAC(0U) | OSCCTRL_DPLLRATIO_LDR(119U);

    while((OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLSYNCBUSY & OSCCTRL_DPLLSYNCBUSY_DPLLRATIO_Msk) != 0U)
    {
        /* Wait for ratio sync */
    }

    /* Enable DPLL0 */
    OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLCTRLA = OSCCTRL_DPLLCTRLA_ENABLE_Msk;

    while((OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLSYNCBUSY & OSCCTRL_DPLLSYNCBUSY_ENABLE_Msk) != 0U)
    {
        /* Wait for enable sync */
    }

    while((OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLSTATUS &
          (OSCCTRL_DPLLSTATUS_LOCK_Msk | OSCCTRL_DPLLSTATUS_CLKRDY_Msk)) !=
          (OSCCTRL_DPLLSTATUS_LOCK_Msk | OSCCTRL_DPLLSTATUS_CLKRDY_Msk))
    {
        /* Wait until DPLL locked and clock ready */
    }
}

static void DFLL_Initialize(void)
{
    /* Not used; keep empty for compatibility */
}

static void GCLK0_Initialize(void)
{
    /* CPU clock division = 1 */
    MCLK_REGS->MCLK_CPUDIV = MCLK_CPUDIV_DIV(0x01U);

    while((MCLK_REGS->MCLK_INTFLAG & MCLK_INTFLAG_CKRDY_Msk) == 0U)
    {
        /* Wait for Main Clock ready */
    }

    /* Generator 0: DIV=1, SRC=7 (DPLL0), enable */
    GCLK_REGS->GCLK_GENCTRL[0] = GCLK_GENCTRL_DIV(1U) | GCLK_GENCTRL_SRC(7U) | GCLK_GENCTRL_GENEN_Msk;

    while((GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL_GCLK0) != 0U)
    {
        /* Wait for GCLK0 sync */
    }
}

static void GCLK1_Initialize(void)
{
    /* Generator 1: DIV=2, SRC=7 (DPLL0), enable */
    GCLK_REGS->GCLK_GENCTRL[1] = GCLK_GENCTRL_DIV(2U) | GCLK_GENCTRL_SRC(7U) | GCLK_GENCTRL_GENEN_Msk;

    while((GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL_GCLK1) != 0U)
    {
        /* Wait for GCLK1 sync */
    }
}

static void GCLK2_Initialize(void)
{
    /* Generator 2: DIV=48, SRC=6, enable (as in both originals) */
    GCLK_REGS->GCLK_GENCTRL[2] = GCLK_GENCTRL_DIV(48U) | GCLK_GENCTRL_SRC(6U) | GCLK_GENCTRL_GENEN_Msk;

    while((GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL_GCLK2) != 0U)
    {
        /* Wait for GCLK2 sync */
    }
}

void CLOCK_Initialize (void)
{
    /* Initialize oscillators */
    OSCCTRL_Initialize();
    OSC32KCTRL_Initialize();

    /* Clocks and PLLs */
    FDPLL0_Initialize();
    DFLL_Initialize();
    GCLK0_Initialize();
    GCLK1_Initialize();
    GCLK2_Initialize();

    /* --- Peripheral Channel Clocks --- */

    /* SERCOM2_CORE (USART): use Generator 1 (matches USART example) */
    GCLK_REGS->GCLK_PCHCTRL[23] = GCLK_PCHCTRL_GEN(0x1U) | GCLK_PCHCTRL_CHEN_Msk;
    while ((GCLK_REGS->GCLK_PCHCTRL[23] & GCLK_PCHCTRL_CHEN_Msk) == 0U)
    {
        /* Wait for SERCOM2_CORE clock channel enable */
    }

    /* TCC0/TCC1: use Generator 0 (matches TCC example) */
    GCLK_REGS->GCLK_PCHCTRL[25] = GCLK_PCHCTRL_GEN(0x0U) | GCLK_PCHCTRL_CHEN_Msk;
    while ((GCLK_REGS->GCLK_PCHCTRL[25] & GCLK_PCHCTRL_CHEN_Msk) == 0U)
    {
        /* Wait for TCC0/TCC1 clock channel enable */
    }

    /* --- Bridge Masks --- */
    /* AHB: keep enabled as in originals */
    MCLK_REGS->MCLK_AHBMASK  = 0x00FFFFFFU;

    /* APBA: from originals */
    MCLK_REGS->MCLK_APBAMASK = 0x000007FFU;

    /* APBB: OR of both originals (0x18856 | 0x18256 = 0x18A56) */
    MCLK_REGS->MCLK_APBBMASK = 0x18256 | 0x18A56;
}
