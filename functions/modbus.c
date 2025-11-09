/* MODBUS */


#include "../mcc_generated_files/uart/usart1.h"
#include "../mcc_generated_files/timer/tcb0.h"
#include "system_registers.h"
#include "modbus.h"
#include "definitions.h"
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>

//UART
/* ===========================================
        UART in USE :: UART2
 *  ;; change -> uart identifier
   ===========================================*/

// Modbus slave address
#define SLAVE_ADDRESS 2

// Function codes
#define FC_READ_HOLDING_REGISTERS 0x03
#define FC_READ_INPUT_REGISTER 0x04
#define FC_WRITE_MULTIPLE_REGISTERS 0x10

// Buffer sizes
#define MAX_FRAME_SIZE 256

// Modbus frame buffer
static uint8_t rx_buffer[MAX_FRAME_SIZE];
static uint8_t tx_buffer[MAX_FRAME_SIZE];
static uint8_t rx_index = 0;
static volatile bool frame_complete = false;

void MB_Init(void){
    UART1_RxCompleteCallbackRegister(modbus_receive);
//    UART1_TxCompleteCallbackRegister(modbus_timer_expired);

    TCB0_CaptureCallbackRegister(modbus_timer_expired);
    RS485_RX_ENABLE();
}

// RS485 RX/TX Select
void RS485_TX_ENABLE(void){
    IO_PC3_SetHigh();
}
void RS485_RX_ENABLE(void){
    IO_PC3_SetLow();
}

// CRC16
uint16_t modbus_crc16(const uint8_t *data, uint8_t length) {
    uint16_t crc = 0xFFFF;
    for (uint8_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001; // Modbus CRC polynomial
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

// Process Modbus frame with system registers attached
void modbus_process(void) {
    if (!frame_complete) return; // Wait for timer to signal complete frame
    if (rx_index < 8) { // Minimum for FC 03: addr (1), func (1), start addr (2), num regs (2), CRC (2)
        rx_index = 0;
        frame_complete = false;
        return;
    }
 
    // Validate address and CRC
    uint16_t crc = modbus_crc16(rx_buffer, rx_index - 2);
    
    if (rx_buffer[0] != SLAVE_ADDRESS || 
        rx_buffer[rx_index - 2] != (crc & 0xFF) || 
        rx_buffer[rx_index - 1] != (crc >> 8)) {
        rx_index = 0;
        frame_complete = false;
        ERROR_LED_TOGGLE();   
        return;
    }
  
    /*=====================  
     * MB Function CODES 
      =====================*/
    switch(rx_buffer[1]){
        case FC_READ_INPUT_REGISTER: {
//            ERROR_LED_SET();

            uint16_t start_addr = (rx_buffer[2] << 8) | rx_buffer[3];
            uint16_t num_regs = (rx_buffer[4] << 8) | rx_buffer[5];

            // Validate request
            if (start_addr + num_regs <= MODBUS_REG_COUNT) {
                // Build response: addr, func, byte count, data, CRC
                tx_buffer[0] = SLAVE_ADDRESS;
                tx_buffer[1] = FC_READ_INPUT_REGISTER;
                tx_buffer[2] = num_regs * 2; // Byte count
                for (uint16_t i = 0; i < num_regs; i++) {
                    tx_buffer[3 + i * 2] = sys_regs[start_addr + i] >> 8; // High byte
                    tx_buffer[4 + i * 2] = sys_regs[start_addr + i] & 0xFF; // Low byte
                } 
                crc = modbus_crc16(tx_buffer, 3 + num_regs * 2);
                tx_buffer[3 + num_regs * 2] = crc & 0xFF;
                tx_buffer[4 + num_regs * 2] = crc >> 8;

                // Send response
//                TX1_LED_SET();
                RS485_TX_ENABLE();
                _delay_us(100);
                for (uint8_t i = 0; i < 5 + num_regs * 2; i++) {
                    while (!UART1_IsTxReady()); //UART1_IsTxReady
                    UART1_Write(tx_buffer[i]);
                }
                while (!UART1_IsTxDone()); //UART1_IsTxDone
                _delay_us(100);
                RS485_RX_ENABLE();
                TX1_LED_nSET();
            }   
            break;
        }
        case FC_READ_HOLDING_REGISTERS: {

            uint16_t start_addr = (rx_buffer[2] << 8) | rx_buffer[3];
            uint16_t num_regs = (rx_buffer[4] << 8) | rx_buffer[5];

            // Validate request
            if (start_addr + num_regs <= MODBUS_REG_COUNT) {
                
                // Build response: addr, func, byte count, data, CRC
                tx_buffer[0] = SLAVE_ADDRESS;
                tx_buffer[1] = FC_READ_HOLDING_REGISTERS;
                tx_buffer[2] = num_regs * 2; // Byte count
                for (uint16_t i = 0; i < num_regs; i++) {
                    tx_buffer[3 + i * 2] = sys_regs[start_addr + i] >> 8; // High byte
                    tx_buffer[4 + i * 2] = sys_regs[start_addr + i] & 0xFF; // Low byte
                }
                crc = modbus_crc16(tx_buffer, 3 + num_regs * 2);
                tx_buffer[3 + num_regs * 2] = crc & 0xFF;
                tx_buffer[4 + num_regs * 2] = crc >> 8;

                
                // TODO: set tx wait timer and set tx buffer, then when tx wait interrupt -> send data!
                ////////////////////////////////////////
                // Send response
//                TX1_LED_SET();
                _delay_us(100);
                RS485_TX_ENABLE();
                _delay_us(300);
                for (uint8_t i = 0; i < 5 + num_regs * 2; i++) {
                    
                    while (!UART1_IsTxReady()); //UART1_IsTxReady
                    UART1_Write(tx_buffer[i]); //UART1_Write
//                    ERROR_LED_SET();
                }
                while (!UART1_IsTxDone()); //UART1_IsTxDone
                _delay_us(300);
                RS485_RX_ENABLE();
                TX1_LED_nSET();
                ///////////////////////////////////////
            }   
            break;
        }
        case FC_WRITE_MULTIPLE_REGISTERS: {
            
            uint16_t start_address = (rx_buffer[2] << 8) | rx_buffer[3];
            uint16_t num_regs = (rx_buffer[4] <<8) | rx_buffer[5];
            
            if (start_address + num_regs <= MODBUS_REG_COUNT){
                
                // --- write registers ---
                uint16_t content;
                for (uint8_t i=0; i < num_regs; i++){
                    content = (rx_buffer[7 + i*2] << 8) | rx_buffer[8 + i*2];
                    sys_regs[start_address + i];
                }
                // --- Build Response ---
                // Response: [Addr][FC][StartHi][StartLo][QtyHi][QtyLo][CRC16]
                uint8_t response[8];
                tx_buffer[0] = rx_buffer[0];                    // Slave Address
                tx_buffer[1] = FC_WRITE_MULTIPLE_REGISTERS;     // Function Code
                tx_buffer[2] = rx_buffer[2];                    // Start Address High
                tx_buffer[3] = rx_buffer[3];                    // Start Address Low
                tx_buffer[4] = rx_buffer[4];                    // Quantity High
                tx_buffer[5] = rx_buffer[5];                    // Quantity Low            
                
                // Append CRC
                uint16_t crc = modbus_crc16(response, 6);
                response[6] = crc & 0xFF;         // CRC Low
                response[7] = (crc >> 8) & 0xFF;  // CRC High
                
                // --- Send response ---
//                TX1_LED_SET();
                RS485_TX_ENABLE();
                _delay_us(100);
                for (uint8_t i = 0; i < 5 + num_regs * 2; i++) {                    
                    while (!UART1_IsTxReady()); //UART1_IsTxReady
                    UART1_Write(tx_buffer[i]); //UART1_Write
//                  ERROR_LED_SET();
                }
                while (!UART1_IsTxDone()); //UART1_IsTxDone
                _delay_us(100);
                RS485_RX_ENABLE();
                TX1_LED_nSET();
            }
            
            break;
        }
    }//switch(rxbuffer[1])
    

    rx_index = 0;
    frame_complete = false;
}

// Receive interrupt handler
void modbus_receive(void) {
    if (frame_complete) return;  // don?t touch rx if we already flagged done
    RX1_LED_SET();
    rx_buffer[rx_index++] = UART1_Read();
    TCB0.CNT = 0;
    TCB0.INTFLAGS = TCB_CAPT_bm; // clear pending flag
    TCB0.INTCTRL |= TCB_CAPT_bm;
}

// Timer interrupt handler (called when 4ms silence detected)
// Added to tcb0.c DefaultInteruptHandler
void modbus_timer_expired(void) {
    TX1_LED_SET();
    frame_complete = true;
    RX1_LED_nSET();
    TCB0.INTCTRL &= ~TCB_CAPT_bm; /* Capture or Timeout: disabled */    
}

