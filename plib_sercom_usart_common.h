#ifndef PLIB_SERCOM_USART_COMMON_H // Guards against multiple inclusion
#define PLIB_SERCOM_USART_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "sam.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

    /* Error status when no error has occurred */
#define USART_ERROR_NONE 0U

    /* Error status when parity error has occurred */
#define USART_ERROR_PARITY SERCOM_USART_INT_STATUS_PERR_Msk

    /* Error status when framing error has occurred */
#define USART_ERROR_FRAMING SERCOM_USART_INT_STATUS_FERR_Msk

    /* Error status when overrun error has occurred */
#define USART_ERROR_OVERRUN SERCOM_USART_INT_STATUS_BUFOVF_Msk


typedef uint16_t USART_ERROR;

typedef enum
{
        USART_DATA_5_BIT = SERCOM_USART_INT_CTRLB_CHSIZE_5_BIT,
    USART_DATA_6_BIT = SERCOM_USART_INT_CTRLB_CHSIZE_6_BIT,
    USART_DATA_7_BIT = SERCOM_USART_INT_CTRLB_CHSIZE_7_BIT,
    USART_DATA_8_BIT = SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT,
    USART_DATA_9_BIT = SERCOM_USART_INT_CTRLB_CHSIZE_9_BIT,


    /* Force the compiler to reserve 32-bit memory for each enum */
    USART_DATA_INVALID = 0xFFFFFFFFU

} USART_DATA;



typedef enum
{
    USART_PARITY_EVEN = SERCOM_USART_INT_CTRLB_PMODE_EVEN,

    USART_PARITY_ODD = SERCOM_USART_INT_CTRLB_PMODE_ODD,

    /* This enum is defined to set frame format only
     * This value won't be written to register
     */
    USART_PARITY_NONE = 0x2,

    /* Force the compiler to reserve 32-bit memory for each enum */
    USART_PARITY_INVALID = 0xFFFFFFFFU

} USART_PARITY;


typedef enum
{
        USART_STOP_0_BIT = SERCOM_USART_INT_CTRLB_SBMODE_1_BIT,
    USART_STOP_1_BIT = SERCOM_USART_INT_CTRLB_SBMODE_2_BIT,


    /* Force the compiler to reserve 32-bit memory for each enum */
    USART_STOP_INVALID = 0xFFFFFFFFU

} USART_STOP;



typedef enum
{
    USART_LIN_MASTER_CMD_NONE = SERCOM_USART_INT_CTRLB_LINCMD_NONE,

    USART_LIN_MASTER_CMD_SOFTWARE_CONTROLLED = SERCOM_USART_INT_CTRLB_LINCMD_SOFTWARE_CONTROL_TRANSMIT_CMD,

    USART_LIN_MASTER_CMD_AUTO_TRANSMIT = SERCOM_USART_INT_CTRLB_LINCMD_AUTO_TRANSMIT_CMD

} USART_LIN_MASTER_CMD;



typedef struct
{
    uint32_t baudRate;

    USART_PARITY parity;

    USART_DATA dataWidth;

    USART_STOP stopBits;

} USART_SERIAL_SETUP;



typedef void (*SERCOM_USART_CALLBACK)( uintptr_t context );



typedef struct
{
    void *                              txBuffer;

    size_t                              txSize;

    size_t                              txProcessedSize;

    SERCOM_USART_CALLBACK               txCallback;

    uintptr_t                           txContext;

    bool                                txBusyStatus;

    void *                              rxBuffer;

    size_t                              rxSize;

    size_t                              rxProcessedSize;

    SERCOM_USART_CALLBACK               rxCallback;

    uintptr_t                           rxContext;

    bool                                rxBusyStatus;

    USART_ERROR                         errorStatus;

} SERCOM_USART_OBJECT;


typedef enum
{
    /* Threshold number of bytes are available in the receive ring buffer */
    SERCOM_USART_EVENT_READ_THRESHOLD_REACHED = 0,

    /* Receive ring buffer is full. Application must read the data out to avoid missing data on the next RX interrupt. */
    SERCOM_USART_EVENT_READ_BUFFER_FULL,

    /* USART error. Application must call the SERCOMx_USART_ErrorGet API to get the type of error and clear the error. */
    SERCOM_USART_EVENT_READ_ERROR,

    /* Threshold number of free space is available in the transmit ring buffer */
    SERCOM_USART_EVENT_WRITE_THRESHOLD_REACHED,

    /* Recevie break signal is detected */
    SERCOM_USART_EVENT_BREAK_SIGNAL_DETECTED,
}SERCOM_USART_EVENT;



typedef void (*SERCOM_USART_RING_BUFFER_CALLBACK)(SERCOM_USART_EVENT event, uintptr_t context );



typedef struct
{
    SERCOM_USART_RING_BUFFER_CALLBACK                   wrCallback;

    uintptr_t                                           wrContext;

    uint32_t                                            wrInIndex;

    uint32_t                                            wrOutIndex;

    uint32_t                                            wrBufferSize;

    bool                                                isWrNotificationEnabled;

    uint32_t                                            wrThreshold;

    bool                                                isWrNotifyPersistently;

    SERCOM_USART_RING_BUFFER_CALLBACK                   rdCallback;

    uintptr_t                                           rdContext;

    uint32_t                                            rdInIndex;

    uint32_t                                            rdOutIndex;

    uint32_t                                            rdBufferSize;

    bool                                                isRdNotificationEnabled;

    uint32_t                                            rdThreshold;

    bool                                                isRdNotifyPersistently;

    USART_ERROR                                         errorStatus;

} SERCOM_USART_RING_BUFFER_OBJECT;

// *****************************************************************************
// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif //PLIB_SERCOM_USART_COMMON_H
