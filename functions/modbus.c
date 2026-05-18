/* MODBUS RTU Library for AVR128DB32 - FIXED VERSION */
#include "../mcc_generated_files/uart/usart1.h"
#include "../mcc_generated_files/timer/tcb0.h"
#include "system_registers.h"
#include "modbus.h"
#include "definitions_sensor.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <util/delay.h>
#include <util/crc16.h>
#include "debug_uart2.h"

// ============================================================================
// MODBUS RTU SLAVE - SENSOR CONTROLLER
// ============================================================================
// Author: Ruhan Louw (@Ruhan_Louw)
// Device: AVR (16MHz, USART1, TCB0)
// Baud:   9600, 8N1
// RS485:  PC3 = DE/RE (high = TX, low = RX)
// ============================================================================

// ----------------------------------------------------------------------------
// CONFIGURATION
// ----------------------------------------------------------------------------
#define SLAVE_ADDRESS                   2

#define FC_READ_HOLDING_REGISTERS       0x03
#define FC_READ_INPUT_REGISTER          0x04
#define FC_WRITE_MULTIPLE_REGISTERS     0x10

#define EX_ILLEGAL_FUNCTION             0x01
#define EX_ILLEGAL_DATA_ADDRESS         0x02
#define EX_ILLEGAL_DATA_VALUE           0x03

#define MAX_FRAME_SIZE                  256

// 3.5 char times at 9600 baud = ~3.646 ms ? use 4ms safe margin
// 16MHz / 1 = 16,000,000 * 0.004s = 64,000 ticks
#define MODBUS_TIMEOUT_TICKS            64000

// RS485 driver switching delay (us) - ensures clean transition
#define TX_ENABLE_DELAY_us              1
#define TX_DISABLE_DELAY_ms             1

// ----------------------------------------------------------------------------
// STATE & BUFFERS
// ----------------------------------------------------------------------------
static uint8_t  rx_buffer[MAX_FRAME_SIZE];
static uint8_t  tx_buffer[MAX_FRAME_SIZE];
static volatile uint8_t rx_index = 0;
static volatile bool    frame_ready = false;
char debug_buffer[256] = {0};

extern volatile uint16_t usart1_cnt_ferr;
extern volatile uint16_t usart1_cnt_perr;
extern volatile uint16_t usart1_cnt_bufovf;
extern volatile uint16_t usart1_cnt_sw_ovf;
extern volatile uint8_t valChecker[8];
extern volatile uint8_t valCheckerIdx;


// ----------------------------------------------------------------------------
// INITIALIZATION
// ----------------------------------------------------------------------------
void MB_Init(void) {
    TCB0_Stop();
//    TCB0_PeriodSet(MODBUS_TIMEOUT_TICKS);

    // Register ISRs
    UART1_RxCompleteCallbackRegister(modbus_receive_isr);
    UART1_TxCompleteCallbackRegister(modbus_transmit_isr);
    TCB0_CaptureCallbackRegister(modbus_timer_isr);
    TCB0_CAPTInterruptEnable();

    // Reset state
    rx_index = 0;
    frame_ready = false;

    // Start in receive mode
    RS485_RX_ENABLE();
}

// ----------------------------------------------------------------------------
// RS485 DIRECTION CONTROL
// ----------------------------------------------------------------------------
void RS485_TX_ENABLE(void) {
    IO_PC3_SetHigh();
    _delay_us(TX_ENABLE_DELAY_us);  // Wait for driver to assert
}

void RS485_RX_ENABLE(void) {
    IO_PC3_SetLow();
//    _delay_ms(TX_DISABLE_DELAY_ms); // Wait for driver to release
}

// ----------------------------------------------------------------------------
// CRC16-MODBUS (optimized with _crc16_update)
// ----------------------------------------------------------------------------
static uint16_t modbus_crc16(const uint8_t *buf, uint8_t len) {
    uint16_t crc = 0xFFFF;
    for (uint8_t i = 0; i < len; i++) {
        crc = _crc16_update(crc, buf[i]);
    }
    return crc;
}

// ----------------------------------------------------------------------------
// SEND RESPONSE (adds CRC + transmits)
// ----------------------------------------------------------------------------
static void modbus_send_response(uint8_t length) {
    // Append CRC (low byte first)
    uint16_t crc = modbus_crc16(tx_buffer, length);
    tx_buffer[length]     = crc & 0xFF;
    tx_buffer[length + 1] = (crc >> 8) & 0xFF;

    TCB0_Stop();           // Prevent timeout during TX
    TX1_LED_SET();
    RS485_TX_ENABLE();

//     Transmit all bytes (length + CRC)
    for (uint8_t i = 0; i < length + 2; i++) {
        while (!UART1_IsTxReady());
        UART1_Write(tx_buffer[i]);
    }
}

// ----------------------------------------------------------------------------
// SEND EXCEPTION RESPONSE
// ----------------------------------------------------------------------------
static void modbus_send_exception(uint8_t fc, uint8_t code) {
    tx_buffer[0] = SLAVE_ADDRESS;
    tx_buffer[1] = fc | 0x80;  // Set high bit
    tx_buffer[2] = code;

    modbus_send_response(3);
}

// ----------------------------------------------------------------------------
// HANDLE READ HOLDING / INPUT REGISTERS
// ----------------------------------------------------------------------------
static void handle_read_registers(uint8_t fc) {
    uint16_t start_addr = (rx_buffer[2] << 8) | rx_buffer[3];
    uint16_t num_regs   = (rx_buffer[4] << 8) | rx_buffer[5];

    // Validate quantity
    if (num_regs == 0 || num_regs > 125) {
//        modbus_send_exception(fc, EX_ILLEGAL_DATA_VALUE);
        return;
    }

    // Validate address range
    if (start_addr + num_regs > SREG_COUNT) {
//        modbus_send_exception(fc, EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // Build response
    tx_buffer[0] = SLAVE_ADDRESS;
    tx_buffer[1] = fc;
    tx_buffer[2] = num_regs * 2;  // Byte count

    for (uint16_t i = 0; i < num_regs; i++) {
        uint16_t value = sys_regs[start_addr + i];
        tx_buffer[3 + i * 2]     = (value >> 8) & 0xFF;  // MSB
        tx_buffer[4 + i * 2]     = value & 0xFF;        // LSB
    }

    modbus_send_response(3 + num_regs * 2);
}

// ----------------------------------------------------------------------------
// HANDLE WRITE MULTIPLE REGISTERS
// ----------------------------------------------------------------------------
static void handle_write_multiple_registers(void) {
    uint16_t start_addr = (rx_buffer[2] << 8) | rx_buffer[3];
    uint16_t num_regs   = (rx_buffer[4] << 8) | rx_buffer[5];
    uint8_t  byte_count = rx_buffer[6];

    // Validate
    if (num_regs == 0 || num_regs > 123 || byte_count != num_regs * 2) {
//        modbus_send_exception(FC_WRITE_MULTIPLE_REGISTERS, EX_ILLEGAL_DATA_VALUE);
        return;
    }
    if (start_addr + num_regs > SREG_COUNT) {
//        modbus_send_exception(FC_WRITE_MULTIPLE_REGISTERS, EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // Write data
    for (uint16_t i = 0; i < num_regs; i++) {
        uint16_t value = (rx_buffer[7 + i * 2] << 8) | rx_buffer[8 + i * 2];
        sys_regs[start_addr + i] = (int16_t)value;
    }

    // Echo request header
    tx_buffer[0] = SLAVE_ADDRESS;
    tx_buffer[1] = FC_WRITE_MULTIPLE_REGISTERS;
    tx_buffer[2] = rx_buffer[2];
    tx_buffer[3] = rx_buffer[3];
    tx_buffer[4] = rx_buffer[4];
    tx_buffer[5] = rx_buffer[5];

    modbus_send_response(6);
}

// ----------------------------------------------------------------------------
// MAIN FRAME PROCESSING
// ----------------------------------------------------------------------------
void modbus_process(void) {
    if (!frame_ready) return;
    
    // Print received frame
    printf("%02X %02X %02X %02X %02X %02X %02X %02X\n",
            rx_buffer[0],
            rx_buffer[1],
            rx_buffer[2],
            rx_buffer[3],
            rx_buffer[4],
            rx_buffer[5],
            rx_buffer[6],
            rx_buffer[7]);
    //////////
    
    
//    modbus_print_frame(rx_buffer, rx_index);
    // Minimum valid frame: Addr + FC + CRC = 4 bytes
    if (rx_index < 4) goto cleanup;
        // Check slave address
    if (rx_buffer[0] != SLAVE_ADDRESS){
//        ERROR_LED_TOGGLE();
        goto cleanup;
    }
    // Validate CRC
    uint16_t recv_crc = rx_buffer[rx_index - 2] | (rx_buffer[rx_index - 1] << 8);
    uint16_t calc_crc = modbus_crc16(rx_buffer, rx_index - 2);
    if (recv_crc != calc_crc) {
        ERROR_LED_TOGGLE();
//        printf("hello");
        
        printf("%02X %02X \n", recv_crc, calc_crc);
        goto cleanup;
    }


    
    uint8_t fc = rx_buffer[1];

    switch (fc) {
        case FC_READ_HOLDING_REGISTERS:
        case FC_READ_INPUT_REGISTER:
            if (rx_index == 8) {
                handle_read_registers(fc);
            }
            break;

        case FC_WRITE_MULTIPLE_REGISTERS: {
            uint8_t byte_count = rx_buffer[6];
            uint8_t expected = 7 + byte_count + 2;
            if (rx_index == expected && byte_count >= 2) {
                handle_write_multiple_registers();
            }
            break;
        }

        default:
//            modbus_send_exception(fc, EX_ILLEGAL_FUNCTION);
            break;
    }

cleanup:
    rx_index = 0;
    frame_ready = false;
}

// ----------------------------------------------------------------------------
// INTERRUPT SERVICE ROUTINES
// ----------------------------------------------------------------------------

// RX: Byte received
void modbus_receive_isr(void) {

    rx_buffer[rx_index] = UART1_Read();
    rx_index++;
    RX1_LED_SET();
    

    TCB0_Start();
    TCB0_CounterSet(0);
}

// TX: Transmission complete
void modbus_transmit_isr(void) {
    RS485_RX_ENABLE();
    TX1_LED_nSET();
}

// Timer: Inter-character timeout (3.5 char times)
void modbus_timer_isr(void) {
    TCB0_Stop();
    if (rx_index > 0) {
        frame_ready = true;
    }
    RX1_LED_nSET();
    
    printf("\nvalChecker %02X %02X %02X %02X %02X %02X %02X %02X\n",
        valChecker[0],
        valChecker[1],
        valChecker[2],
        valChecker[3],
        valChecker[4],
        valChecker[5],
        valChecker[6],
        valChecker[7]);
    
    for (uint8_t i = 0; i < 8; i++) valChecker[i] = 0;
    valCheckerIdx = 0;
}

// ----------------------------------------------------------------------------
// DIAGNOSTIC / DEBUG FUNCTIONS
// ----------------------------------------------------------------------------
//uint8_t MB_GetRxIndex(void) {
//    return rx_index;
//}
//
//bool MB_IsFrameReady(void) {
//    return frame_ready;
//}
//
//// Echo last received frame (for debugging via master)
//void MB_EchoLastFrame(void) {
//    if (rx_index == 0) return;
//
//    TCB0_Stop();
//
//    uint8_t data_len = rx_index - 2;  // Exclude CRC
//    uint16_t calc_crc = modbus_crc16(rx_buffer, data_len);
//    uint16_t recv_crc = rx_buffer[rx_index - 2] | (rx_buffer[rx_index - 1] << 8);
//
//    tx_buffer[0] = SLAVE_ADDRESS;
//    tx_buffer[1] = 0xFF;                    // Echo function
//    tx_buffer[2] = rx_index;                // Total received bytes
//    uint8_t echo_bytes = (rx_index < 20) ? rx_index : 20;
//    memcpy(&tx_buffer[3], rx_buffer, echo_bytes);
//
//    uint8_t offset = 3 + echo_bytes;
//    tx_buffer[offset++] = calc_crc & 0xFF;
//    tx_buffer[offset++] = (calc_crc >> 8) & 0xFF;
//    tx_buffer[offset++] = recv_crc & 0xFF;
//    tx_buffer[offset++] = (recv_crc >> 8) & 0xFF;
//
//    modbus_send_response(offset);
//    rx_index = 0;
//    frame_ready = false;
//}

// ============================================================================
// END OF FILE
// ============================================================================
//
//    if (rx_index >= MAX_FRAME_SIZE) {
//        rx_index = 0;
//        TCB0_Stop();
//        (void)UART1_Read();
//        return;
//    }
//#include "../mcc_generated_files/uart/usart1.h"
//#include "../mcc_generated_files/timer/tcb0.h"
//#include "system_registers.h"
//#include "modbus.h"
//#include "definitions.h"
//#include <stdint.h>
//#include <stdbool.h>
//#include <string.h>
//#include <util/delay.h>
//#include "debug_uart2.h"
//
//// ============================================================================
//// CONFIGURATION
//// ============================================================================
//
//#define SLAVE_ADDRESS 2
//
//// Function codes
//#define FC_READ_HOLDING_REGISTERS   0x03
//#define FC_READ_INPUT_REGISTER      0x04
//#define FC_WRITE_MULTIPLE_REGISTERS 0x10
//
//// Exception codes
//#define EX_ILLEGAL_FUNCTION         0x01
//#define EX_ILLEGAL_DATA_ADDRESS     0x02
//#define EX_ILLEGAL_DATA_VALUE       0x03
//
//// Buffer sizes
//#define MAX_FRAME_SIZE 256
//
//// Timing for 9600 baud: 3.5 char time = 3.646ms
//// At 16MHz with DIV1: 16000000 * 0.004 = 64000 = 0xFA00
//// Use 4ms to be safe
//#define MODBUS_TIMEOUT_TICKS 64000
//
//// RS485 switching delays (microseconds)
//#define TX_ENABLE_DELAY_MS  100
//#define TX_DISABLE_DELAY_MS 100
//
////char debug_tx[128] = {0};
//
//// ============================================================================
//// STATE MANAGEMENT
//// ============================================================================
//static uint8_t rx_buffer[MAX_FRAME_SIZE];
//static uint8_t tx_buffer[MAX_FRAME_SIZE];
//static volatile uint8_t rx_index = 0;
//static volatile bool frame_ready = false;
//
//// ============================================================================
//// INITIALIZATION
//// ============================================================================
//
//void MB_Init(void) {
//    TCB0_Stop();
//    TCB0_PeriodSet(MODBUS_TIMEOUT_TICKS); // Set compare value for timeout
//    
//    // Register callbacks
//    UART1_RxCompleteCallbackRegister(modbus_receive_isr);
//    UART1_TxCompleteCallbackRegister(modbus_transmit_isr);
//    TCB0_CAPTInterruptEnable();  // Enable capture/timeout interrupt
//    TCB0_CaptureCallbackRegister(modbus_timer_isr);
//    
//    // Initialize state
//    rx_index = 0;
//    frame_ready = false;
//    
//    // Start in RX mode
//    RS485_RX_ENABLE();
//}
//
//// ============================================================================
//// RS485 CONTROL
//// ============================================================================
//
//void RS485_TX_ENABLE(void) {
//    IO_PC3_SetHigh();
//    _delay_ms(TX_ENABLE_DELAY_MS);
//}
//
//void RS485_RX_ENABLE(void) {
//    IO_PC3_SetLow();
//    _delay_ms(TX_DISABLE_DELAY_MS);
//}
//
//// ============================================================================
//// CRC CALCULATION
//// ============================================================================
//static uint16_t modbus_crc16(const uint8_t *buf, uint8_t len)
//{
//    uint16_t crc = 0xFFFF;
//    for(uint8_t i = 0; i < len; ++i) {
//        crc = _crc16_update(crc, buf[i]);
//    }
//    return crc;
//}
////uint16_t modbus_crc16(const uint8_t *data, uint8_t length) {
////    uint16_t crc = 0xFFFF;
////    uint8_t i;
////
////    while (length--) {
////        crc ^= *data++;
////        for (i = 0; i < 8; i++) {
////            if (crc & 1)
////                crc = (crc >> 1) ^ 0xA001;
////            else
////                crc >>= 1;
////        }
////    }
////    return crc;
////}
////CRC Example Code
////
////This function is an example how to calculate a CRC word using the C language.
//
////uint16_t modbus_crc16(const uint8_t *data, uint8_t length){
////    
////    static const uint16_t wCRCTable[] = {
////       0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
////       0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
////       0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
////       0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
////       0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
////       0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
////       0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
////       0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
////       0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
////       0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
////       0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
////       0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
////       0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
////       0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
////       0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
////       0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
////       0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
////       0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
////       0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
////       0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
////       0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
////       0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
////       0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
////       0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
////       0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
////       0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
////       0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
////       0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
////       0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
////       0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
////       0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
////       0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };
////
////    uint8_t nTemp;
////    uint16_t wCRCWord = 0xFFFF;
////
////   while (length--)
////   {
////      nTemp = *data++ ^ wCRCWord;
////      wCRCWord >>= 8;
////      wCRCWord  ^= wCRCTable[nTemp];
////   }
////   return wCRCWord;
////} // End: CRC16
//
//
//
//// ============================================================================
//// FRAME TRANSMISSION
//// ============================================================================
//
//static void modbus_send_response(uint8_t length) {
//    // Calculate and append CRC
//    uint16_t crc = modbus_crc16(tx_buffer, length);
//    tx_buffer[length] = crc & 0xFF;
//    tx_buffer[length + 1] = (crc >> 8) & 0xFF;
//    
//    TCB0_Stop(); // Stop timeout timer during transmission
//        
//    TX1_LED_SET();
//    RS485_TX_ENABLE();
//
//    // Transmit frame
//    for (uint8_t i = 0; i < length + 2; i++) {
//        while (!UART1_IsTxReady());
//        UART1_Write(tx_buffer[i]);
//    }
//
//}
//
//// ============================================================================
//// EXCEPTION RESPONSE
//// ============================================================================
//
//static void modbus_send_exception(uint8_t function_code, uint8_t exception_code) {
//    tx_buffer[0] = SLAVE_ADDRESS;
//    tx_buffer[1] = function_code | 0x80;  // Set exception bit
//    tx_buffer[2] = exception_code;
//    
//    modbus_send_response(3);
//}
//
//// ============================================================================
//// MODBUS FUNCTION HANDLERS
//// ============================================================================
//
//static void handle_read_registers(uint8_t function_code) {
//    uint16_t start_addr = (rx_buffer[2] << 8) | rx_buffer[3];
//    uint16_t num_regs = (rx_buffer[4] << 8) | rx_buffer[5];
//    
//    // Validate request
//    if (num_regs == 0 || num_regs > 125) {
//        modbus_send_exception(function_code, EX_ILLEGAL_DATA_VALUE);
//        return;
//    }
//    
//    if (start_addr + num_regs > SREG_COUNT) {
//        modbus_send_exception(function_code, EX_ILLEGAL_DATA_ADDRESS);
//        return;
//    }
//    
//    // Build response
//    tx_buffer[0] = SLAVE_ADDRESS;
//    tx_buffer[1] = function_code;
//    tx_buffer[2] = num_regs * 2;  // Byte count
//    
//    for (uint16_t i = 0; i < num_regs; i++) {
//        tx_buffer[3 + i * 2] = (sys_regs[start_addr + i] >> 8) & 0xFF;  // High byte
//        tx_buffer[4 + i * 2] = sys_regs[start_addr + i] & 0xFF;          // Low byte
//    }
////    ERROR_LED_SET();
//    modbus_send_response(3 + num_regs * 2);
//}
//
//static void handle_write_multiple_registers(void) {
//    uint16_t start_addr = (rx_buffer[2] << 8) | rx_buffer[3];
//    uint16_t num_regs = (rx_buffer[4] << 8) | rx_buffer[5];
//    uint8_t byte_count = rx_buffer[6];
//    
//    // Validate request
//    if (num_regs == 0 || num_regs > 123 || byte_count != num_regs * 2) {
//        modbus_send_exception(FC_WRITE_MULTIPLE_REGISTERS, EX_ILLEGAL_DATA_VALUE);
//        return;
//    }
//    
//    if (start_addr + num_regs > SREG_COUNT) {
//        modbus_send_exception(FC_WRITE_MULTIPLE_REGISTERS, EX_ILLEGAL_DATA_ADDRESS);
//        return;
//    }
//    
//    // Write registers
//    for (uint16_t i = 0; i < num_regs; i++) {
//        uint16_t value = (rx_buffer[7 + i * 2] << 8) | rx_buffer[8 + i * 2];
//        sys_regs[start_addr + i] = value;
//    }
//    
//    // Build response (echo request without data)
//    tx_buffer[0] = SLAVE_ADDRESS;
//    tx_buffer[1] = FC_WRITE_MULTIPLE_REGISTERS;
//    tx_buffer[2] = rx_buffer[2];  // Start address high
//    tx_buffer[3] = rx_buffer[3];  // Start address low
//    tx_buffer[4] = rx_buffer[4];  // Quantity high
//    tx_buffer[5] = rx_buffer[5];  // Quantity low
//    
//    modbus_send_response(6);
//}
//
//// ============================================================================
//// FRAME PROCESSING
//// ============================================================================
//
//void modbus_process(void) {
//    if (!frame_ready) {
//        return;
//    }
//
//    bool valid_frame = false;
//    
//    // Minimum frame: Address(1) + Function(1) + CRC(2) = 4 bytes
//    if (rx_index >= 4) {
//        // Validate CRC
//        uint16_t received_crc = rx_buffer[rx_index - 2] | (rx_buffer[rx_index - 1] << 8);
//        uint16_t calculated_crc = modbus_crc16(rx_buffer, rx_index - 2);
//        
//        if (received_crc == calculated_crc) {
//            // Check if message is for us
//            if (rx_buffer[0] == SLAVE_ADDRESS) {
//                valid_frame = true;
//                
//                // Process function code
//                uint8_t function_code = rx_buffer[1];
//                
//                switch (function_code) {
//                    case FC_READ_HOLDING_REGISTERS:
//                    case FC_READ_INPUT_REGISTER:
//                        // Read requests are always exactly 8 bytes
//                        if (rx_index == 8) {
//                            handle_read_registers(function_code);
//                        } else {
//                            // Malformed frame - wrong size for this function
////                            ERROR_LED_TOGGLE();                        
//                        }
//                        break;
//                        
//                    case FC_WRITE_MULTIPLE_REGISTERS:
//                        // Write requests: 9 bytes minimum (7 header + 2 data + 2 CRC)
//                        if (rx_index >= 11 && rx_index <= 256) {
//                            // Validation: byte count should match frame size
//                            uint8_t byte_count = rx_buffer[6];
//                            uint8_t expected_size = 7 + byte_count + 2;
//                            
//                            if (rx_index == expected_size) {
//                                handle_write_multiple_registers();
//                            } else {
////                                ERROR_LED_TOGGLE(); // Frame size mismatch
//                            }
//                        } else {
////                            ERROR_LED_TOGGLE(); // Frame too small or too large
//                        }
//                        break;
//                        
//                    default:
//                        // Unknown function code - send exception response
//                        modbus_send_exception(function_code, EX_ILLEGAL_FUNCTION);
//                        break;
//                }
//            }
//        } else {
//            ERROR_LED_TOGGLE(); // CRC error
//        }
//    }
//    
//    // Reset state for next frame
//    rx_index = 0;
//    frame_ready = false;
//}
//
//// ============================================================================
//// INTERRUPT HANDLERS - FIXED VERSION
//// ============================================================================
//
//void modbus_receive_isr(void) {
//    
//    // Buffer overflow protection
//    if (rx_index >= MAX_FRAME_SIZE) {
//        rx_index = 0;
//        TCB0_Stop();
//        (void)UART1_Read();  // Discard byte
//        return;
//    }
//    
//    // Read byte and store
//    rx_buffer[rx_index++] = UART1_Read();
//    
//    // Restart timeout timer for inter-character timeout
//    TCB0_Stop();
//    TCB0_CounterSet(0);
//    TCB0_Start();
//    
//    RX1_LED_SET();
//}
//
//void modbus_transmit_isr(void){
//    RS485_RX_ENABLE();
//    TX1_LED_nSET();
//}
//
//void modbus_timer_isr(void) {
//    // Stop the timer
//    TCB0_Stop();
//    
//    // Frame timeout - mark as complete if we have data
//    if (rx_index > 0) {
//        frame_ready = true;
//    }
//    RX1_LED_nSET();
//}
//
//// ============================================================================
//// DIAGNOSTIC FUNCTIONS
//// ============================================================================
//
//// Add these for debugging
//uint8_t MB_GetRxIndex(void) {
//    return rx_index;
//}
//
//bool MB_IsFrameReady(void) {
//    return frame_ready;
//}
//
//modbus_state_t MB_GetState(void) {
//    return mb_state;
//}
//
//void MB_GetLastFrame(uint8_t *buffer, uint8_t *length) {
//    *length = rx_index;
//    for (uint8_t i = 0; i < rx_index && i < MAX_FRAME_SIZE; i++) {
//        buffer[i] = rx_buffer[i];
//    }
//}
//
////void print_hex_frame(uint8_t *frame, uint8_t len) {
////    sprintf(debug_tx, "received: ");
////    int offset = strlen(debug_tx);
////
////    for (uint8_t i = 0; i < len; i++) {
////        sprintf(debug_tx + offset, "%02X ", frame[i]);
////        offset += 3;
////    }
////
////    // Remove trailing space
////    if (offset > 0) debug_tx[offset - 1] = '\0';
////
////    sprintf(debug_tx + offset - 1, "\r\n");
////    RS485_TX_ENABLE();
////    _delay_ms(100);
////    USART1_write_string(debug_tx);
////    _delay_ms(100);
////    RS485_TX_ENABLE();
////    _delay_ms(100);
////}
//
//void MB_EchoLastFrame(void) {
//    if (rx_index == 0) return;
//   
//    TCB0_Stop();
//
//    // COPY ONLY VALID BYTES TO TEMP BUFFER
//    uint8_t valid_frame[MAX_FRAME_SIZE];
//    memcpy(valid_frame, rx_buffer, rx_index);
//
//    // CALCULATE CRC ON CLEAN DATA
//    uint16_t calculated_crc = modbus_crc16(valid_frame, rx_index - 2);
//    uint16_t received_crc = valid_frame[rx_index - 2] | (valid_frame[rx_index - 1] << 8);
//
//    // Build response
//    tx_buffer[0] = SLAVE_ADDRESS;
//    tx_buffer[1] = 0xFF;
//    tx_buffer[2] = rx_index;
//
//    uint8_t echo_len = (rx_index < 20) ? rx_index : 20;
//    for (uint8_t i = 0; i < echo_len; i++) {
//        tx_buffer[3 + i] = valid_frame[i];
//    }
//
//    uint8_t offset = 3 + echo_len;
//    tx_buffer[offset]     = calculated_crc & 0xFF;
//    tx_buffer[offset + 1] = (calculated_crc >> 8) & 0xFF;
//    tx_buffer[offset + 2] = received_crc & 0xFF;
//    tx_buffer[offset + 3] = (received_crc >> 8) & 0xFF;
//
//    modbus_send_response(offset + 4);
//
//    rx_index = 0;
//    frame_ready = false;
//    mb_state = MB_STATE_IDLE;
//}
//
//// ============================================================================
//// END OF FILE
//// ============================================================================