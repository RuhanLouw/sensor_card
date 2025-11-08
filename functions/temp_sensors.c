
#include "definitions.h"
#include "temp_sensors.h"
#include "../mcc_generated_files/system/system.h"
//#include "../mcc_generated_files/spi/spi0.h"
#include "math.h"
#include "float.h"
#define F_CPU 16000000UL
#include <util/delay.h>
#include "debug_uart2.h"
#include "../mcc_generated_files/timer/tcb0.h"
#include "../DS.h"
#include "../mcc_generated_files/uart/usart1.h"

char debug_buffer[64];
KTYPE_STATE_t KTYPE_STATE = KTYPE_IDLE;
uint8_t tracker = 0;

void tempSensors_init(void){
    

    /* thermocouple enable PD1
     * ENntc PD7
     * SPI0miso PA5
     * SPI0clk PA6
     * A PC2
     * B PF5
     * C PF4
     */

    // MUX pins
    DISABLE_NTC_MUX();
    _A();
    _B();
    _C();
    DISABLE_KTYPE();

}

/*================================================
 NTC Temperature Conversion Functions
   ===============================================*/

// Select a single NTC component
void CS_NTC(uint8_t ntc_num){
    switch(ntc_num){
        case 1:
            _nA();
            _nB();
            _nC();
            break;
        case 2:
            _A();
            _nB();
            _nC();            
            break;
        case 3:
            _nA();
            _B();
            _nC();           
            break;
        case 4:
            _A();
            _B();
            _nC();            
            break;
        case 5:
            _nA();
            _nB();
            _C();            
            break;
        case 6:
            _A();
            _nB();
            _C();           
            break;
        case 7:
            _nA();
            _B();
            _C();            
            break;
        case 8:
            _A();
            _B();
            _C();           
            break;         
    }  
};

uint16_t mcp3201_read_bitbang(uint8_t ntc_num)
{

    uint16_t value = 0;

    CS_NTC(ntc_num); 
    enable_ntc();// select MCP
    _delay_us(2);

    // 16 clocks
    for (uint8_t i=0; i<16; i++) {
        SCK_HIGH();
        _delay_us(2);        // short delay

        value <<= 1;
        if (READ_MISO()) {
            value |= 1;
        }

        SCK_LOW();
        _delay_us(2);
    }

    disable_ntc();               // release MCP

    // drop null bit, keep 12 bits
    value = (value >> 1) & 0x0FFF;
    return value;
}

uint16_t readRaw_NTC(uint8_t ntc_num){
    uint8_t rxBuffer[2];
    CS_NTC(ntc_num);
    ENABLE_NTC_MUX();   // CS low
    _delay_us(2);   // small setup delay (tCSS)
    
//    SPI0_BufferRead(rxBuffer, 2);
//    MEASURE_LED_SET();

    DISABLE_NTC_MUX();  // CS high
    uint16_t raw = ((uint16_t)rxBuffer[0] << 8) | rxBuffer[1] >> 1;
    raw &= 0x0FFF;    // keep 12 bits
    return raw;
}

float readAvg_NTC(uint8_t ntc_num, uint8_t num_reads){
    if (num_reads == 0) return 0;
    uint32_t rawBuffer = 0;
    for(uint8_t i = 0; i < num_reads; i++){
        uint16_t raw = mcp3201_read_bitbang(ntc_num);

        //        uint16_t raw = readRaw_NTC(ntc_num);
        
        // debug print of raw (temporary)
//        printf("RAW[%u]: 0x%03X (%u)\n", ntc_num, raw, raw);

        rawBuffer += raw;
        _delay_us(10); // small inter-sample delay; adjust as needed
    }
    float avg = (float)rawBuffer / (float)num_reads;
    return avg;
}

// return in 'C
float getTemp_NTC(uint8_t ntc_num, uint8_t num_reads){
    DISABLE_KTYPE();
    float rawAvg = readAvg_NTC(ntc_num, num_reads);
    if (rawAvg < 1.0f) {
        // raw near zero -> probably wiring/CS issue. Return NaN or sentinel.
        return -INFINITY; // or NAN, or a sentinel like -273.15 to indicate error
    }

    float v_in = (rawAvg / devider_12b) * V_REF;
    if (v_in <= 0.0f || v_in >= V_REF) {
        return -INFINITY;
    }
    float r_ntc = (v_in * R_FIXED) / (V_REF - v_in);
    float tempK = 1.0f / ((1.0f / T_25) + (1.0f / BETA) * logf(r_ntc / R_25));
    float tempC = tempK - 273.15f;
    return tempC;
}

NTC_SENSOR_t read_ntc(uint8_t ntc_number, uint8_t ntc_poll_number){
    NTC_SENSOR_t buffer;
    float temp = getTemp_NTC(ntc_number, ntc_poll_number);
    
    if(temp == -INFINITY) buffer.error = 1;
    buffer.temp = (int16_t) temp*10; // Note on Master; preserve .1f
    
    return buffer;
}

/*================================================
 KTYPE Temperature Conversion Functions
   ===============================================*/

void KTYPE_bitbang(uint8_t *buffer){
    uint32_t data = 0;
    for (int i = 31; i >= 0; i--) {
        SCK_HIGH();
        _delay_us(1);
        if (READ_MISO()) data |= (1UL << i);
        SCK_LOW();
        _delay_us(1);
    }
    buffer[0] = (data >> 24) & 0xFF;
    buffer[1] = (data >> 16) & 0xFF;
    buffer[2] = (data >> 8)  & 0xFF;
    buffer[3] = data & 0xFF;
}

KTYPE_STATE_t KTYPE_check_state(void){
    return KTYPE_STATE;
}
KTYPE_STATE_t KTYPE_start_conversion(void){
    if (KTYPE_STATE != KTYPE_IDLE) return KTYPE_TIMEOUT;
    KTYPE_STATE = KTYPE_CONVERTING;
    TCB2.CNT = 0;
    TCB2.INTCTRL |= TCB_CAPT_bm; // Enable TCB2 CaptureCallback()
    return KTYPE_OK;
}

    
//};
// Incoming command =  
//[Start Byte: 1 byte]
//[Command ID: 1 byte]
//[Data Length: 1 byte]
//[Data Payload Request: 2 bytes]
//[Checksum: 1 byte]
//[End Byte: 1 byte]

//// Read NTC sensor (placeholder)
//int16_t readNTCSensor(uint8_t sensorNum) {
//    // TODO: Configure MCC ADC0 to read NTC voltage
//    // 1. Select ADC channel for sensorNum (e.g., AIN0-AIN7 for NTC 1-8)
//    // 2. Read voltage, convert to resistance (e.g., voltage divider)
//    // 3. Convert resistance to temperature using Steinhart-Hart or lookup table
//    // 4. Scale to °C × 10 (e.g., 36.8°C ? 368, -5.2°C ? -52)
//    // Dummy values in range -40°C to 150°C
//    float temp;
//    switch (sensorNum) {
//        case 1: temp = 36.8; break; // Example: NTC 1 = 36.8°C
//        case 2: temp = -5.2; break; // Example: NTC 2 = -5.2°C
//        default: temp = 25.0; // Default for others
//    }
//    return (int16_t)(temp * 10); // Convert to °C × 10 (e.g., 368, -52)
//}
//
// Read K-type thermocouple (placeholder)
KTYPE_ERROR_t readKTypeSensor(float *thermo, float *junc) {
    // TODO: Configure MCC SPI for MAX31855 or similar
    // 1. Read temperature via SPI
    // 2. Apply cold junction compensation if needed
    // 3. Scale to °C × 10
    
    // max31855kasa
    // 32 bits read only
    // unconnected = 011111111 binary temp data
    // MSBF
    // D31 = signed
    // D[30:18] = 14 bit thermocouple data
    // D[17] = reserved
    // D[16] = high when fault
    // D[15:4] = 12 bit reference junction temp data
    // D[3] = reserved
    // D[2] = short Vcc
    // D[1] = short GND
    // D[0] = open circuit
    // power up time = 200ms // this should be taken into account!!!!!!!!! has not been implimented just yet!!!
    // convertion time = 70ms
//    uint16_t thermoBuffer;
//    bool fault;
//    uint16_t juncBuffer;
//    bool shortVcc;
//    bool shortGND;
//    bool openCircuit;
////////////////////////////////
    uint8_t rxBuffer[4];
    DISABLE_NTC_MUX();
    ENABLE_KTYPE();
    _delay_us(1);
//    SPI0_BufferRead(rxBuffer, 4);
    KTYPE_bitbang(rxBuffer);
    DISABLE_KTYPE();
    
    int16_t thermocoupleRaw;
    int16_t coldJunctionRaw;

    uint32_t data = ((uint32_t)rxBuffer[0] << 24) |
                    ((uint32_t)rxBuffer[1] << 16) |
                    ((uint32_t)rxBuffer[2] <<  8) |
                    ((uint32_t)rxBuffer[3]);

//  char debug1_buffer[64];
//  sprintf(debug1_buffer, "Raw data: %X \n", data);
//  debug1_send_string(debug1_buffer);
    
    // Fault check
    if (data & 0x00010000UL) {
        // Fault occurred
//        ERROR_LED_SET();
        if(data & 0x04){
            return KTYPE_SCV;
        };        
        if(data & 0x02){
            return KTYPE_SCG;
        };
        if(data & 0x01){    
            return KTYPE_OC;
        };
    }

    // Extract thermocouple temp
    thermocoupleRaw = (data >> 18) & 0x3FFF;  // 14-bit
    if (data & 0x80000000UL) {                // sign extend
        thermocoupleRaw |= 0xC000;
    }
    *thermo = thermocoupleRaw * 0.25f;

    // Extract cold junction temp
    coldJunctionRaw = (data >> 4) & 0x0FFF;   // 12-bit
    if (data & 0x00008000UL) {                // sign extend
        coldJunctionRaw |= 0xF000;
    }
    *junc = coldJunctionRaw * 0.0625f;

    return KTYPE_OK; // Convert to °C × 10 (1000)
}

KTYPE_SENSOR_t read_ktype(void){
    KTYPE_SENSOR_t buffer;
    float therm;
    float junc;
    KTYPE_ERROR_t ktype_error = readKTypeSensor(&therm, &junc);
    if (ktype_error != KTYPE_OK){ 
        buffer.error = ktype_error;
        buffer.temp = 0;
        buffer.cold_junction = 0;
        return buffer;
    }
    float hold = therm*100;
    buffer.temp = (int16_t) hold; // Note master; preserve .2f
    hold = junc*100;
    buffer.cold_junction = (int16_t) hold; // Note master; preserve .2f
    buffer.error = ktype_error;
    return buffer;
}

void KTYPE_timer_CapCallBack(void){
//    tracker +=1;
//    UART1_Write(tracker);
//    DS_Timeout();
    KTYPE_STATE = KTYPE_READ_READY;
    TCB2.CNT = 0;
    TCB2.INTCTRL &= ~TCB_CAPT_bm; /* Capture or Timeout: disabled */
};

void KTYPE_Init(void){
    TCB2_CaptureCallbackRegister(KTYPE_timer_CapCallBack);
    KTYPE_start_conversion();
//    DS_Init();
}