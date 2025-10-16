/* 
 * File:   definitions.h
 * Author: Ruhan Louw
 *
 * Created on July 1, 2025, 5:43 PM
 */

#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../mcc_generated_files/system/pins.h"
#include "../mcc_generated_files/system/system.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
    
    // Function-specific pin names
#define XTALHF1_PIN PA0   
#define XTALHF2_PIN PA1
    
#define TIMER_18B20 TCB1_timer
#define SENSOR1_18B20_PIN PA2 // 18b20_1
#define SENSOR2_18B20_PIN PA3 // 18b20_2   
    
#define DHT22_PIN PA7
    
#define TIMEOUT_TIMER TCB0_timer
#define TX1_RS485_PIN PC0
#define RX1_RS485_PIN PC1
#define TX1_RS485_EN_PIN PC3
    
#define TX2 PF0
#define RX2 PF1
#define TX2_EN PF3
    
#define SPI0_MISO_PIN PA5
#define SPI0_SCK_PIN PA6
    
#define TIMER_THERM TCB2_timer
#define THERM_nCS_PIN PD1
#define NTC_EN_PIN PD7
#define nC_SELECT PF4
#define nB_SELECT PF5
#define nA_SELECT PC2
    
#define LED_RX1_PIN PD2
#define LED_TX1_PIN PD3
#define LED_MEAS_PIN PD4
#define LED_ERROR_PIN PD5
#define LED_RUN_PIN PD6

    
    // Pin Functions
#define enable_ntc() IO_PD7_SetHigh()
#define disable_ntc() IO_PD7_SetLow()
    
#define RUN_LED_SET() IO_PD6_SetHigh()
#define RUN_LED_nSET() IO_PD6_SetLow()

#define MEASURE_LED_SET() IO_PD4_SetHigh()
#define MEASURE_LED_nSET() IO_PD4_SetLow()

#define ERROR_LED_SET() IO_PD5_SetHigh()
#define ERROR_LED_nSET() IO_PD5_SetLow()
    
#define TX1_LED_SET() IO_PD3_SetHigh()
#define TX1_LED_nSET() IO_PD3_SetLow()
    
#define RX1_LED_SET() IO_PD2_SetHigh()
#define RX1_LED_nSET() IO_PD2_SetLow()
    
#define TX2_LED_SET() IO_PD3_SetHigh()
#define TX2_LED_nSET() IO_PD3_SetLow()
    
#define RX2_LED_SET() IO_PD2_SetHigh()
#define RX2_LED_nSET() IO_PD2_SetLow()
    


    
////================================= SENSOR CMD ===================================
//// Message format constants
//#define START_BYTE      0xAA  // Start of message
//#define END_BYTE        0xBB  // End of message
//#define MAX_PAYLOAD_LEN 18    // Maximum payload length (9 sensors x 2 bytes for response)
//#define SENSOR_COUNT    9     // Total sensors (8 NTC + 1 K-type)
//
//// Command codes (Master to Sensor PCB)
//#define CMD_READ_NTC_1  0x01  // Read NTC sensor 1 temperature
//#define CMD_READ_NTC_2  0x02  // Read NTC sensor 2 temperature
//#define CMD_READ_NTC_3  0x03  // Read NTC sensor 3 temperature
//#define CMD_READ_NTC_4  0x04  // Read NTC sensor 4 temperature
//#define CMD_READ_NTC_5  0x05  // Read NTC sensor 5 temperature
//#define CMD_READ_NTC_6  0x06  // Read NTC sensor 6 temperature
//#define CMD_READ_NTC_7  0x07  // Read NTC sensor 7 temperature
//#define CMD_READ_NTC_8  0x08  // Read NTC sensor 8 temperature
//#define CMD_READ_KTYPE  0x09  // Read K-type thermocouple temperature
//#define CMD_READ_ALL_NTC 0x0A // Read all NTC sensors (1-8)
//#define CMD_READ_ALL_SENSORS 0x0B // Read all sensors (NTC 1-8 + K-type)
//#define CMD_READ_SELECTED_SENSORS 0x0C // Read selected sensors (2-byte bitmask)
//#define CMD_STATUS      0x0D  // Request sensor PCB status
//#define CMD_RESET       0x0E  // Reset sensor PCB
//#define CMD_DEBUG       0x0F  // Request debug info (e.g., raw ADC values)
//
//// Sensor selection bitmasks (for CMD_READ_SELECTED_SENSORS)
//#define SENSOR_NTC_1    0x0001  // Bit 0: NTC sensor 1
//#define SENSOR_NTC_2    0x0002  // Bit 1: NTC sensor 2
//#define SENSOR_NTC_3    0x0004  // Bit 2: NTC sensor 3
//#define SENSOR_NTC_4    0x0008  // Bit 3: NTC sensor 4
//#define SENSOR_NTC_5    0x0010  // Bit 4: NTC sensor 5
//#define SENSOR_NTC_6    0x0020  // Bit 5: NTC sensor 6
//#define SENSOR_NTC_7    0x0040  // Bit 6: NTC sensor 7
//#define SENSOR_NTC_8    0x0080  // Bit 7: NTC sensor 8
//#define SENSOR_KTYPE    0x0100  // Bit 8: K-type thermocouple
//
//// Response codes (Sensor PCB to Master)
//#define RESP_OK         0x00  // Successful response (used in payload for status)
//#define RESP_ERROR      0xFF  // Error response
//#define ERROR_SENSOR_FAIL 0x01 // Error: Sensor failure or not connected
//#define ERROR_INVALID_CMD 0x02 // Error: Invalid command received
//#define ERROR_INVALID_SENSOR 0x03 // Error: Invalid sensor selection (reserved bits set)
//
//// Timing and buffer constants
//#define RESPONSE_TIMEOUT_MS 100 // Timeout for response (ms)
//#define UART_BAUD_RATE 115200   // UART baud rate
//#define MAX_MESSAGE_LEN (MAX_PAYLOAD_LEN + 5) // Start + Cmd + Len + Payload + Chksum + End
////================================= SENSOR CMD END ===================================
//
//    //
//    
    bool enable(void);

#ifdef	__cplusplus
}
#endif

#endif	/* DEFINITIONS_H */

