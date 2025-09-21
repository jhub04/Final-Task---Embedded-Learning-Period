#ifndef PLIB_SERCOM2_USART_H // Guards against multiple inclusion
#define PLIB_SERCOM2_USART_H

#include "plib_sercom_usart_common.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

void SERCOM2_USART_Initialize( void );

bool SERCOM2_USART_SerialSetup( USART_SERIAL_SETUP * serialSetup, uint32_t clkFrequency );

void SERCOM2_USART_Enable( void );

void SERCOM2_USART_Disable( void );

void SERCOM2_USART_TransmitterEnable( void );

void SERCOM2_USART_TransmitterDisable( void );

bool SERCOM2_USART_Write( void *buffer, const size_t size );

bool SERCOM2_USART_TransmitComplete( void );


bool SERCOM2_USART_WriteIsBusy( void );

size_t SERCOM2_USART_WriteCountGet( void );

void SERCOM2_USART_WriteCallbackRegister( SERCOM_USART_CALLBACK callback, uintptr_t context );


void SERCOM2_USART_ReceiverEnable( void );

void SERCOM2_USART_ReceiverDisable( void );

bool SERCOM2_USART_Read( void *buffer, const size_t size );

bool SERCOM2_USART_ReadIsBusy( void );

size_t SERCOM2_USART_ReadCountGet( void );

bool SERCOM2_USART_ReadAbort(void);

void SERCOM2_USART_ReadCallbackRegister( SERCOM_USART_CALLBACK callback, uintptr_t context );

USART_ERROR SERCOM2_USART_ErrorGet( void );

uint32_t SERCOM2_USART_FrequencyGet( void );


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif //PLIB_SERCOM2_USART_H
