
// DS.c - Single shared state machine ? clean, simple, bullet-proof

#include "DS.h"
#include <util/delay.h>
//#include "mcc_generated_files/timer/tcb1.h"
#include "functions/system_registers.h"
#include "functions/definitions_sensor.h"

#define DS_PORT1 PORTA
#define DS_PIN1  PIN2_bm
#define DS_PORT2 PORTA
#define DS_PIN2  PIN3_bm

volatile ds_system_state_t ds_system_state = DS_SYSTEM_IDLE;

float prev_ds1_hold = 0;
bool start_ds1_hold = true;
float prev_ds2_hold = 0;
bool start_ds2_hold = true;
// -----------------------------------------------------------
// 1-Wire low-level functions 
// -----------------------------------------------------------
static void ds_write_bit(volatile PORT_t *port, uint8_t pin, uint8_t bit) {
    port->DIRSET = pin; port->OUTCLR = pin;
    if (bit) { _delay_us(6);  port->OUTSET = pin; _delay_us(64); }
    else     { _delay_us(60); port->OUTSET = pin; _delay_us(10); }
}

static uint8_t ds_read_bit(volatile PORT_t *port, uint8_t pin) {
    uint8_t bit;
    port->DIRSET = pin; port->OUTCLR = pin; _delay_us(3);
    port->DIRCLR = pin; _delay_us(10);
    bit = (port->IN & pin) ? 1 : 0;
    _delay_us(47);
    return bit;
}

static void ds_write_byte(volatile PORT_t *port, uint8_t pin, uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        ds_write_bit(port, pin, byte & 1);
        byte >>= 1;
    }
}

static uint8_t ds_read_byte(volatile PORT_t *port, uint8_t pin) {
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
        byte >>= 1;
        if (ds_read_bit(port, pin)) byte |= 0x80;
    }
    return byte;
}

static uint8_t ds_crc8(const uint8_t *data, uint8_t len) {
    uint8_t crc = 0;
    for (uint8_t i = 0; i < len; i++) {
        uint8_t b = data[i];
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t mix = (crc ^ b) & 1;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            b >>= 1;
        }
    }
    return crc;
}

static DS_error_t ds_reset(volatile PORT_t *port, uint8_t pin) {
    port->DIRSET = pin; port->OUTCLR = pin; _delay_us(480);
    port->DIRCLR = pin; _delay_us(70);
    if (port->IN & pin) { _delay_us(410); return DS_NO_DEVICE; }
    _delay_us(410);
    return (port->IN & pin) ? DS_OK : DS_TIMEOUT;
}

// -----------------------------------------------------------
// Public API
// -----------------------------------------------------------
void DS_StartConversion(void) {
    bool any_started = false;

    if (sys_regs[SREG_SENSOR_ENABLE] & ENABLE_DS18B20_1) {
        if (ds_reset(&DS_PORT1, DS_PIN1) == DS_OK) {
            ds_write_byte(&DS_PORT1, DS_PIN1, 0xCC);  // Skip ROM
            ds_write_byte(&DS_PORT1, DS_PIN1, 0x44);  // Convert T
            any_started = true;
        }
    }
    if (sys_regs[SREG_SENSOR_ENABLE] & ENABLE_DS18B20_2) {
        if (ds_reset(&DS_PORT2, DS_PIN2) == DS_OK) {
            ds_write_byte(&DS_PORT2, DS_PIN2, 0xCC);
            ds_write_byte(&DS_PORT2, DS_PIN2, 0x44);
            any_started = true;
        }
    }

    if (any_started) {
        ds_system_state = DS_SYSTEM_CONVERTING;
        TCB1.CNT = 0;
        TCB1.INTCTRL |= TCB_CAPT_bm;        // start 750?1000 ms timer
    }
}

ds_system_state_t DS_GetSystemState(void) {
    return ds_system_state;
}

static DS_SENSOR_t read_one_sensor(volatile PORT_t *port, uint8_t pin) {
    DS_SENSOR_t res = {0, DS_TIMEOUT};


    if (ds_reset(port, pin) != DS_OK) {
        res.error = DS_NO_DEVICE;
        if(pin == DS_PIN1){
            res.temp = prev_ds1_hold;
        }else{
            res.temp = prev_ds2_hold;
        }
        return res;
    }
    ds_write_byte(port, pin, 0xCC);     // Skip ROM
    ds_write_byte(port, pin, 0xBE);     // Read Scratchpad

    uint8_t data[9];
    for (uint8_t i = 0; i < 9; i++) {
        data[i] = ds_read_byte(port, pin);
    }

    if (ds_crc8(data, 8) != data[8]) {
        res.error = DS_CRC_ERROR;
        if(pin == DS_PIN1){
            res.temp = prev_ds1_hold;
        }else{
            res.temp = prev_ds2_hold;
        }
        return res;
    }

    int16_t raw = (int16_t)((data[1] << 8) | data[0]);
    
//    if (pin == DS_PIN2){
//        printf("%02X \n", raw);
//    }
    
    res.temp  = (int16_t)(raw * 6.25);      // 0.0625°C × 100
    res.error = DS_OK;
    if(pin == DS_PIN1){
        prev_ds1_hold = res.temp;
    }else{
        prev_ds2_hold = res.temp;
    }
    return res;
}

DS_SENSOR_t DS_ReadSensor1(void) {
    if (ds_system_state != DS_SYSTEM_READY) {
        DS_SENSOR_t err = {0, DS_TIMEOUT};
        return err;
    }
    
    return read_one_sensor(&DS_PORT1, DS_PIN1);
}

DS_SENSOR_t DS_ReadSensor2(void) {
    if (ds_system_state != DS_SYSTEM_READY) {
        DS_SENSOR_t err = {0, DS_TIMEOUT};
        return err;
    }
    return read_one_sensor(&DS_PORT2, DS_PIN2);
}

// Timer fires ? conversion is done
void DS_Timeout(void) {
    if (ds_system_state == DS_SYSTEM_CONVERTING) {
        ds_system_state = DS_SYSTEM_READY;
    }
    
    TCB1.INTCTRL &= ~TCB_CAPT_bm;
}

void DS_Init(void) {
    TCB1_CaptureCallbackRegister(DS_Timeout);

    // Set pins high (idle state)
    DS_PORT1.DIRSET = DS_PIN1; DS_PORT1.OUTSET = DS_PIN1;
    DS_PORT2.DIRSET = DS_PIN2; DS_PORT2.OUTSET = DS_PIN2;

    TCB1_Start();
    DS_StartConversion();       // start first conversion immediately
}

//
//// DS.c - Single shared state machine (clean & reliable)
//
//#include "DS.h"
//#include <util/delay.h>
//#include "mcc_generated_files/timer/tcb1.h"
//#include "functions/system_registers.h"
//
//#define DS_PORT1 PORTA
//#define DS_PIN1  PIN2_bm
//#define DS_PORT2 PORTA
//#define DS_PIN2  PIN3_bm
//
//static volatile ds_system_state_t ds_system_state = DS_SYSTEM_IDLE;
//
//// -----------------------------------------------------------
//// Low-level 1-Wire (unchanged ? your code is good)
//// -----------------------------------------------------------
//static void ds_write_bit(volatile PORT_t *port, uint8_t pin, uint8_t bit) {
//    port->DIRSET = pin; port->OUTCLR = pin;
//    if (bit) { _delay_us(6);  port->OUTSET = pin; _delay_us(64); }
//    else     { _delay_us(60); port->OUTSET = pin; _delay_us(10); }
//}
//static uint8_t ds_read_bit(volatile PORT_t *port, uint8_t pin) {
//    uint8_t bit;
//    port->DIRSET = pin; port->OUTCLR = pin; _delay_us(3);
//    port->DIRCLR = pin; _delay_us(10);
//    bit = (port->IN & pin) ? 1 : 0;
//    _delay_us(47);
//    return bit;
//}
//static void ds_write_byte(volatile PORT_t *port, uint8_t pin, uint8_t byte) {
//    for (uint8_t i = 0; i < 8; i++) {
//        ds_write_bit(port, pin, byte & 1);
//        byte >>= 1;
//    }
//}
//static uint8_t ds_read_byte(volatile PORT_t *port, uint8_t pin) {
//    uint8_t byte = 0;
//    for (uint8_t i = 0; i < 8; i++) {
//        byte >>= 1;
//        if (ds_read_bit(port, pin)) byte |= 0x80;
//    }
//    return byte;
//}
//static uint8_t ds_crc8(const uint8_t *data, uint8_t len) {
//    uint8_t crc = 0;
//    for (uint8_t i = 0; i < len; i++) {
//        uint8_t b = data[i];
//        for (uint8_t j = 0; j < 8; j++) {
//            uint8_t mix = (crc ^ b) & 1;
//            crc >>= 1;
//            if (mix) crc ^= 0x8C;
//            b >>= 1;
//        }
//    }
//    return crc;
//}
//static DS_error_t ds_reset(volatile PORT_t *port, uint8_t pin) {
//    port->DIRSET = pin; port->OUTCLR = pin; _delay_us(480);
//    port->DIRCLR = pin; _delay_us(70);
//    if (port->IN & pin) { _delay_us(410); return DS_NO_DEVICE; }
//    _delay_us(410);
//    return (port->IN & pin) ? DS_OK : DS_TIMEOUT;
//}
//
//// -----------------------------------------------------------
//// Public API
//// -----------------------------------------------------------
//void DS_StartConversion(void) {
//    bool any_started = false;
//
//    if (sys_regs[SREG_SENSOR_ENABLE] & ENABLE_DS18B20_1) {
//        if (ds_reset(&DS_PORT1, DS_PIN1) == DS_OK) {
//            ds_write_byte(&DS_PORT1, DS_PIN1, 0xCC);
//            ds_write_byte(&DS_PORT1, DS_PIN1, 0x44);
//            any_started = true;
//        }
//    }
//    if (sys_regs[SREG_SENSOR_ENABLE] & ENABLE_DS18B20_2) {
//        if (ds_reset(&DS_PORT2, DS_PIN2) == DS_OK) {
//            ds_write_byte(&DS_PORT2, DS_PIN2, 0xCC);
//            ds_write_byte(&DS_PORT2, DS_PIN2, 0x44);
//            any_started = true;
//        }
//    }
//
//    if (any_started) {
//        ds_system_state = DS_SYSTEM_CONVERTING;
//        TCB1.CNT = 0;
//        TCB1.INTCTRL |= TCB_CAPT_bm;        // ~750?1000 ms timer
//    }
//}
//
//ds_system_state_t DS_GetSystemState(void) {
//    return ds_system_state;
//}
//
//// Read only sensor 1 (used from poll_sensors)
//DS_SENSOR_t DS_ReadSensor1(void) {
//    DS_SENSOR_t res = {0, DS_TIMEOUT};
//    if (ds_system_state != DS_SYSTEM_READY) return res;
//
//    if (ds_reset(&DS_PORT1, DS_PIN1) != DS_OK) {
//        res.error = DS_NO_DEVICE;
//        return res;
//    }
//    ds_write_byte(&DS_PORT1, DS_PIN1, 0xCC);
//    ds_write_byte(&DS_PORT1, DS_PIN1, 0xBE);
//    uint8_t data[9];
//    for (uint8_t i = 0; i < 9; i++) data[i] = ds_read_byte(&DS_PORT1, DS_PIN1);
//
//    if (ds_crc8(data, 8) != data[8]) {
//        res.error = DS_CRC_ERROR;
//    } else {
//        int16_t raw = (int16_t)((data[1] << 8) | data[0]);
//        res.temp = (int16_t)(raw * 6.25);
//        res.error = DS_OK;
//    }
//    return res;
//}
//
//// Read only sensor 2 (used from poll_sensors)
//DS_SENSOR_t DS_ReadSensor2(void) {
//    DS_SENSOR_t res = {0, DS_TIMEOUT};
//    if (ds_system_state != DS_SYSTEM_READY) return res;
//
//    if (ds_reset(&DS_PORT2, DS_PIN2) != DS_OK) {
//        res.error = DS_NO_DEVICE;
//        return res;
//    }
//    ds_write_byte(&DS_PORT2, DS_PIN2, 0xCC);
//    ds_write_byte(&DS_PORT2, DS_PIN2, 0xBE);
//    uint8_t data[9];
//    for (uint8_t i = 0; i < 9; i++) data[i] = ds_read_byte(&DS_PORT2, DS_PIN2);
//
//    if (ds_crc8(data, 8) != data[8]) {
//        res.error = DS_CRC_ERROR;
//    } else {
//        int16_t raw = (int16_t)((data[1] << 8) | data[0]);
//        res.temp = (int16_t)(raw * 6.25);
//        res.error = DS_OK;
//    }
//    return res;
//}
//
//// Timer callback ? conversion finished
//void DS_Timeout(void) {
//    if (ds_system_state == DS_SYSTEM_CONVERTING) {
//        ds_system_state = DS_SYSTEM_READY;
//    }
//    TCB1.INTCTRL &= ~TCB_CAPT_bm;
//}
//
//void DS_Init(void) {
//    TCB1_CaptureCallbackRegister(DS_Timeout);
//    DS_PORT1.DIRSET = DS_PIN1; DS_PORT1.OUTSET = DS_PIN1;
//    DS_PORT2.DIRSET = DS_PIN2; DS_PORT2.OUTSET = DS_PIN2;
//    TCB1_Start();
//    DS_StartConversion();           // kick off first conversion
//}



//// DS.c for ds18b20 sensor - IMPROVED VERSION
//#include "DS.h"
//#include <util/delay.h>
//#include "mcc_generated_files/timer/tcb1.h"
//#include "functions/system_registers.h"
//
//#define DS_PORT1  PORTA
//#define DS_PIN1   PIN2_bm
//#define DS_PORT2  PORTA
//#define DS_PIN2   PIN3_bm
//
////static volatile ds_state_t state1 = DS_IDLE;
////static volatile ds_state_t state2 = DS_IDLE;
//static volatile ds_state_t state = DS_IDLE;
//
//// --- 1-Wire Bit-Banging ---
//static void ds_write_bit(volatile PORT_t *port, uint8_t pin, uint8_t bit) {
//    port->DIRSET = pin;
//    port->OUTCLR = pin;
//    if (bit) {
//        _delay_us(6); port->OUTSET = pin; _delay_us(64);
//    } else {
//        _delay_us(60); port->OUTSET = pin; _delay_us(10);
//    }
//}
//
//static uint8_t ds_read_bit(volatile PORT_t *port, uint8_t pin) {
//    uint8_t bit;
//    port->DIRSET = pin; port->OUTCLR = pin; _delay_us(3);
//    port->DIRCLR = pin; _delay_us(10);
//    bit = (port->IN & pin) ? 1 : 0;
//    _delay_us(47);
//    return bit;
//}
//
//static void ds_write_byte(volatile PORT_t *port, uint8_t pin, uint8_t byte) {
//    for (uint8_t i = 0; i < 8; i++) {
//        ds_write_bit(port, pin, byte & 1);
//        byte >>= 1;
//    }
//}
//
//static uint8_t ds_read_byte(volatile PORT_t *port, uint8_t pin) {
//    uint8_t byte = 0;
//    for (uint8_t i = 0; i < 8; i++) {
//        byte >>= 1;
//        if (ds_read_bit(port, pin)) byte |= 0x80;
//    }
//    return byte;
//}
//
//static uint8_t ds_crc8(const uint8_t *data, uint8_t len) {
//    uint8_t crc = 0;
//    for (uint8_t i = 0; i < len; i++) {
//        uint8_t b = data[i];
//        for (uint8_t j = 0; j < 8; j++) {
//            uint8_t mix = (crc ^ b) & 1;
//            crc >>= 1;
//            if (mix) crc ^= 0x8C;
//            b >>= 1;
//        }
//    }
//    return crc;
//}
//
//static DS_error_t ds_reset(volatile PORT_t *port, uint8_t pin) {
//    port->DIRSET = pin; port->OUTCLR = pin; _delay_us(480);
//    port->DIRCLR = pin; _delay_us(70);
//    if (port->IN & pin) { _delay_us(410); return DS_NO_DEVICE; }
//    _delay_us(410);
//    return (port->IN & pin) ? DS_OK : DS_TIMEOUT;
//}
//
//// --- Public API ---
//// Start conversion on all enabled sensors
//void DS_StartConversion(void) {
//    // Start conversion on sensor 1 if enabled
//    if (state == DS_IDLE && (sys_regs[SREG_SENSOR_ENABLE] & ENABLE_DS18B20_1)) {
//        if (ds_reset(&DS_PORT1, DS_PIN1) == DS_OK) {
//            ds_write_byte(&DS_PORT1, DS_PIN1, 0xCC);  // Skip ROM
//            ds_write_byte(&DS_PORT1, DS_PIN1, 0x44);  // Convert T
//            state1 = DS_CONVERTING;
//        }
//    }
//    
//    // Start conversion on sensor 2 if enabled
//    if (state2 == DS_IDLE && (sys_regs[SREG_SENSOR_ENABLE] & ENABLE_DS18B20_2)) {
//        if (ds_reset(&DS_PORT2, DS_PIN2) == DS_OK) {
//            ds_write_byte(&DS_PORT2, DS_PIN2, 0xCC);  // Skip ROM
//            ds_write_byte(&DS_PORT2, DS_PIN2, 0x44);  // Convert T
//            state2 = DS_CONVERTING;
//        }
//    }
//    
//    // Start timer if any conversion was started
//    if (state1 == DS_CONVERTING || state2 == DS_CONVERTING) {
//        TCB1.CNT = 0;
//        TCB1.INTCTRL |= TCB_CAPT_bm;
//    }
//}
////// Start conversion on all enabled sensors
////void DS_StartConversion(void) {
////    // Start conversion on sensor 1 if enabled
////    if (state1 == DS_IDLE && (sys_regs[SREG_SENSOR_ENABLE] & ENABLE_DS18B20_1)) {
////        if (ds_reset(&DS_PORT1, DS_PIN1) == DS_OK) {
////            ds_write_byte(&DS_PORT1, DS_PIN1, 0xCC);  // Skip ROM
////            ds_write_byte(&DS_PORT1, DS_PIN1, 0x44);  // Convert T
////            state1 = DS_CONVERTING;
////        }
////    }
////    
////    // Start conversion on sensor 2 if enabled
////    if (state2 == DS_IDLE && (sys_regs[SREG_SENSOR_ENABLE] & ENABLE_DS18B20_2)) {
////        if (ds_reset(&DS_PORT2, DS_PIN2) == DS_OK) {
////            ds_write_byte(&DS_PORT2, DS_PIN2, 0xCC);  // Skip ROM
////            ds_write_byte(&DS_PORT2, DS_PIN2, 0x44);  // Convert T
////            state2 = DS_CONVERTING;
////        }
////    }
////    
////    // Start timer if any conversion was started
////    if (state1 == DS_CONVERTING || state2 == DS_CONVERTING) {
////        TCB1.CNT = 0;
////        TCB1.INTCTRL |= TCB_CAPT_bm;
////    }
////}
//
//// Check if sensor is ready to read
//ds_state_t DS_GetState(uint8_t sensor) {
//    if (sensor == 1) return state1;
//    if (sensor == 2) return state2;
//    return DS_IDLE;
//}
//
//// Check if all enabled sensors are ready to read
//bool DS_AllReady(void) {
//    bool sensor1_ok = !(sys_regs[SREG_SENSOR_ENABLE] & ENABLE_DS18B20_1) || (state1 == DS_READY);
//    bool sensor2_ok = !(sys_regs[SREG_SENSOR_ENABLE] & ENABLE_DS18B20_2) || (state2 == DS_READY);
//    return sensor1_ok && sensor2_ok;
//}
//
//// Read temperature from sensor
//DS_SENSOR_t DS_Read(uint8_t sensor) {
//    DS_SENSOR_t result = {0, DS_TIMEOUT};
//    volatile PORT_t *port = (sensor == 1) ? &DS_PORT1 : &DS_PORT2;
//    uint8_t pin = (sensor == 1) ? DS_PIN1 : DS_PIN2;
//    ds_state_t *state = (sensor == 1) ? &state1 : &state2;
//
//    if (*state != DS_READY) {
//        result.error = DS_TIMEOUT;
//        return result;
//    }
//
//    if (ds_reset(port, pin) != DS_OK) {
//        result.error = DS_TIMEOUT;
//        *state = DS_IDLE;
//        return result;
//    }
//
//    ds_write_byte(port, pin, 0xCC);  // Skip ROM
//    ds_write_byte(port, pin, 0xBE);  // Read Scratchpad
//    
//    uint8_t data[9];
//    for (uint8_t i = 0; i < 9; i++) {
//        data[i] = ds_read_byte(port, pin);
//    }
//
//    if (ds_crc8(data, 8) != data[8]) {
//        result.error = DS_CRC_ERROR;
//        *state = DS_IDLE;
//        return result;
//    }
//    if(sensor == 2) ERROR_LED_TOGGLE();
//    int16_t raw = (data[1] << 8) | data[0];
//    result.temp = (int16_t)(raw * 6.25);  // 0.0625 * 100
//    result.error = DS_OK;
//    *state = DS_IDLE;
//    
//    return result;
//}
//
//// TCB1 Timeout Callback - marks conversions as ready
//void DS_Timeout(void) {
//    if (state1 == DS_CONVERTING) state1 = DS_READY;
//    if (state2 == DS_CONVERTING) state2 = DS_READY;
//    TCB1.CNT = 0;
//    TCB1.INTCTRL &= ~TCB_CAPT_bm;
//}
//
//// Initialize DS18B20 driver
//void DS_Init(void) {
//    TCB1_CaptureCallbackRegister(DS_Timeout);
//    DS_PORT1.DIRSET = DS_PIN1; DS_PORT1.OUTSET = DS_PIN1;
//    DS_PORT2.DIRSET = DS_PIN2; DS_PORT2.OUTSET = DS_PIN2;
//    TCB1_Start();
//    DS_StartConversion();  // Start first conversion cycle
//}