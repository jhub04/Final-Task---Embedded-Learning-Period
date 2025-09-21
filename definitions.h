#ifndef DEFINITIONS_H
#define DEFINITIONS_H


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "plib_tcc0.h"
#include "plib_port.h"
#include "plib_clock.h"
#include "plib_nvic.h"
#include "plib_sercom2_usart.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

/* Device Information */
#define DEVICE_NAME          "ATSAME54P20A"
#define DEVICE_ARCH          "CORTEX-M4"
#define DEVICE_FAMILY        "SAME"
#define DEVICE_SERIES        "SAME54"

/* CPU clock frequency */
#define CPU_CLOCK_FREQUENCY 120000000U



void SYS_Initialize( void *data );

/* Nullify SYS_Tasks() if only PLIBs are used. */
#define     SYS_Tasks()

// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************




//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* DEFINITIONS_H */
/*******************************************************************************
 End of File
*/

