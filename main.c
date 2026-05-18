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
#include "functions/definitions_sensor.h"
#include "functions/temp_sensors.h"
//#include "functions/system_cmd.h"
//#include "functions/ds18b20.h"
#include "DS.h"
#include "dht11.h"
#include "functions/debug_uart2.h"
#include "functions/system_registers.h"
#include "functions/modbus.h"
#include <stdio.h>
/*
    Main application
*/
/* ============================================================
 * GLOBAL SENSOR INSTANCES
 * ============================================================ */

uint8_t ntc_samples = 20; // Set the number of samples to avg for the NTC sensors

// System type
typedef struct {
    NTC_SENSOR_t ntcs[8];
    KTYPE_SENSOR_t ktype;
    DHT11_SENSOR_t dht11;
    DS_SENSOR_t ds18b20[2];
}SYS_SENSORS;

// System Sensors Registers
SYS_SENSORS sys_sensors;
int uptime = 0; //1s

/* ============================================================
 * FUNCTION: INITIALISE SYSTEM REGISTERS AND SENSORS
 * ============================================================ */
void _SYS_INIT(void){   
    // System info registers
    // After variable declarations, before MB_Init():
    for (int i = 0; i < SREG_COUNT; i++) sys_regs[i] = 0;
    REG(SREG_FIRMWARE) = 100; // v1.00
    REG(SREG_UPTIME_L) = 0;         // To be filled from timer
    REG(SREG_UPTIME_H) = 0;
    
    // --- SET ---
    REG(SREG_STATUS) = STATUS_POLL_ACTIVE | STATUS_SYSTEM_READY;
    REG(SREG_SENSOR_ENABLE) = (
                ENABLE_NTC1
            |   ENABLE_NTC2
            |   ENABLE_NTC3
            |   ENABLE_NTC4
            |   ENABLE_NTC5
            |   ENABLE_NTC6
            |   ENABLE_NTC7
            |   ENABLE_NTC8
            |   ENABLE_KTYPE   
//            |   ENABLE_DHT11
            |   ENABLE_DS18B20_1
            |   ENABLE_DS18B20_2
            );
    
    
    
    MB_Init();    
    KTYPE_Init();
    DS_Init();
//    DHT11_Init();
    
}

/* ============================================================
 * FUNCTION: Update Modbus Registers from Sensor Structs
 * ============================================================ */
void update_sensor_registers(void) {

    // NTC sensors (use i, temp + error addr = 32)
    for (int i = 0; i < 8; i++) {
        sys_regs[SREG_NTC1_TEMP + i] = sys_sensors.ntcs[i].temp;
        sys_regs[SREG_NTC_ERROR_BITS] |= sys_sensors.ntcs[i].error << i;
    }
    
    // K-type thermocouple
    sys_regs[SREG_KTYPE_TEMP] = sys_sensors.ktype.temp;
    sys_regs[SREG_KTYPE_CJ_TEMP] = sys_sensors.ktype.cold_junction;
    sys_regs[SREG_KTYPE_ERROR] = sys_sensors.ktype.error;

    // DHT22
    sys_regs[SREG_DHT22_TEMP] = sys_sensors.dht11.temp;
    sys_regs[SREG_DHT22_HUMIDITY] = sys_sensors.dht11.humidity;
    sys_regs[SREG_DHT22_ERROR] = sys_sensors.dht11.error;

    // DS18B20
    sys_regs[SREG_DS18B20_1_TEMP] = sys_sensors.ds18b20[0].temp;
    sys_regs[SREG_DS18B20_ERROR1] = sys_sensors.ds18b20[0].error;
    sys_regs[SREG_DS18B20_2_TEMP] = sys_sensors.ds18b20[1].temp;
    sys_regs[SREG_DS18B20_ERROR2] = sys_sensors.ds18b20[1].error;
    
    // Main System Temperatures
    sys_regs[SREG_CHAMBER_INTERNAL] = get_chamber_temp();
    sys_regs[SREG_HEATING_ELEMENT] = sys_sensors.ktype.temp;
    sys_regs[SREG_EVAPORATOR] = sys_sensors.ds18b20[1].temp;
    sys_regs[SREG_SUPERHEAT] = get_superheat_temp();
    sys_regs[SREG_SUBCOOLING] = get_subcool_temp();
    sys_regs[SREG_EXTERNAL] = sys_sensors.ds18b20[0].temp;
}

/* ============================================================
 * FUNCTION: Update Sensor Data 
 * ============================================================ */
void poll_sensors(void) {
    if (sys_regs[SREG_STATUS] & STATUS_POLL_ACTIVE) {
        MEASURE_LED_SET();
        
        // ======================================================
        // Poll NTC sensors (1 to 8)
        // ======================================================
        for (uint8_t i = 1; i <= 8; i++) {
            if (sys_regs[SREG_SENSOR_ENABLE] & (ENABLE_NTC1 << (i-1))) {
                sys_sensors.ntcs[i-1] = read_ntc(i, ntc_samples);
//                printf("%d ", sys_sensors.ntcs[i-1].temp);
            }
//            printf("\n");
        }
        
        // ======================================================
        // Poll K-Type thermocouple
        // ======================================================
        if ((KTYPE_check_state() == KTYPE_READ_READY) &&
            (sys_regs[SREG_SENSOR_ENABLE] & ENABLE_KTYPE)) {
            sys_sensors.ktype = read_ktype(); 
            printf("ktype %d \n", sys_sensors.ktype.temp);
        }
        // ======================================================
        // DS18B20 ? both sensors, one single state
        // ======================================================
        if (DS_GetSystemState() == DS_SYSTEM_READY) {

            if (sys_regs[SREG_SENSOR_ENABLE] & ENABLE_DS18B20_1) {
                sys_sensors.ds18b20[0] = DS_ReadSensor1();
                printf("ds1 %d \n", sys_sensors.ds18b20[0].temp);
            }
            if (sys_regs[SREG_SENSOR_ENABLE] & ENABLE_DS18B20_2) {
                sys_sensors.ds18b20[1] = DS_ReadSensor2();
                printf("ds2 %d \n", sys_sensors.ds18b20[1].temp);
            }

            // We have read both ? start next conversion cycle
            ds_system_state = DS_SYSTEM_IDLE;   // optional, but clean
            DS_StartConversion();
        }
        // ======================================================
        // DHT11 Poll   --->>> NOT a DHT22! damn chinese...
        // ======================================================
//        if(DHT11_STATE() == DHT11_READ_READY &&
//            (sys_regs[SREG_SENSOR_ENABLE] & ENABLE_DHT11)){
//            sys_sensors.dht11 = read_dht11();
//            
//            // Instant, crystal-clear debug output
//            printf("DHT22: %c%d.%d°C   %d.%d%%RH   Err:0x%04X\r\n",
//            (sys_sensors.dht11.temp < 0 ? '-' : '+'),           // sign
//            abs(sys_sensors.dht11.temp) / 10,                   // degrees
//            abs(sys_sensors.dht11.temp) % 10,                   // tenths
//            sys_sensors.dht11.humidity / 10,                    // %RH integer
//            sys_sensors.dht11.humidity % 10,                    // %RH tenths
//            sys_sensors.dht11.error);                            // error code
//        }
//        
        MEASURE_LED_nSET();
        update_sensor_registers();
    } 
}


//-------- 1s PIT -------------
void PIT(void){
    RUN_LED_TOGGLE();
    uptime++;
    REG(SREG_UPTIME_L) = uptime & 0xFFFF;
    REG(SREG_UPTIME_H) = uptime >> 16;
    
//    DHT11_Tick_1Hz();
}
/* ============================================================= */
/* =============================================================
 * MAIN LOOP
 * ============================================================= */
/* ============================================================= */
int main(void) {
    SYSTEM_Initialize();

    RTC_SetPITIsrCallback(PIT);
    _SYS_INIT();
    RS485_RX_ENABLE();

    RUN_LED_SET();
    sei();
    
    while(1) {
         modbus_process();
         poll_sensors();
    }
}






















//    char debug_buffer[256] = {0};
//    uint8_t debug_index = 0;



//        
//        sprintf(debug_buffer, "Hello");
//        printf(debug_buffer);
//        
//        _delay_ms(1000);
//
//char debug_tx[128] = {0};
//void test_crc(void) {
//    uint8_t test[6] = {0x02, 0x03, 0x00, 0x00, 0x00, 0x04};
//    uint16_t crc = modbus_crc16(test, 6);
//    
//    sprintf(debug_tx, "TEST CRC: %02X %02X | LEN=%d\r\n", 
//            crc & 0xFF, crc >> 8, 6);
//    RS485_TX_ENABLE();
//    _delay_ms(100);
//    USART1_write_string(debug_tx);
//    _delay_ms(100);
//    RS485_RX_ENABLE();
//}
//
//int main(void) {
//    SYSTEM_Initialize();
//    sei();
//    _SYS_INIT();
//    RS485_RX_ENABLE();
//
//    test_crc();  // ? ADD THIS
//
//    while(1) {
//        if (MB_IsFrameReady() && MB_GetState() == MB_STATE_IDLE) {
//            MB_EchoLastFrame();
//        }
//    }
//}

//        if(UART2_IsRxReady()){
//            debug_buffer[debug_index] = UART2_Read();
//            if(debug_buffer[debug_index] == '\n'){
//                
//            }
//        }
//
//#define DEBUG_BUFFER_SIZE 128
//
//static uint8_t debug_buffer[DEBUG_BUFFER_SIZE];
//static uint8_t debug_index = 0;
//
////void RS485_TX_ENABLE(void) { IO_PC3_SetHigh(); }
////void RS485_RX_ENABLE(void) { IO_PC3_SetLow(); }
//
//void process_echo(void) {
//    if (UART1_IsRxReady()) {
//        RX1_LED_Toggle();
//        uint8_t byte = UART1_Read();
//
//        // Store in buffer
//        if (debug_index < DEBUG_BUFFER_SIZE - 1) {
//            debug_buffer[debug_index++] = byte;
//        }
//
//        // If Enter ('\n') ? echo back entire line
//        if (byte == '\n' || byte == '\r') {
//            TX1_LED_Toggle();
//
//            // Optional: replace \r with \n for clean output
//            if (byte == '\r') debug_buffer[debug_index - 1] = '\n';
//
//            // Switch to TX
//            RS485_TX_ENABLE();
//            _delay_us(100);
//
//            // Send back all bytes
//            for (uint8_t i = 0; i < debug_index; i++) {
//                while (!UART1_IsTxReady());
//                UART1_Write(debug_buffer[i]);
//            }
//
//            // Wait for transmission
//            while (!UART1_IsTxDone());
//            _delay_us(100);
//
//            // Back to RX
//            RS485_RX_ENABLE();
//
//            // Reset buffer
//            debug_index = 0;
//        }
//    }
//}
//
//int main(void) {
//    SYSTEM_Initialize();
//    RS485_RX_ENABLE();  // Start in receive mode
//    RUN_LED_SET();
//    while (1) {
////        process_echo();
////
//        // Optional: heartbeat
////        ERROR_LED_TOGGLE();
//        if(UART1_IsRxReady()){
//            uint8_t buf = UART1_Read();
//            ERROR_LED_TOGGLE();
//            RS485_TX_ENABLE();
//            _delay_ms(1);
//            UART1_Write(buf);
//            while(!UART1_IsTxDone());
//            _delay_ms(1);
//            RS485_RX_ENABLE();
//        }
//        
////        debug1_send_string("a");
//        
////        _delay_ms(50);
//    }
//}
//
//
//




//        RS485_RX_ENABLE();
//        debug1_send_string("Hello my name is Ruhan\n");
//        _delay_ms(1000);
//        sprintf(debug_buffer, "NTC5: %d\nNTC6: %d\nDS1: %d ERROR: %d\nDS1: %d ERROR: %d\n\n",
//                (int)sys_regs[MB_REG_NTC5_TEMP],
//                (int)sys_regs[MB_REG_NTC6_TEMP],
//                (int)sys_regs[MB_REG_DS18B20_1_TEMP],
//                (int)sys_regs[MB_REG_DS18B20_1_ERROR],
//                (int)sys_regs[MB_REG_DS18B20_2_TEMP],
//                (int)sys_regs[MB_REG_DS18B20_2_ERROR]);
//        TX1_LED_Toggle();
//        debug1_send_string(debug_buffer);
//        _delay_ms(500); // adjust polling rate
   
//        sprintf(debug_buffer, "NTC1: %d\nNTC2: %d\nNTC3: %d\nNTC4: %d\nNTC5: %d\nNTC6: %d\nNTC7: %d\nNTC8: %d\nKTYPE: %d\nKTYPE_jc: %d\nDS1: %d\nDS2 %d\n\n",
//                (int)sys_regs[MB_REG_NTC1_TEMP],
//                (int)sys_regs[MB_REG_NTC2_TEMP],
//                (int)sys_regs[MB_REG_NTC3_TEMP],
//                (int)sys_regs[MB_REG_NTC4_TEMP],
//                (int)sys_regs[MB_REG_NTC5_TEMP],
//                (int)sys_regs[MB_REG_NTC6_TEMP],
//                (int)sys_regs[MB_REG_NTC7_TEMP],
//                (int)sys_regs[MB_REG_NTC8_TEMP],
//                (int)sys_regs[MB_REG_KTYPE_TEMP],
//                (int)sys_regs[MB_REG_KTYPE_CJ_TEMP],
//                (int)sys_regs[MB_REG_DS18B20_1_TEMP],
//                (int)sys_regs[MB_REG_DS18B20_2_TEMP]);
//        
//        debug1_send_string(debug_buffer);




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