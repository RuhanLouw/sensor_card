/*
 * DHT11 Temperature and Humidity Sensor Library
 * 
 * @file dht11.c
 * @brief Implementation of DHT11 single-wire protocol
 * 
 * Communication Protocol:
 * 1. MCU pulls line LOW for >18ms (start signal)
 * 2. MCU releases line, waits for DHT11 response
 * 3. DHT11 pulls LOW for 80us, then HIGH for 80us
 * 4. DHT11 sends 40 bits: 8-bit humidity int + 8-bit humidity dec + 
 *    8-bit temp int + 8-bit temp dec + 8-bit checksum
 * 5. Each bit: 50us LOW, then 26-28us HIGH (0) or 70us HIGH (1)
 */

#include "dht11.h"
#include <avr/interrupt.h>

/* ============================================================
 * PRIVATE VARIABLES
 * ============================================================ */
static volatile DHT11_State_t dht11_state = DHT11_IDLE;
static volatile uint8_t seconds_since_read = 255;  // Allow immediate first read
static uint8_t dht11_data[5] = {0};  // 40 bits = 5 bytes

/* ============================================================
 * PRIVATE FUNCTION PROTOTYPES
 * ============================================================ */
static uint16_t DHT11_ReadByte(void);
static uint8_t DHT11_WaitForPinState(uint8_t state, uint16_t timeout_us);

/* ============================================================
 * FUNCTION: Initialize DHT11
 * ============================================================ */
void DHT11_Init(void) {
    // Configure pin as output, initially HIGH
    DHT11_PIN_HIGH();
    DHT11_PIN_OUTPUT();
    
    // Enable pull-up resistor
    DHT11_PORT.PIN7CTRL = PORT_PULLUPEN_bm;
    
    dht11_state = DHT11_IDLE;
    seconds_since_read = 255;  // Allow immediate read
}

/* ============================================================
 * FUNCTION: 1Hz Tick (call from timer ISR)
 * ============================================================ */
void DHT11_Tick_1Hz(void) {
    if (seconds_since_read < 255) {
        seconds_since_read++;
    }
    
    // Auto-trigger measurement every 2 seconds when idle
    if (dht11_state == DHT11_IDLE && seconds_since_read >= 2) {
        DHT11_StartMeasurement();
    }
}

/* ============================================================
 * FUNCTION: Get Current State
 * ============================================================ */
DHT11_State_t DHT11_STATE(void) {
    return dht11_state;
}

/* ============================================================
 * FUNCTION: Start Measurement
 * ============================================================ */
uint16_t DHT11_StartMeasurement(void) {
    // DHT11 requires minimum 1 second between readings
    if (seconds_since_read < 1) {
        return DHT11_ERROR_TOO_SOON;
    }
    
    dht11_state = DHT11_BUSY;
    seconds_since_read = 0;
    
    return DHT11_OK;
}

/* ============================================================
 * FUNCTION: Wait for Pin State with Timeout
 * @return 1 if successful, 0 if timeout
 * ============================================================ */
static uint8_t DHT11_WaitForPinState(uint8_t state, uint16_t timeout_us) {
    uint16_t count = 0;
    
    while (((DHT11_PIN_READ() > 0) != (state > 0)) && count < timeout_us) {
        _delay_us(1);
        count++;
    }
    
    return (count < timeout_us) ? 1 : 0;
}

/* ============================================================
 * FUNCTION: Read Single Byte (8 bits) from DHT11
 * @return Byte value, or error code in high byte
 * ============================================================ */
static uint16_t DHT11_ReadByte(void) {
    uint8_t byte_val = 0;
    
    for (uint8_t i = 0; i < 8; i++) {
        // Wait for LOW period (50us nominal)
        if (!DHT11_WaitForPinState(0, 70)) {
            return 0xFF00 | DHT11_ERROR_TIMEOUT;
        }
        
        // Wait for HIGH period
        if (!DHT11_WaitForPinState(1, 70)) {
            return 0xFF00 | DHT11_ERROR_TIMEOUT;
        }
        
        // Measure HIGH pulse width
        // ~28us = '0', ~70us = '1'
        // Sample at 40us to discriminate
        _delay_us(40);
        
        byte_val <<= 1;
        if (DHT11_PIN_READ()) {
            byte_val |= 1;  // Still HIGH = '1' bit
        }
        
        // Wait for bit to finish
        DHT11_WaitForPinState(0, 100);
    }
    
    return byte_val;
}

/* ============================================================
 * FUNCTION: Read DHT11 Sensor
 * ============================================================ */
DHT11_SENSOR_t read_dht11(void) {
    DHT11_SENSOR_t result = {0, 0, DHT11_OK};
    uint16_t byte_result;
    uint8_t checksum;
    
    // Check if measurement was started
    if (dht11_state != DHT11_BUSY) {
        result.error = DHT11_ERROR_NO_DATA;
        return result;
    }
    
    /* ========================================
     * STEP 1: Send Start Signal (>18ms LOW)
     * Interrupts OK during this long delay
     * ======================================== */
    DHT11_PIN_OUTPUT();
    DHT11_PIN_LOW();
    DHT11_DELAY_MS(20);  // 20ms LOW pulse
    
    /* ========================================
     * STEP 2: Release line and switch to input
     * ======================================== */
    DHT11_PIN_HIGH();
    DHT11_PIN_INPUT();
    DHT11_DELAY_US(40);  // Wait 20-40us for DHT11 to take control
    
    // Disable interrupts ONLY for timing-critical bit reading
    uint8_t sreg = SREG;
    cli();
    
    /* ========================================
     * STEP 3: Wait for DHT11 response (80us LOW)
     * ======================================== */
    if (!DHT11_WaitForPinState(0, 100)) {
        result.error = DHT11_ERROR_TIMEOUT;
        dht11_state = DHT11_ERROR;
        SREG = sreg;
        return result;
    }
    
    /* ========================================
     * STEP 4: Wait for DHT11 ready signal (80us HIGH)
     * ======================================== */
    if (!DHT11_WaitForPinState(1, 100)) {
        result.error = DHT11_ERROR_TIMEOUT;
        dht11_state = DHT11_ERROR;
        SREG = sreg;
        return result;
    }
    
    /* ========================================
     * STEP 5: Read 40 bits (5 bytes) of data
     * This takes ~5ms total - keep interrupts disabled
     * ======================================== */
    for (uint8_t i = 0; i < 5; i++) {
        byte_result = DHT11_ReadByte();
        
        // Check for timeout error
        if (byte_result & 0xFF00) {
            result.error = byte_result & 0x00FF;
            dht11_state = DHT11_ERROR;
            SREG = sreg;
            return result;
        }
        
        dht11_data[i] = (uint8_t)(byte_result & 0xFF);
    }
    
    // Re-enable interrupts immediately after bit reading
    SREG = sreg;
    
    /* ========================================
     * STEP 6: Verify Checksum
     * ======================================== */
    checksum = dht11_data[0] + dht11_data[1] + dht11_data[2] + dht11_data[3];
    
    if (checksum != dht11_data[4]) {
        result.error = DHT11_ERROR_CHECKSUM;
        dht11_state = DHT11_ERROR;
        return result;
    }
    
    /* ========================================
     * STEP 7: Parse Data
     * DHT11 format: [RH_int][RH_dec][Temp_int][Temp_dec][Checksum]
     * Note: DHT11 decimal bytes are typically 0
     * ======================================== */
    result.humidity = (int16_t)dht11_data[0] * 10 + dht11_data[1];
    result.temp = (int16_t)dht11_data[2] * 10 + dht11_data[3];
    
    // Handle negative temperatures (MSB of temp_int indicates sign)
    if (dht11_data[2] & 0x80) {
        result.temp = -((int16_t)(dht11_data[2] & 0x7F) * 10 + dht11_data[3]);
    }
    
    result.error = DHT11_OK;
    dht11_state = DHT11_READ_READY;
    
    return result;
}

/* ============================================================
 * END OF FILE
 * ============================================================ */