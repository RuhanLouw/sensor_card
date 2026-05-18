#include "../mcc_generated_files/uart/usart2.h"
#include "../mcc_generated_files/uart/usart1.h"

#include "system_registers.h"
#include "debug_uart2.h"
#include <stdio.h>
#include <string.h>
#include "../functions/modbus.h"

// Initialize USART2 (called after MCC SYSTEM_Initialize)
void debug_uart_init(void) {
    USART2_Initialize();
}

// Send a string over USART2
void debug_uart_send_string(const char *str) {
    while (*str) {
        while (!UART2_IsTxReady());
        USART2_Write(*str++);
    }
}

 void debug1_send_string(const char *str){
//    RS485_TX_ENABLE();
    while(*str){
        while(!UART1_IsTxReady());
        UART1_Write(*str++);
    }
}

void USART1_write_string(const char *str) {
    while (*str) {
        while (!(USART1.STATUS & USART_DREIF_bm)); // Wait for data register empty
        USART1.TXDATAL = *str++;
        
    }
}

// Call this whenever you are about to send a Modbus frame
void modbus_print_frame(const uint8_t *frame, uint16_t length)
{
    char hexbuf[3 * 64 + 1] = {0};  // worst case: 64 bytes -> 192 chars + null
                                           // (3 chars per byte: "XX ")

    // Safety: don't overflow if someone sends a huge frame
    if (length > 64) length = 64;

    for (uint16_t i = 0; i < length; i++) {
        sprintf(hexbuf + 3*i, "%02X ", frame[i]);   // "A1 00 01 ..." format
    }

    printf("[MB RX] %s\r\n", hexbuf);   // you will see this on the RPi terminal
}

// Send all sensor data in human-readable format
//void debug_uart_send_sensor_data(void) {
//    char buffer[64];
//    
//    // NTC sensors
//    for (uint8_t i = 0; i < 8; i++) {
//        sprintf(buffer, "NTC%d: %d.%dC, Status: %d\n", 
//                i + 1, 
//                sensor_registers[i] / 10, 
//                abs(sensor_registers[i] % 10), 
//                sensor_registers[REG_NTC_STATUS]);
//        debug_uart_send_string(buffer);
//    }
//    
//    // K-type thermocouple
//    sprintf(buffer, "K-type: %d.%dC, Status: %d\n", 
//            sensor_registers[REG_KTYPE_TEMP] / 10, 
//            abs(sensor_registers[REG_KTYPE_TEMP] % 10), 
//            sensor_registers[REG_KTYPE_STATUS]);
//    debug_uart_send_string(buffer);
//    
//    // DS18B20 sensors
//    for (uint8_t i = 0; i < 2; i++) {
//        sprintf(buffer, "DS18B20_%d: %d.%dC, Status: %d\n", 
//                i + 1, 
//                sensor_registers[REG_DS18B20_1_TEMP + i] / 10, 
//                abs(sensor_registers[REG_DS18B20_1_TEMP + i] % 10), 
//                sensor_registers[REG_DS18B20_STATUS]);
//        debug_uart_send_string(buffer);
//    }
//    
//    // DHT22
//    sprintf(buffer, "DHT22 Temp: %d.%dC, Humidity: %d.%d%%, Status: %d\n", 
//            sensor_registers[REG_DHT22_TEMP] / 10, 
//            abs(sensor_registers[REG_DHT22_TEMP] % 10), 
//            sensor_registers[REG_DHT22_HUMIDITY] / 10, 
//            abs(sensor_registers[REG_DHT22_HUMIDITY] % 10), 
//            sensor_registers[REG_DHT22_STATUS]);
//    debug_uart_send_string(buffer);
//}
