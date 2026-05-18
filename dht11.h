/* 
 * File:   dht11.h
 * Author: Ruhan Louw
 *
 * Created on November 25, 2025, 6:04 PM
 */

#ifndef DHT11_H
#define	DHT11_H

#ifdef	__cplusplus
extern "C" {
#endif


#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

/* ============================================================
 * CONFIGURATION
 * ============================================================ */
#define DHT11_PORT      PORTA
#define DHT11_PIN       7
#define DHT11_VPORT     VPORTA

/* ============================================================
 * DHT11 STATE MACHINE
 * ============================================================ */
typedef enum {
    DHT11_IDLE = 0,         // Ready to start new measurement
    DHT11_BUSY,             // Currently reading sensor
    DHT11_READ_READY,       // Data available to read
    DHT11_ERROR             // Communication error
} DHT11_State_t;

/* ============================================================
 * DHT11 ERROR CODES
 * ============================================================ */
#define DHT11_OK                0x0000
#define DHT11_ERROR_TIMEOUT     0x0001  // No response from sensor
#define DHT11_ERROR_CHECKSUM    0x0002  // Checksum mismatch
#define DHT11_ERROR_TOO_SOON    0x0004  // Measurement requested too soon
#define DHT11_ERROR_NO_DATA     0x0008  // No valid data available

/* ============================================================
 * DHT11 SENSOR STRUCTURE
 * ============================================================ */
typedef struct {
    int16_t temp;           // Temperature in tenths of °C (e.g., 235 = 23.5°C)
    int16_t humidity;       // Humidity in tenths of % (e.g., 650 = 65.0%)
    uint16_t error;         // Error flags
} DHT11_SENSOR_t;

/* ============================================================
 * FUNCTION PROTOTYPES
 * ============================================================ */

/**
 * @brief Initialize DHT11 sensor and pin configuration
 */
void DHT11_Init(void);

/**
 * @brief Start a new DHT11 measurement (non-blocking trigger)
 * @return 0 on success, error code if too soon since last read
 */
uint16_t DHT11_StartMeasurement(void);

/**
 * @brief Get current state of DHT11 state machine
 * @return Current DHT11_State_t
 */
DHT11_State_t DHT11_STATE(void);

/**
 * @brief Read DHT11 sensor data (call after DHT11_StartMeasurement)
 * @return DHT11_SENSOR_t structure with temperature, humidity, and error status
 */
DHT11_SENSOR_t read_dht11(void);

/**
 * @brief 1 Hz tick function for timing enforcement (call from 1s timer)
 */
void DHT11_Tick_1Hz(void);

/* ============================================================
 * TIMING MACROS (for 16 MHz clock)
 * ============================================================ */
#define DHT11_DELAY_MS(ms)      _delay_ms(ms)
#define DHT11_DELAY_US(us)      _delay_us(us)

/* ============================================================
 * PIN CONTROL MACROS
 * ============================================================ */
#define DHT11_PIN_OUTPUT()      (DHT11_PORT.DIRSET = (1 << DHT11_PIN))
#define DHT11_PIN_INPUT()       (DHT11_PORT.DIRCLR = (1 << DHT11_PIN))
#define DHT11_PIN_LOW()         (DHT11_VPORT.OUT &= ~(1 << DHT11_PIN))
#define DHT11_PIN_HIGH()        (DHT11_VPORT.OUT |= (1 << DHT11_PIN))
#define DHT11_PIN_READ()        (DHT11_VPORT.IN & (1 << DHT11_PIN))


#ifdef	__cplusplus
}
#endif

#endif	/* DHT11_H */

