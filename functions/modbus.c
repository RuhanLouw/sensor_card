#include "../mcc_generated_files/uart/usart1.h"
#include "../mcc_generated_files/timer/tcb0.h"
#include "system_registers.h"
#include "modbus.h"
#include "definitions.h"
#include <stdint.h>
#include <stdbool.h>

// Modbus slave address
#define SLAVE_ADDRESS 2

// Function codes
#define FC_READ_HOLDING_REGISTERS 0x03
#define FC_READ_INPUT_REGISTERS 0x04
#define FC_WRITE_MULTIPLE_REGISTERS 0x10
#define FC_READ_INPUT_REGISTER 0x04

// Buffer sizes
#define MAX_FRAME_SIZE 256

// Modbus frame buffer
static uint8_t rx_buffer[MAX_FRAME_SIZE];
static uint8_t tx_buffer[MAX_FRAME_SIZE];
static uint8_t rx_index = 0;
static volatile bool frame_complete = false;

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
    
    IO_PD4_Toggle();
    
    // Validate address and CRC
    uint16_t crc = modbus_crc16(rx_buffer, rx_index - 2);
    
    if (rx_buffer[0] != SLAVE_ADDRESS || 
        rx_buffer[rx_index - 2] != (crc & 0xFF) || 
        rx_buffer[rx_index - 1] != (crc >> 8)) {
        rx_index = 0;
        frame_complete = false;
        return;
    }

    switch(rx_buffer[1]){
        case FC_READ_INPUT_REGISTER: {
            
            
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

                // Send response
                RS485_TX_ENABLE();
                for (uint8_t i = 0; i < 5 + num_regs * 2; i++) {
                    while (!UART1_IsTxReady());
                    UART1_Write(tx_buffer[i]);
                }
                while (!UART1_IsTxDone());
                RS485_RX_ENABLE();
                
            }   
            break;
        }
        case FC_WRITE_MULTIPLE_REGISTERS: {
            // todo
            
            
            break;
        }
    }//switch(rxbuffer[1])
    
    // Handle function code 03
    if (rx_buffer[1] == FC_READ_HOLDING_REGISTERS) {

    }
    
    // Handle other MODBUS function
    // Function code -> to be written into sensor card
    
    

    rx_index = 0;
    frame_complete = false;
}

// Receive interrupt handler
void modbus_receive(uint8_t data) {
    MEASURE_LED_SET();
    if (rx_index < MAX_FRAME_SIZE) {
        rx_buffer[rx_ndex++] = data;
        // Reset timer on each byte
        TCB0_CNT = 0; // Reset counter
        TCB0_CAPTInterruptEnable(); // Enable interrupt
    } else {
        rx_index = 0; // Overflow, reset
        frame_complete = false;
    }
    
}

// Timer interrupt handler (called when 4ms silence detected)
// Added to tcb0.c DefaultInteruptHandler
void modbus_timer_expired(void) {
    frame_complete = true;
    TCB0_CAPTInterruptDisable(); // Disable until next byte
    
}
