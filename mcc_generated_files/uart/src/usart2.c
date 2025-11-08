/**
 * USART2 Generated Driver API Header File
 * 
 * @file usart2.c
 * 
 * @ingroup usart2
 * 
 * @brief This is the generated driver implementation file for the USART2 driver using the  Universal Synchronous and Asynchronous serial Receiver and Transmitter (USART) module. 
 *
 * @version USART2 Driver Version 2.1.2
*/

/*
© [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

/**
  Section: Included Files
*/

#include "../usart2.h"

/**
  Section: Macro Declarations
*/

#define USART2_TX_BUFFER_SIZE (256U) //buffer size should be 2^n
#define USART2_TX_BUFFER_MASK (USART2_TX_BUFFER_SIZE - 1U) 

#define USART2_RX_BUFFER_SIZE (256U) //buffer size should be 2^n
#define USART2_RX_BUFFER_MASK (USART2_RX_BUFFER_SIZE - 1U)



/**
  Section: Driver Interface
 */

const uart_drv_interface_t UART2 = {
    .Initialize = &USART2_Initialize,
    .Deinitialize = &USART2_Deinitialize,
    .Read = &USART2_Read,
    .Write = &USART2_Write,
    .IsRxReady = &USART2_IsRxReady,
    .IsTxReady = &USART2_IsTxReady,
    .IsTxDone = &USART2_IsTxDone,
    .TransmitEnable = &USART2_TransmitEnable,
    .TransmitDisable = &USART2_TransmitDisable,
    .AutoBaudSet = &USART2_AutoBaudSet,
    .AutoBaudQuery = &USART2_AutoBaudQuery,
    .BRGCountSet = NULL,
    .BRGCountGet = NULL,
    .BaudRateSet = NULL,
    .BaudRateGet = NULL,
    .AutoBaudEventEnableGet = NULL,
    .ErrorGet = &USART2_ErrorGet,
    .TxCompleteCallbackRegister = &USART2_TxCompleteCallbackRegister,
    .RxCompleteCallbackRegister = &USART2_RxCompleteCallbackRegister,
    .TxCollisionCallbackRegister = NULL,
    .FramingErrorCallbackRegister = &USART2_FramingErrorCallbackRegister,
    .OverrunErrorCallbackRegister = &USART2_OverrunErrorCallbackRegister,
    .ParityErrorCallbackRegister = &USART2_ParityErrorCallbackRegister,
    .EventCallbackRegister = NULL,
};

/**
  Section: USART2 variables
*/
static volatile uint16_t usart2TxHead = 0;
static volatile uint16_t usart2TxTail = 0;
static volatile uint16_t usart2TxBufferRemaining;
static volatile uint8_t usart2TxBuffer[USART2_TX_BUFFER_SIZE];
static volatile bool usart2IsTxComplete;
static volatile uint16_t usart2RxHead = 0;
static volatile uint16_t usart2RxTail = 0;
static volatile uint16_t usart2RxCount;
static volatile uint8_t usart2RxBuffer[USART2_RX_BUFFER_SIZE];
/**
 * @misradeviation{@advisory,19.2}
 * The UART error status necessitates checking the bit field and accessing the status within the group byte therefore the use of a union is essential.
 */
  /* cppcheck-suppress misra-c2012-19.2 */
static volatile usart2_status_t usart2RxStatusBuffer[USART2_RX_BUFFER_SIZE];
 /* cppcheck-suppress misra-c2012-19.2 */
static volatile usart2_status_t usart2RxLastError;

/**
  Section: USART2 APIs
*/
static void (*USART2_FramingErrorHandler)(void);
static void (*USART2_OverrunErrorHandler)(void);
static void (*USART2_ParityErrorHandler)(void);
void (*USART2_TxInterruptHandler)(void);
/* cppcheck-suppress misra-c2012-8.9 */
static void (*USART2_TxCompleteInterruptHandler)(void) = NULL;
void (*USART2_RxInterruptHandler)(void);
static void (*USART2_RxCompleteInterruptHandler)(void) = NULL;

static void USART2_DefaultFramingErrorCallback(void);
static void USART2_DefaultOverrunErrorCallback(void);
static void USART2_DefaultParityErrorCallback(void);
void USART2_TransmitISR (void);
void USART2_ReceiveISR(void);



/**
  Section: USART2  APIs
*/

void USART2_Initialize(void)
{
    USART2_RxInterruptHandler = USART2_ReceiveISR;  
    USART2_TxInterruptHandler = USART2_TransmitISR;

    // Set the USART2 module to the options selected in the user interface.

    //BAUD 6666; 
    USART2.BAUD = (uint16_t)USART2_BAUD_RATE(9600UL);
	
    // ABEIE disabled; DREIE disabled; LBME disabled; RS485 DISABLE; RXCIE enabled; RXSIE enabled; TXCIE enabled; 
    USART2.CTRLA = 0xD0;
	
    // MPCM disabled; ODME disabled; RXEN enabled; RXMODE NORMAL; SFDEN disabled; TXEN enabled; 
    USART2.CTRLB = 0xC0;
	
    // CMODE Asynchronous Mode; UCPHA enabled; UDORD disabled; CHSIZE Character size: 8 bit; PMODE No Parity; SBMODE 1 stop bit; 
    USART2.CTRLC = 0x3;
	
    //DBGRUN disabled; 
    USART2.DBGCTRL = 0x0;
	
    //IREI disabled; 
    USART2.EVCTRL = 0x0;
	
    //RXPL 0x0; 
    USART2.RXPLCTRL = 0x0;
	
    //TXPL 0x0; 
    USART2.TXPLCTRL = 0x0;
	
    USART2_FramingErrorCallbackRegister(USART2_DefaultFramingErrorCallback);
    USART2_OverrunErrorCallbackRegister(USART2_DefaultOverrunErrorCallback);
    USART2_ParityErrorCallbackRegister(USART2_DefaultParityErrorCallback);
    usart2RxLastError.status = 0;  
    usart2TxHead = 0;
    usart2TxTail = 0;
    usart2TxBufferRemaining = sizeof(usart2TxBuffer);
    usart2IsTxComplete = true;
    usart2RxHead = 0;
    usart2RxTail = 0;
    usart2RxCount = 0;
    USART2.CTRLA |= USART_RXCIE_bm; 

}

void USART2_Deinitialize(void)
{
    USART2.CTRLA &= ~(USART_RXCIE_bm);    
    USART2.CTRLA &= ~(USART_DREIE_bm);  
    USART2.BAUD = 0x00;	
    USART2.CTRLA = 0x00;	
    USART2.CTRLB = 0x00;	
    USART2.CTRLC = 0x00;	
    USART2.DBGCTRL = 0x00;	
    USART2.EVCTRL = 0x00;	
    USART2.RXPLCTRL = 0x00;	
    USART2.TXPLCTRL = 0x00;	
}

void USART2_Enable(void)
{
    USART2.CTRLB |= USART_RXEN_bm | USART_TXEN_bm; 
}

void USART2_Disable(void)
{
    USART2.CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm); 
}

void USART2_TransmitEnable(void)
{
    USART2.CTRLB |= USART_TXEN_bm; 
}

void USART2_TransmitDisable(void)
{
    USART2.CTRLB &= ~(USART_TXEN_bm); 
}

void USART2_ReceiveEnable(void)
{
    USART2.CTRLB |= USART_RXEN_bm ; 
}

void USART2_ReceiveDisable(void)
{
    USART2.CTRLB &= ~(USART_RXEN_bm); 
}

void USART2_AutoBaudSet(bool enable)
{
    if(enable)
    {
        USART2.CTRLB |= USART_RXMODE_gm & (0x02 << USART_RXMODE_gp); 
        USART2.STATUS |= USART_WFB_bm ; 
    }
    else
    {
       USART2.CTRLB &= ~(USART_RXMODE_gm); 
       USART2.STATUS &= ~(USART_BDF_bm);  
    }
}

bool USART2_AutoBaudQuery(void)
{
     return (bool)(USART2.STATUS & USART_BDF_bm) ; 
}

bool USART2_IsAutoBaudDetectError(void)
{
     return (bool)(USART2.STATUS & USART_ISFIF_bm) ; 
}

void USART2_AutoBaudDetectErrorReset(void)
{
    USART2.STATUS |= USART_ISFIF_bm ;
	USART2_AutoBaudSet(false);
    USART2_ReceiveDisable();
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    USART2_ReceiveEnable();
    USART2_AutoBaudSet(true);
}

void USART2_TransmitInterruptEnable(void)
{
    USART2.CTRLA |= USART_DREIE_bm ; 
}

void USART2_TransmitInterruptDisable(void)
{ 
    USART2.CTRLA &= ~(USART_DREIE_bm); 
}

void USART2_ReceiveInterruptEnable(void)
{
    USART2.CTRLA |= USART_RXCIE_bm ; 
}
void USART2_ReceiveInterruptDisable(void)
{
    USART2.CTRLA &= ~(USART_RXCIE_bm); 
}

bool USART2_IsRxReady(void)
{
    return (usart2RxCount ? true : false);
}

bool USART2_IsTxReady(void)
{
    return (usart2TxBufferRemaining ? true : false);
}

bool USART2_IsTxDone(void)
{
    bool usart2TxCompleteStatus = false;
    usart2TxCompleteStatus = usart2IsTxComplete;
    usart2IsTxComplete = false;
    return usart2TxCompleteStatus;
}

size_t USART2_ErrorGet(void)
{
    usart2RxLastError.status = usart2RxStatusBuffer[usart2RxTail & USART2_RX_BUFFER_MASK].status;
    return usart2RxLastError.status;
}

uint8_t USART2_Read(void)
{
    uint8_t readValue  = 0;
    uint16_t tempRxTail;
    
    readValue = usart2RxBuffer[usart2RxTail];
    tempRxTail = (usart2RxTail + 1U) & USART2_RX_BUFFER_MASK; // Buffer size of RX should be in the 2^n  
    usart2RxTail = tempRxTail;
    USART2.CTRLA &= ~(USART_RXCIE_bm); 
    if(0U != usart2RxCount)
    {
        usart2RxCount--;
    }
    USART2.CTRLA |= USART_RXCIE_bm; 


    return readValue;
}

/* Interrupt service routine for RX complete */
/* cppcheck-suppress misra-c2012-2.7 */
/* cppcheck-suppress misra-c2012-8.4 */
ISR(USART2_RXC_vect)
/* cppcheck-suppress misra-c2012-5.5 */
{
    USART2_RxInterruptHandler();
}

void USART2_ReceiveISR(void)
{
    uint8_t regValue;
    uint16_t tempRxHead;
    
    usart2RxStatusBuffer[usart2RxHead].status = 0;

    if(USART_FERR_bm == (USART2.RXDATAH & USART_FERR_bm))
    {
        usart2RxStatusBuffer[usart2RxHead].ferr = 1;
        if(NULL != USART2_FramingErrorHandler)
        {
            USART2_FramingErrorHandler();
        } 
    }
    if(USART_PERR_bm == (USART2.RXDATAH & USART_PERR_bm))
    {
        usart2RxLastError.perr = 1;
        if(NULL != USART2_ParityErrorHandler)
        {
            USART2_ParityErrorHandler();
        }  
    }
    if(USART_BUFOVF_bm == (USART2.RXDATAH & USART_BUFOVF_bm))
    {
        usart2RxStatusBuffer[usart2RxHead].oerr = 1;
        if(NULL != USART2_OverrunErrorHandler)
        {
            USART2_OverrunErrorHandler();
        }   
    }    
    
    regValue = USART2.RXDATAL;
    
    tempRxHead = (usart2RxHead + 1U) & USART2_RX_BUFFER_MASK;// Buffer size of RX should be in the 2^n
    if (tempRxHead == usart2RxTail) {
		// ERROR! Receive buffer overflow 
	} 
    else
    {
        // Store received data in buffer 
		usart2RxBuffer[usart2RxHead] = regValue;
		usart2RxHead = tempRxHead;

		usart2RxCount++;
	}
    if (NULL != USART2_RxCompleteInterruptHandler)
    {
        (*USART2_RxCompleteInterruptHandler)();
    }
    
    else {
        // Do Nothing. Added for MISRA C Compliant.
    }
}

void USART2_Write(uint8_t txData)
{
    uint16_t tempTxHead;
    
    if(0U < usart2TxBufferRemaining) // check if at least one byte place is available in TX buffer
    {
       usart2TxBuffer[usart2TxHead] = txData;
       tempTxHead = (usart2TxHead + 1U) & USART2_TX_BUFFER_MASK;// Buffer size of TX should be in the 2^n
       
       usart2TxHead = tempTxHead;
       USART2.CTRLA &= ~(USART_DREIE_bm);  //Critical value decrement
       usart2TxBufferRemaining--;  // one less byte remaining in TX buffer
    }
    else
    {
        //overflow condition; TX buffer is full
    }

    USART2.CTRLA |= USART_DREIE_bm;  
}

/* Interrupt service routine for Data Register Empty */
/* cppcheck-suppress misra-c2012-2.7 */
/* cppcheck-suppress misra-c2012-8.4 */
ISR(USART2_DRE_vect)
/* cppcheck-suppress misra-c2012-5.5 */
{
    USART2_TxInterruptHandler();
}

/* Interrupt service routine for shift register and data register empty */
/* cppcheck-suppress misra-c2012-2.7 */
/* cppcheck-suppress misra-c2012-8.4 */
ISR(USART2_TXC_vect)
/* cppcheck-suppress misra-c2012-5.5 */
{
    usart2IsTxComplete = (bool)(USART2.STATUS & USART_TXCIF_bm);

    if (NULL != USART2_TxCompleteInterruptHandler)
    {
        (*USART2_TxCompleteInterruptHandler)();
    }

    USART2.STATUS |= USART_TXCIF_bm;
}

void USART2_TransmitISR(void)
{
    uint16_t tempTxTail;

    // use this default transmit interrupt handler code
    if(sizeof(usart2TxBuffer) > usart2TxBufferRemaining) // check if all data is transmitted
    {
       USART2.TXDATAL = usart2TxBuffer[usart2TxTail];

       tempTxTail = (usart2TxTail + 1U) & USART2_TX_BUFFER_MASK;// Buffer size of TX should be in the 2^n
       
       usart2TxTail = tempTxTail;

       usart2TxBufferRemaining++; // one byte sent, so 1 more byte place is available in TX buffer
    }
    else
    {
        USART2.CTRLA &= ~(USART_DREIE_bm); 
    }
}

static void USART2_DefaultFramingErrorCallback(void)
{
    
}

static void USART2_DefaultOverrunErrorCallback(void)
{
    
}

static void USART2_DefaultParityErrorCallback(void)
{
    
}

void USART2_FramingErrorCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
        USART2_FramingErrorHandler = callbackHandler;
    }
}

void USART2_OverrunErrorCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
        USART2_OverrunErrorHandler = callbackHandler;
    }    
}

void USART2_ParityErrorCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
        USART2_ParityErrorHandler = callbackHandler;
    } 
}

void USART2_RxCompleteCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
       USART2_RxCompleteInterruptHandler = callbackHandler; 
    }   
}

void USART2_TxCompleteCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
       USART2_TxCompleteInterruptHandler = callbackHandler;
    }   
}


