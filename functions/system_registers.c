#include "system_registers.h"

volatile uint16_t sys_regs[SYS_REGS_COUNT];
// -------------------------------------------------------------
// Function: Enable all sensors by setting the flag bits
// -------------------------------------------------------------
void enable_all_sensors(void)
{
    sys_regs[MB_REG_SENSOR_ENABLE_FLAGS] =
          EN_FLAG_NTC1
        | EN_FLAG_NTC2
        | EN_FLAG_NTC3
        | EN_FLAG_NTC4
        | EN_FLAG_NTC5
        | EN_FLAG_NTC6
        | EN_FLAG_NTC7
        | EN_FLAG_NTC8
        | EN_FLAG_KTYPE
        | EN_FLAG_DHT22
        | EN_FLAG_DS18B20_1
        | EN_FLAG_DS18B20_2;
}

// -------------------------------------------------------------
// Enable specific sensors dynamically
// -------------------------------------------------------------
void enable_sensor(uint16_t flag)
{
    sys_regs[MB_REG_SENSOR_ENABLE_FLAGS] |= flag;
}

void disable_sensor(uint16_t flag)
{
    sys_regs[MB_REG_SENSOR_ENABLE_FLAGS] &= ~flag;
}






///* ============================================================
// * FUNCTION: Update Modbus Registers from Sensor Structs
// * ============================================================ */
//void update_modbus_registers(void) {
//    // System info registers (example values)
//    sys_regs[MB_REG_FIRMWARE_VERSION] = 100; // v1.05
//    sys_regs[MB_REG_UPTIME_LSW] = 0;         // To be filled from timer
//    sys_regs[MB_REG_UPTIME_MSW] = 0;
//    sys_regs[MB_REG_SYSTEM_STATUS] = SYS_STAT_SENSOR_POLL_ACTIVE | SYS_STAT_SYSTEM_READY;
//
//    // NTC sensors (use int i, temp + error addr = 32)
//    for (int i = 0; i < 8; i++) {
//        sys_regs[MB_REG_NTC1_TEMP + i] = sys_sensors.ntcs[i].temp;
//        sys_regs[MB_REG_NTC1_ERROR + i] = sys_sensors.ntcs[i].error;
//    }
//
//    // K-type thermocouple
//    sys_regs[MB_REG_KTYPE_TEMP] = sys_sensors.ktype->temp;
//    sys_regs[MB_REG_KTYPE_CJ_TEMP] = sys_sensors.ktype->cold_junction;
//    sys_regs[MB_REG_KTYPE_ERROR] = sys_sensors.ktype->error;
//
//    // DHT22
//    sys_regs[MB_REG_DHT22_TEMP] = sys_sensors.dht22->temp;
//    sys_regs[MB_REG_DHT22_HUMIDITY] = sys_sensors.dht22->humidity;
//    sys_regs[MB_REG_DHT22_ERROR] = sys_sensors.dht22->error;
//
//    // DS18B20
//    for (int i = 0; i < 2; i++) {
//        sys_regs[MB_REG_DS18B20_1_TEMP + i] = sys_sensors.ds18b20[i].temp;
//        sys_regs[MB_REG_DS18B20_ERROR] = sys_sensors.ds18b20[i].error;
//    }
//}
