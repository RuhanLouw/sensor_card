
#include "ds18b20.h"
#include "../mcc_generated_files/uart/usart1.h" // MCC-generated drivers
#define F_CPU 16000000UL
#include <util/delay.h> // For _delay_us()
#include "../mcc_generated_files/timer/tcb1.h"
#include "definitions.h"
// Conversion states
static volatile ds18b20_state_t state1 = DS18B20_STATE_IDLE; // Sensor 1 (PA3)
static volatile ds18b20_state_t state2 = DS18B20_STATE_IDLE; // Sensor 2 (PA4)

// NOW USING TCB1
// Initialize for 750ms non-blocking delay
void ds18b20_CallbackRegister(void){
    if (state1 == DS18B20_STATE_CONVERTING) state1 = DS18B20_STATE_READY;    
    if (state2 == DS18B20_STATE_CONVERTING) state2 = DS18B20_STATE_READY;
    TCB1.INTFLAGS = TCB_CAPT_bm;    // Clear flag
//    TCB1_CAPTInterruptDisable();
//    TCB1_Stop();
//    TCB1_CounterSet(0);
    ERROR_LED_TOGGLE();
}


void ds18b20_Init(void) {
    //TCB1
    TCB1_CaptureCallbackRegister(ds18b20_CallbackRegister);
    ds18b20_init_pin1();
    ds18b20_init_pin2();
//    TCB1_CAPTInterruptEnable();
}

// Initialize DS18B20 pins
void ds18b20_init_pin1(void) {
    DS18B20_PORT1.DIRSET = DS18B20_PIN1_bm; // PA3 output
    DS18B20_PORT1.OUTSET = DS18B20_PIN1_bm; // High (idle)
}

void ds18b20_init_pin2(void) {
    DS18B20_PORT2.DIRSET = DS18B20_PIN2_bm; // PA4 output
    DS18B20_PORT2.OUTSET = DS18B20_PIN2_bm; // High (idle)
}

// Reset for Sensor 1 (PA3)
ds18b20_error_t ds18b20_reset1(void) {
    DS18B20_PORT1.DIRSET = DS18B20_PIN1_bm; // Output
    DS18B20_PORT1.OUTCLR = DS18B20_PIN1_bm; // Low 480us
    _delay_us(480);
    DS18B20_PORT1.DIRCLR = DS18B20_PIN1_bm; // Input
    _delay_us(70);
    if (DS18B20_PORT1.IN & DS18B20_PIN1_bm) {
        _delay_us(410);
        return DS18B20_NO_DEVICE;
    }
    _delay_us(410);
    if (!(DS18B20_PORT1.IN & DS18B20_PIN1_bm)) {
        return DS18B20_TIMEOUT;
    }
    return DS18B20_OK;
}

// Reset for Sensor 2 (PA4)
ds18b20_error_t ds18b20_reset2(void) {
    DS18B20_PORT2.DIRSET = DS18B20_PIN2_bm; // Output
    DS18B20_PORT2.OUTCLR = DS18B20_PIN2_bm; // Low 480us
    _delay_us(480);
    DS18B20_PORT2.DIRCLR = DS18B20_PIN2_bm; // Input
    _delay_us(70);
    if (DS18B20_PORT2.IN & DS18B20_PIN2_bm) {
        _delay_us(410);
        return DS18B20_NO_DEVICE;
    }
    _delay_us(410);
    if (!(DS18B20_PORT2.IN & DS18B20_PIN2_bm)) {
        return DS18B20_TIMEOUT;
    }
    return DS18B20_OK;
}

// Write bit for Sensor 1 (PA3)
void ds18b20_write_bit1(uint8_t bit) {
    DS18B20_PORT1.DIRSET = DS18B20_PIN1_bm; // Output
    DS18B20_PORT1.OUTCLR = DS18B20_PIN1_bm; // Low
    if (bit) {
        _delay_us(6); // 1: Low 6us
        DS18B20_PORT1.OUTSET = DS18B20_PIN1_bm; // High
        _delay_us(64); // Complete slot
    } else {
        _delay_us(60); // 0: Low 60us
        DS18B20_PORT1.OUTSET = DS18B20_PIN1_bm; // High
        _delay_us(10); // Recovery
    }
}

// Write bit for Sensor 2 (PA4)
void ds18b20_write_bit2(uint8_t bit) {
    DS18B20_PORT2.DIRSET = DS18B20_PIN2_bm; // Output
    DS18B20_PORT2.OUTCLR = DS18B20_PIN2_bm; // Low
    if (bit) {
        _delay_us(6); // 1: Low 6us
        DS18B20_PORT2.OUTSET = DS18B20_PIN2_bm; // High
        _delay_us(64); // Complete slot
    } else {
        _delay_us(60); // 0: Low 60us
        DS18B20_PORT2.OUTSET = DS18B20_PIN2_bm; // High
        _delay_us(10); // Recovery
    }
}

// Read bit for Sensor 1 (PA3)
uint8_t ds18b20_read_bit1(void) {
    uint8_t bit;
    DS18B20_PORT1.DIRSET = DS18B20_PIN1_bm; // Output
    DS18B20_PORT1.OUTCLR = DS18B20_PIN1_bm; // Low 6us
    _delay_us(3);
    DS18B20_PORT1.DIRCLR = DS18B20_PIN1_bm; // Input
    _delay_us(10);
    bit = (DS18B20_PORT1.IN & DS18B20_PIN1_bm) ? 1 : 0;
    _delay_us(47); // Complete slot
    return bit;
}

// Read bit for Sensor 2 (PA4)
uint8_t ds18b20_read_bit2(void) {
    uint8_t bit;
    DS18B20_PORT2.DIRSET = DS18B20_PIN2_bm; // Output
    DS18B20_PORT2.OUTCLR = DS18B20_PIN2_bm; // Low 6us
    _delay_us(3);
    DS18B20_PORT2.DIRCLR = DS18B20_PIN2_bm; // Input
    _delay_us(10);
    bit = (DS18B20_PORT2.IN & DS18B20_PIN2_bm) ? 1 : 0;
    _delay_us(47); // Complete slot
    return bit;
}

// Write byte for Sensor 1 (PA3)
void ds18b20_write_byte1(uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        ds18b20_write_bit1(byte & 1);
        byte >>= 1;
    }
}

// Write byte for Sensor 2 (PA4)
void ds18b20_write_byte2(uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        ds18b20_write_bit2(byte & 1);
        byte >>= 1;
    }
}

// Read byte for Sensor 1 (PA3)
uint8_t ds18b20_read_byte1(void) {
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
        byte >>= 1;
        if (ds18b20_read_bit1()) byte |= 0x80;
    }
    return byte;
}

// Read byte for Sensor 2 (PA4)
uint8_t ds18b20_read_byte2(void) {
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
        byte >>= 1;
        if (ds18b20_read_bit2()) byte |= 0x80;
    }
    return byte;
}

// CRC calculation (shared)
static uint8_t ds18b20_crc8(const uint8_t *data, uint8_t len) {
    uint8_t crc = 0;
    for (uint8_t i = 0; i < len; i++) {
        uint8_t byte = data[i];
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t mix = (crc ^ byte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C; // Polynomial x^8 + x^5 + x^4 + 1
            byte >>= 1;
        }
    }
    return crc;
}
// Start conversion for Sensor 1 (PA3)
ds18b20_error_t ds18b20_start_conversion(void) {
    // Sensor 1
    if (state1 == DS18B20_STATE_IDLE) {
        if (ds18b20_reset1() == DS18B20_OK) {
            ds18b20_write_byte1(0xCC);
            ds18b20_write_byte1(0x44);
            state1 = DS18B20_STATE_CONVERTING;
        }
    }
    // Sensor 2
    if (state2 == DS18B20_STATE_IDLE) {
        if (ds18b20_reset2() == DS18B20_OK) {
            ds18b20_write_byte2(0xCC);
            ds18b20_write_byte2(0x44);
            state2 = DS18B20_STATE_CONVERTING;
        }
    }
    // Only start timer if at least one sensor is converting
    if (state1 == DS18B20_STATE_CONVERTING || state2 == DS18B20_STATE_CONVERTING) {
        TCB1_CounterSet(0);
        TCB1_CAPTInterruptEnable();
        TCB1_Start();
    }

    return DS18B20_OK;
}

// Start conversion for Sensor 2 (PA4)
//ds18b20_error_t ds18b20_start_conversion2(void) {
////f
//}

// Check conversion status for Sensor 1 (PA3)
ds18b20_state_t ds18b20_check_conversion1(void) {
    return state1;
}

// Check conversion status for Sensor 2 (PA4)
ds18b20_state_t ds18b20_check_conversion2(void) {
    return state2;
}

// Read temperature for Sensor 1 (PA3)
ds18b20_error_t ds18b20_read_temp1(float *temp_celsius) {
    if (state1 != DS18B20_STATE_READY) return DS18B20_TIMEOUT; // Not ready
    ds18b20_error_t status = ds18b20_reset1();
    if (status != DS18B20_OK) return status;
    ds18b20_write_byte1(0xCC); // Skip ROM
    ds18b20_write_byte1(0xBE); // Read scratchpad
    uint8_t scratchpad[9];
    for (uint8_t i = 0; i < 9; i++) scratchpad[i] = ds18b20_read_byte1();
    if (ds18b20_crc8(scratchpad, 8) != scratchpad[8]) {
        state1 = DS18B20_STATE_IDLE;
        return DS18B20_CRC_ERROR;
    }
    int16_t raw_temp = (scratchpad[1] << 8) | scratchpad[0];
    *temp_celsius = (float)raw_temp / 16.0; // 0.0625°C per LSB
    state1 = DS18B20_STATE_IDLE;
    return DS18B20_OK;
}

// Read temperature for Sensor 2 (PA4)
ds18b20_error_t ds18b20_read_temp2(float *temp_celsius) {
    if (state2 != DS18B20_STATE_READY) return DS18B20_TIMEOUT; // Not ready
    ds18b20_error_t status = ds18b20_reset2();
    if (status != DS18B20_OK) return status;
    ds18b20_write_byte2(0xCC); // Skip ROM
    ds18b20_write_byte2(0xBE); // Read scratchpad
    uint8_t scratchpad[9];
    for (uint8_t i = 0; i < 9; i++) {
        scratchpad[i] = ds18b20_read_byte2();
    }
    if (ds18b20_crc8(scratchpad, 8) != scratchpad[8]) {
        state2 = DS18B20_STATE_IDLE;
        return DS18B20_CRC_ERROR;
    }
    int16_t raw_temp = (scratchpad[1] << 8) | scratchpad[0];
    *temp_celsius = (float)raw_temp / 16.0; // 0.0625°C per LSB
    state2 = DS18B20_STATE_IDLE;
    return DS18B20_OK;
}

DS18B20_SENSOR_t read_ds18b20(uint8_t sensor_number){
    DS18B20_SENSOR_t buffer;
    ds18b20_error_t ds18b20;
    float temp;
switch (sensor_number) {
    case 1: 
        ds18b20 = ds18b20_read_temp1(&temp);
        break;
    case 2: 
        ds18b20 = ds18b20_read_temp2(&temp);
        break;
    default:
        buffer.error = DS18B20_NO_DEVICE;
        ERROR_LED_SET();
        return buffer;
}

//    TCB1_CAPTInterruptEnable();
//    TCB1_CounterSet(0);
//    TCB1_Start();
    
    if(ds18b20 != DS18B20_OK){
        buffer.error = ds18b20;
        ERROR_LED_SET();
        return buffer;
    }
    buffer.temp = (int16_t) temp*10;
    buffer.error = DS18B20_OK;

    return buffer;
}

ds18b20_state_t DS18B201_check_state(void){
    return state1;
}
ds18b20_state_t DS18B202_check_state(void){
    return state2;
}

