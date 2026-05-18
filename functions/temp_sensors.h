/* 
 * File:   temp_sensors.h
 * Author: Ruhan Louw
 *
 * Created on June 30, 2025, 9:28 AM
 */

#ifndef TEMP_SENSORS_H
#define	TEMP_SENSORS_H


#include "../mcc_generated_files/system/pins.h"
#include "../mcc_generated_files/system/port.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../functions/ds18b20.h"

#define V_REF 3.0f        // Reference voltage (V)
#define devider_12b 4096.0f // Reference to ADC V_ref's devidable
#define R_FIXED 10000.0f  // Fixed resistor (ohms)
#define R_25 10000.0f     // NTC resistance at 25°C (ohms)
#define BETA 3977.0f      // Beta value (K)
#define T_25 298.15f      // 25C in Kelvin

#define Antc_PORT PORTC
#define Antc_PIN_bm PIN2_bm
#define Antc_PIN_bp PIN2_bp
#define Bntc_PORT PORTF
#define Bntc_PIN_bm PIN5_bm
#define Bntc_PIN_bp PIN5_bp
#define Cntc_PORT PORTF
#define Cntc_PIN_bm PIN4_bm
#define Cntc_PIN_bp PIN4_bp

#define _A() IO_PC2_SetHigh()
#define _B() IO_PF5_SetHigh()
#define _C() IO_PF4_SetHigh()
#define _nA() IO_PC2_SetLow()
#define _nB() IO_PF5_SetLow()
#define _nC() IO_PF4_SetLow()

#define SCK_PORT PORTA   
#define SCK_PIN_bm  PIN6_bm    
#define SCK_PIN_bp  PIN6_bp    

#define MISO_PORT PORTA
#define MISO_PIN_bm PIN5_bm
#define MISO_PIN_bp PIN5_bp
#define READ_MISO() IO_PA5_GetValue()

#define CLK_PORT PORTA
#define CLK_PIN_bm PIN6_bm
#define CLK_PIN_bp PIN6_bp
#define SCK_HIGH()  IO_PA6_SetHigh()
#define SCK_LOW()   IO_PA6_SetLow()

#define EN_NTC_PORT PORTD
#define EN_NTC_PIN_bm PIN7_bm
#define EN_NTC_PIN_bp PIN7_bp
#define ENABLE_NTC_MUX() IO_PD7_SetHigh()
#define DISABLE_NTC_MUX() IO_PD7_SetLow()

#define ENABLE_KTYPE_PIN() IO_PD1_SetLow();
#define DISABLE_KTYPE_PIN() IO_PD1_SetHigh();

typedef enum{
    KTYPE_IDLE = 0,
    KTYPE_CONVERTING,
    KTYPE_READ_READY,
    KTYPE_TIMEOUT
}KTYPE_STATE_t;

typedef enum{
    KTYPE_OK = 0,
    KTYPE_SCV,
    KTYPE_SCG,
    KTYPE_OC
}KTYPE_ERROR_t;

typedef struct {
    int16_t temp;      // 0.1 °C
    uint8_t error;    // NTC error bits
} NTC_SENSOR_t;

typedef struct {
    int16_t temp;          // Thermocouple temp, 0.1°C
    int16_t cold_junction; // Cold junction temp, 0.1°C
    KTYPE_ERROR_t error;        // Error bits
} KTYPE_SENSOR_t;


void tempSensors_init(void);
void CS_NTC(uint8_t ntc_num);
uint16_t mcp3201_read_bitbang(uint8_t ntc_num);
uint16_t readRaw_NTC(uint8_t ntc_num);
float readAvg_NTC(uint8_t ntc_num, uint8_t num_reads);
float getTemp_NTC(uint8_t ntc_num, uint8_t num_reads);

NTC_SENSOR_t read_ntc(uint8_t ntc_number, uint8_t ntc_poll_number);



KTYPE_STATE_t KTYPE_start_conversion(void);
KTYPE_STATE_t KTYPE_check_state(void);
void KTYPE_bitbang(uint8_t *buffer);
KTYPE_ERROR_t readKTypeSensor(float *thermo, float *junc);
KTYPE_SENSOR_t read_ktype(void);
void KTYPE_Init(void);

void KTYPE_timer_CapCallBack(void);

int16_t get_chamber_temp(void);
int16_t get_superheat_temp(void);
int16_t get_subcool_temp(void);

#ifdef	__cplusplus
extern "C" {
#endif
    

#ifdef	__cplusplus
}
#endif

#endif	/* TEMP_SENSORS_H */

//// Pin definitions for multiplexer (SN74HC138, 3-to-8 decoder)
//#define MUX_PORT PORTF
//#define MUX_A PIN6_bm // PA5 (A0 input)
//#define MUX_B PIN5_bm // PA6 (A1 input)
//#define MUX_C PIN4_bm // PA7 (A2 input)



//#define READ_SINGLE 0
//#define READ_ALL 1
    
//#define TEMP_SENSOR_COUNT 9
//#define TEMP_SENSOR_1 0
//#define TEMP_SENSOR_2 1
//#define TEMP_SENSOR_3 2
//#define TEMP_SENSOR_4 3
//#define TEMP_SENSOR_5 4
//#define TEMP_SENSOR_6 5
//#define TEMP_SENSOR_7 6
//#define TEMP_SENSOR_8 7
//#define K_TYPE 8    

//#define ntc_1 0
//#define ntc_2 1
//#define ntc_3 2
//#define ntc_4 3
//#define ntc_5 4
//#define ntc_6 5
//#define ntc_7 6
//#define ntc_8 7
//#define ktype_9 8
