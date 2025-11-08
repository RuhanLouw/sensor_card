/* 
 * File:   ds18b20.h
 * Author: Ruhan Louw
 *
 * Created on August 28, 2025, 2:34 PM
 */

#ifndef DS18B20_H
#define	DS18B20_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include "../functions/temp_sensors.h"

// Pin configuration for DS18B20s
#define DS18B20_PORT1 PORTA
#define DS18B20_PIN1_bm PIN2_bm // PA3 (Sensor 1)
#define DS18B20_PORT2 PORTA
#define DS18B20_PIN2_bm PIN3_bm // PA4 (Sensor 2, confirm pin)

// Error codes
typedef enum {
    DS18B20_OK = 0,
    DS18B20_NO_DEVICE = 1,
    DS18B20_TIMEOUT = 2,
    DS18B20_CRC_ERROR = 3
}ds18b20_error_t;

// Conversion state for each sensor
typedef enum {
    DS18B20_STATE_IDLE = 0,
    DS18B20_STATE_CONVERTING,
    DS18B20_STATE_READY
}ds18b20_state_t;

typedef struct {
    uint16_t temp;       // 0.1°C
    ds18b20_error_t error;     // Error bits
}DS18B20_SENSOR_t;

// Function prototypes for Sensor 1 (PA3)


void ds18b20_Init(void);

void ds18b20_init_pin1(void);
ds18b20_error_t ds18b20_reset1(void);
void ds18b20_write_bit1(uint8_t bit);
uint8_t ds18b20_read_bit1(void);
void ds18b20_write_byte1(uint8_t byte);
uint8_t ds18b20_read_byte1(void);

ds18b20_error_t ds18b20_start_conversion(void);

ds18b20_state_t ds18b20_check_conversion1(void);
ds18b20_error_t ds18b20_read_temp1(float *temp_celsius);

// Function prototypes for Sensor 2 (PA4)
void ds18b20_init_pin2(void);
ds18b20_error_t ds18b20_reset2(void);
void ds18b20_write_bit2(uint8_t bit);
uint8_t ds18b20_read_bit2(void);
void ds18b20_write_byte2(uint8_t byte);
uint8_t ds18b20_read_byte2(void);

ds18b20_state_t ds18b20_check_conversion2(void);
ds18b20_error_t ds18b20_read_temp2(float *temp_celsius);

// User Functions
DS18B20_SENSOR_t read_ds18b20(uint8_t sensor_number);
ds18b20_state_t DS18B201_check_state(void);
ds18b20_state_t DS18B202_check_state(void);

void ds18b20_CallbackRegister(void);

#ifdef	__cplusplus
}
#endif

#endif	/* DS18B20_H */

