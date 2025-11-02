 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.2
 *
 * @version Package Version: 3.1.2
*/
#define F_CPU 16000000UL
#include <util/delay.h>

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
#include "mcc_generated_files/system/system.h"
#include "functions/definitions.h"
#include "functions/temp_sensors.h"
//#include "functions/system_cmd.h"
#include "functions/ds18b20.h"
#include "functions/debug_uart2.h"
#include "functions/system_registers.h"
#include "functions/modbus.h"

/*
    Main application
*/
/* ============================================================
 * GLOBAL SENSOR INSTANCES
 * ============================================================ */

uint8_t ntc_samples = 10; // Set the number of samples to avg for the NTC sensors

// System type
typedef struct {
    NTC_SENSOR_t ntcs[8];
    KTYPE_SENSOR_t ktype;
    DHT22_SENSOR_t dht22;
    DS18B20_SENSOR_t ds18b20[2];
}SYS_SENSORS;

// System Sensors Registers
SYS_SENSORS sys_sensors;

/* ============================================================
 * FUNCTION: INITIALISE SYSTEM REGISTERS
 * ============================================================ */
void SYS_REGS_INIT(void){   
    // System info registers
    sys_regs[MB_REG_FIRMWARE_VERSION] = 100; // v1.00
    sys_regs[MB_REG_UPTIME_LSW] = 2;         // To be filled from timer
    sys_regs[MB_REG_UPTIME_MSW] = 3;
    
    // --- SET ---
    sys_regs[MB_REG_SYSTEM_STATUS] = SYS_STAT_SYSTEM_READY;
    sys_regs[MB_REG_SENSOR_ENABLE_FLAGS] = (
                EN_FLAG_NTC1
            |   EN_FLAG_NTC2
            |   EN_FLAG_NTC3
            |   EN_FLAG_NTC4
            |   EN_FLAG_NTC5
            |   EN_FLAG_NTC6
            |   EN_FLAG_NTC7
            |   EN_FLAG_NTC8
            |   EN_FLAG_KTYPE   
//          |   EN_FLAG_DHT22
//          |   EN_FLAG_DS18B20_1
//          |   EN_FLAG_DS18B20_2
            );
    
    // --- NOT SET ---
    sys_regs[MB_REG_SYSTEM_STATUS] &= ~SYS_STAT_SENSOR_POLL_ACTIVE;
}

/* ============================================================
 * FUNCTION: Update Modbus Registers from Sensor Structs
 * ============================================================ */
void update_sensor_registers(void) {

    // NTC sensors (use int i, temp + error addr = 32)
    for (int i = 0; i < 8; i++) {
        sys_regs[MB_REG_NTC1_TEMP + i] = sys_sensors.ntcs[i].temp;
        sys_regs[MB_REG_NTC1_ERROR + i] = sys_sensors.ntcs[i].error;
    }

    // K-type thermocouple
    sys_regs[MB_REG_KTYPE_TEMP] = sys_sensors.ktype.temp;
    sys_regs[MB_REG_KTYPE_CJ_TEMP] = sys_sensors.ktype.cold_junction;
    sys_regs[MB_REG_KTYPE_ERROR] = sys_sensors.ktype.error;

    // DHT22
    sys_regs[MB_REG_DHT22_TEMP] = sys_sensors.dht22.temp;
    sys_regs[MB_REG_DHT22_HUMIDITY] = sys_sensors.dht22.humidity;
    sys_regs[MB_REG_DHT22_ERROR] = sys_sensors.dht22.error;

    // DS18B20
    for (int i = 0; i < 2; i++) {
        sys_regs[MB_REG_DS18B20_1_TEMP + i] = sys_sensors.ds18b20[i].temp;
        sys_regs[MB_REG_DS18B20_ERROR] = sys_sensors.ds18b20[i].error;
    }
}

/* ============================================================
 * FUNCTION: Update Sensor Data 
 * ============================================================ */
void poll_sensors(void) {
    MEASURE_LED_SET();
    if(sys_regs[MB_REG_COMMAND] & SYS_STAT_SENSOR_POLL_ACTIVE){
        // NTC Poll
        
        for(uint8_t i=0; i<8; i++){
            if(sys_regs[MB_REG_SENSOR_ENABLE_FLAGS] & (EN_FLAG_NTC1 << i)){
                sys_sensors.ntcs[i] = read_ntc(i, ntc_samples); 
            }
        }
        // KTYPE Poll
        if(KTYPE_check_state() == KTYPE_READ_READY &&
                (sys_regs[MB_REG_SENSOR_ENABLE_FLAGS] & EN_FLAG_KTYPE)){
            sys_sensors.ktype = read_ktype(); 
        }
        // DS18B201 Poll
        if(DS18B201_check_state() == DS18B20_STATE_READY &&
                (sys_regs[MB_REG_SENSOR_ENABLE_FLAGS] & EN_FLAG_DS18B20_1)) {
            sys_sensors.ds18b20[0] = read_ds18b20(0);
        }
        // DS18B202 Poll
        if(DS18B202_check_state() == DS18B20_STATE_READY &&
                (sys_regs[MB_REG_SENSOR_ENABLE_FLAGS] & EN_FLAG_DS18B20_2)){
            sys_sensors.ds18b20[1] = read_ds18b20(1);
        }
        // DHT22 Poll
//        if(DHT22_STATE == DHT22_READ_READY &&
//                 (sys_regs[MB_REG_SENSOR_ENABLE_FLAGS] & EN_FLAG_KTYPE)){
//            sys_sensors.dht22 = read_dht22();
//        }
    
        // After polling, update the Modbus register array
        MEASURE_LED_nSET();
        update_sensor_registers();
    
    }
}

/* ============================================================
 * MODBUS READ/WRITE HANDLERS
 * ============================================================ */
//uint16_t modbus_read_register(uint16_t reg_addr) {
//    if (reg_addr < MODBUS_REG_COUNT) {
//        return modbus_regs[reg_addr];
//    }
//    return 0;
//}
//
//void modbus_write_register(uint16_t reg_addr, uint16_t value) {
//    if (reg_addr < MODBUS_REG_COUNT) {
//        modbus_regs[reg_addr] = value;
//
//        // If writing to command or config registers, update structs
//        if (reg_addr == MB_REG_COMMAND) {
//            if (value & CMD_SYS_RESET) {
//                // handle system reset
//            }
//            if (value & CMD_FORCE_REFRESH) {
//                poll_sensors();
//            }
//        } else if (reg_addr == MB_REG_LOG_INTERVAL) {
//            // update logging interval
//        }
//    }
//}

/* ============================================================
 * INTERRUPT SERVISE
 * ============================================================ */
//ISR(USART1_RXC_vect)
//{
//    uint8_t byte = USART1.RXDATAL; // read received byte
//    modbus_receive(byte);           // send to your own handler
//}

// System registers init:

/* ============================================================= */


/* ============================================================
 * MAIN LOOP
 * ============================================================ */
int main(void) {
    SYSTEM_Initialize();
//    TCB0_CAPTInterruptEnable();
//    SYS_REGS_INIT();
    sys_regs[0] = 0x1234;
    sys_regs[1] = 0x5678;
    // Initialize hardware, timers, RS485, etc.
    RUN_LED_SET();
    /*Init Functions*/
//    Init_sensors();
//    KTYPE_start_conversion();

    while (1) {
        poll_sensors(); // update sensor structs and registers
        modbus_process(); // Handle Modbus requests from control card
        
        _delay_ms(50); // adjust polling rate

        
//        buffer = read_ktype();
//        sprintf(debug1, "RS485 Received\n");
//        debug1_send_string(debug1);
//        char debug1[64];
//        sprintf(debug1, "Temp: %d, Junc: %d, Err: %d \n", 
//                (int)buffer.temp, 
//                (int)buffer.cold_junction,
//                (int)buffer.error
//                );
//        debug1_send_string(debug1);
    }
}
   





//============================================= OLD ==================================================//

//// Modbus flag (set by USART1_RXC_vect, processed in main loop)
//volatile bool modbus_data_ready = false;
//int16_t modbus_regs[];
//
//// UART0 buffer for ESP debugging
//char debug_buffer[64];
//
//
//
//int main(void) {
//    ////
//    SYSTEM_Initialize();
//    RUN_LED_SET();
//    sei(); 
//    ////
//    
//    // Init Functions
//    SPI0_Open(0);
//
//    char testBuffer[2];
//    float thermoTemp, juncTemp, ntcTemp;
//    KTYPE_ERROR_t KTYPE_ERROR;
//    
//    // Loop
//    while(1){
//        // MODBUS
//        
//        
//        
//        
//        // NTC Temperature Readings
//        ntcTemp = getTemp_NTC(3, 10);
//        printf("ntc3: %.2f\n", ntcTemp);
//        ntcTemp = getTemp_NTC(5, 10);
//        printf("ntc5: %.2f\n", ntcTemp);
//        ntcTemp = getTemp_NTC(8, 10);
//        printf("ntc8: %.2f\n", ntcTemp);
//        
//        // KTYPE Thermocouple Readings
//        KTYPE_ERROR = readKTypeSensor(&thermoTemp, &juncTemp);
//        if(KTYPE_ERROR != KTYPE_OK) printf("%d", KTYPE_ERROR);
//        else printf("Thermocouple = %.2f °C, Junction = %.2f °C, Return = %d\n", thermoTemp, juncTemp, KTYPE_ERROR); 
//        
//        
//        
//        //
//        // System Register Update:
//        // Copy NTC structs to modbus registers
//        for (int i = 0; i < 8; i++) {
//            modbus_regs[MB_REG_NTC1_TEMP + i] = ntcs[i].temp;
//            modbus_regs[MB_REG_NTC1_ERROR + i] = ntcs[i].error;
//        }
//
//        // Copy K-type
//        modbus_regs[MB_REG_KTYPE_TEMP] = ktype.temp;
//        modbus_regs[MB_REG_KTYPE_CJ_TEMP] = ktype.cold_junction;
//        modbus_regs[MB_REG_KTYPE_ERROR] = ktype.error;
//
//        // Copy DHT22
//        modbus_regs[MB_REG_DHT22_TEMP] = dht22.temp;
//        modbus_regs[MB_REG_DHT22_HUMIDITY] = dht22.humidity;
//        modbus_regs[MB_REG_DHT22_ERROR] = dht22.error;
//
//        
//        //
//        
//        _delay_ms(1000);
//        
//    }
//}



//// MCP3201 SPI read (single reading, ~12.5us at 1.6MHz)
//uint16_t mcp3201_read(uint8_t adc_index) {
//    // Select ADC via multiplexer (active-low CS)
//    MUX_PORT.OUT = (MUX_PORT.OUT & ~(MUX_A0 | MUX_A1 | MUX_A2)) | (adc_index & 0x07);
//    SPI0.CTRLB |= SPI_SS_bm; // Ensure CS high
//    SPI0.CTRLB &= ~SPI_SS_bm; // CS low
//    uint8_t high_byte = SPI0_exchange_byte(0x00); // Start bit, null bit
//    uint8_t low_byte = SPI0_exchange_byte(0x00); // Read 8 LSBs
//    SPI0.CTRLB |= SPI_SS_bm; // CS high
//    return ((high_byte & 0x0F) << 8) | low_byte; // 12-bit result
//}
//
//// NTC read (10 samples averaged per ADC)
//void read_ntc_sensors(float *ntc_values) {
//    for (uint8_t adc = 0; adc < 8; adc++) {
//        uint32_t sum = 0;
//        uint8_t valid_count = 0;
//        for (uint8_t i = 0; i < 10; i++) {
//            uint16_t value = mcp3201_read(adc); // ~12.5us
//            if (value <= 4095) { // Validate reading
//                sum += value;
//                valid_count++;
//            }
//            _delay_us(10); // Processing delay
//        }
//        ntc_values[adc] = valid_count ? (float)sum / valid_count : 0.0; // Average
//        sprintf(debug_buffer, "NTC%d: %.0f ADC\n", adc + 1, ntc_values[adc]);
//        USART0_write_string(debug_buffer); // Send to ESP
//    }
//}
//
//// Placeholder for Modbus processing
//void process_modbus_data(void) {
//    if (modbus_data_ready) {
//        sprintf(debug_buffer, "Modbus data received\n");
//        USART0_write_string(debug_buffer); // Send to ESP
//        modbus_data_ready = false; // Reset flag
//    }
//}

// UART0 write string for ESP debugging (polled TX)

//    while (1) {
//        // Start DS18B20 conversions if idle
//        if (ds18b20_check_conversion1() == DS18B20_STATE_IDLE) {
//            status1 = ds18b20_start_conversion1();
//            sprintf(debug_buffer, "18b20 start conversion...");
//            USART0_write_string(debug_buffer);
//            if (status1 != DS18B20_OK) {
//                sprintf(debug_buffer, "Sensor 1 (PA3): Error %d\n", status1);
//                USART0_write_string(debug_buffer);
//            }
//        }
//        if (ds18b20_check_conversion2() == DS18B20_STATE_IDLE) {
//            status2 = ds18b20_start_conversion2();
//            if (status2 != DS18B20_OK) {
//                sprintf(debug_buffer, "Sensor 2 (PA4): Error %d\n", status2);
//                USART0_write_string(debug_buffer);
//            }
//        }
//
////        // Read NTCs during conversion (~1.8ms for 8 NTCs)
//        read_ntc_sensors(ntc_values);
////
//        // Check DS18B20 conversions
//        if (ds18b20_check_conversion1() == DS18B20_STATE_READY) {
//            status1 = ds18b20_read_temp1(&temp1);
//            if (status1 == DS18B20_OK) {
//                sprintf(debug_buffer, "Sensor 1 (PA3): %.2f C\n", temp1);
//                USART0_write_string(debug_buffer);
//            } else {
//                sprintf(debug_buffer, "Sensor 1 (PA3): Error %d\n", status1);
//                USART0_write_string(debug_buffer);
//            }
//        }
//        if (ds18b20_check_conversion2() == DS18B20_STATE_READY) {
//            status2 = ds18b20_read_temp2(&temp2);
//            if (status2 == DS18B20_OK) {
//                sprintf(debug_buffer, "Sensor 2 (PA4): %.2f C\n", temp2);
//                USART0_write_string(debug_buffer);
//            } else {
//                sprintf(debug_buffer, "Sensor 2 (PA4): Error %d\n", status2);
//                USART0_write_string(debug_buffer);
//            }
//        }
//        sprintf(debug_buffer, "main\n");
//        USART0_write_string(debug_buffer);
//        ntc_meas[0] = temp_betaC_NTC(3, 10);
//        sprintf(debug_buffer, "%.1f\n", ntc_meas[0]);
//        USART0_write_string(debug_buffer);
//        
//        _delay_ms(1000);
//        ntc_meas[0] = temp_betaC_NTC(1, 10);
//        sprintf(debug_buffer, "%.1f\n", ntc_meas[0]);
//        USART0_write_string(debug_buffer);
//        ntc_meas[1] = temp_betaC_NTC(2, 10);
//        sprintf(debug_buffer, "NTC2: %.1f 'C\n", ntc_meas[1]);
//        USART0_write_string(debug_buffer);        
//
//
//
//        //UART2_Write(1);
//        // Process Modbus data
//        process_modbus_data();
//        IO_PF6_Toggle();
//        IO_PF5_Toggle();
//        MEASURE_LED_nSET();
//
//        PORTF.OUTSET = (1 << 6);   // Drive high
//        _delay_ms(1000);
//        PORTF.OUTCLR = (1 << 6);   // Drive low
//
//
//        _delay_ms(2000); // Short delay
////    }