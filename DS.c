// DS.c
#include "DS.h"
#include <util/delay.h>
#include "mcc_generated_files/timer/tcb1.h"
#include "functions/system_registers.h"

#define DS_PORT1  PORTA
#define DS_PIN1   PIN2_bm
#define DS_PORT2  PORTA
#define DS_PIN2   PIN3_bm

static volatile ds_state_t state1 = DS_IDLE;
static volatile ds_state_t state2 = DS_IDLE;
static volatile uint8_t ms100_ticks = 0;

// --- 1-Wire Bit-Banging ---
static void ds_write_bit(volatile PORT_t *port, uint8_t pin, uint8_t bit) {
    port->DIRSET = pin;
    port->OUTCLR = pin;
    if (bit) {
        _delay_us(6); port->OUTSET = pin; _delay_us(64);
    } else {
        _delay_us(60); port->OUTSET = pin; _delay_us(10);
    }
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

// --- Public API ---
void DS_StartConversion(void) {
    bool conversion_started = false;
    if (state1 == DS_IDLE && (sys_regs[MB_REG_SENSOR_ENABLE_FLAGS] & EN_FLAG_DS18B20_1)){
        if(ds_reset(&DS_PORT1, DS_PIN1) == DS_OK) {
            ds_write_byte(&DS_PORT1, DS_PIN1, 0xCC);
            ds_write_byte(&DS_PORT1, DS_PIN1, 0x44);
            state1 = DS_CONVERTING;
            conversion_started = true;   
        }
    }
    if(state2 == DS_IDLE && (sys_regs[MB_REG_SENSOR_ENABLE_FLAGS] && EN_FLAG_DS18B20_2)){
        if(ds_reset(&DS_PORT2, DS_PIN2) == DS_OK) {
            ds_write_byte(&DS_PORT2, DS_PIN2, 0xCC);
            ds_write_byte(&DS_PORT2, DS_PIN2, 0x44);
            state2 = DS_CONVERTING;
            conversion_started = true;
        }
    }
    if (conversion_started) {
        TCB1.CNT = 0;
        TCB1.INTCTRL |= TCB_CAPT_bm;       
    }
}

ds_state_t DS_Check_State(uint8_t sensor) {
    if(sensor == 1) return state1;
    if(sensor == 2) return state2;
    else return DS_NO_DEVICE;
}

DS_SENSOR_t DS_Read(uint8_t sensor) {
    DS_SENSOR_t result = {0, DS_TIMEOUT};
    volatile PORT_t *port = (sensor == 1) ? &DS_PORT1 : &DS_PORT2;
    uint8_t pin = (sensor == 1) ? DS_PIN1 : DS_PIN2;
    ds_state_t *state = (sensor == 1) ? &state1 : &state2;

    if (*state != DS_READY) return result;

    if (ds_reset(port, pin) != DS_OK) {
        result.error = DS_TIMEOUT;
        *state = DS_IDLE;
        return result;
    }

    ds_write_byte(port, pin, 0xCC);
    ds_write_byte(port, pin, 0xBE);
    uint8_t data[9];
    for (uint8_t i = 0; i < 9; i++) data[i] = ds_read_byte(port, pin);

    if (ds_crc8(data, 8) != data[8]) {
        result.error = DS_CRC_ERROR;
        *state = DS_IDLE;

        return result;
    }

    int16_t raw = (data[1] << 8) | data[0];
    result.temp = (int16_t)(raw * 6.25);  // 0.0625 * 100
    result.error = DS_OK;
    *state = DS_IDLE;
//    DS_StartConversion();
    return result;
}

//  TCB1 Timeout Callback 
void DS_Timeout(void) {
    if (state1 == DS_CONVERTING) state1 = DS_READY;
    if (state2 == DS_CONVERTING) state2 = DS_READY;
    TCB1.CNT = 0;
    TCB1.INTCTRL &= ~TCB_CAPT_bm;
    
}
void DS_Init(void) {
    TCB1_CaptureCallbackRegister(DS_Timeout);
    DS_PORT1.DIRSET = DS_PIN1; DS_PORT1.OUTSET = DS_PIN1;
    DS_PORT2.DIRSET = DS_PIN2; DS_PORT2.OUTSET = DS_PIN2;
    TCB1_Start();
    DS_StartConversion();
}
