/* MODBUS SENSOR SLAVE REGISTERS
 * File:   system_registers.h
 * Author: Ruhan Louw
 *
 * Created on October 10, 2025, 11:32 AM
 */

#ifndef SYSTEM_REGISTERS_H
#define	SYSTEM_REGISTERS_H

#ifdef	__cplusplus
extern "C" {
#endif
//
/* SLAVE_REGISTERS.H ? FULL COMPACT SENSOR MAP (v2.0)
 * 64 registers total (0x0000?0x003F), single Modbus poll
 * Priority: System + Critical Derived Temps first (0x0000?0x000C)
 * Sensors follow (0x000D?0x001E), Expansion (0x001F?0x003F)
 * All values: int16_t, °C×10, %×10, -32768 = invalid
 */

#include <stdint.h>

#define SREG_COUNT 128
extern volatile int16_t sys_regs[SREG_COUNT];
#define REG(x) sys_regs[x]

 /* ============================================================
 * PRIORITY BLOCK: SYSTEM + CRITICAL DERIVED (0x0000?0x000C)
 * ============================================================ */
#define SREG_FIRMWARE           0x0000  // Firmware version ×100 (e.g., 200 = v2.00)
#define SREG_UPTIME_L           0x0001  // Uptime seconds, low word
#define SREG_UPTIME_H           0x0002  // Uptime seconds, high word
#define SREG_STATUS             0x0003  // System status bitfield
#define SREG_SENSOR_ENABLE      0x0004  // Sensor enable bitfield

#define SREG_CHAMBER_INTERNAL   0x0005  // Chamber internal temp (°C×10)
#define SREG_HEATING_ELEMENT    0x0006  // Heating element temp (°C×10)
#define SREG_EVAPORATOR         0x0007  // Evaporator coil temp (°C×10)
#define SREG_SUPERHEAT          0x0008  // Superheat (°C×10)
#define SREG_SUBCOOLING         0x0009  // Subcooling (°C×10)

#define SREG_COMMAND            0x000A  // Control command register
#define SREG_LOG_INTERVAL       0x000B  // Logging interval (seconds)

#define SREG_EXTERNAL           0x000C
/* ============================================================
 * SENSOR BLOCK (0x000D?0x001E)
 * ============================================================ */
// --- NTC Sensors (0x000D?0x0015) ---
#define SREG_NTC1_TEMP          0x000D  // NTC1 temp (°C×10)
#define SREG_NTC2_TEMP          0x000E
#define SREG_NTC3_TEMP          0x000F
#define SREG_NTC4_TEMP          0x0010
#define SREG_NTC5_TEMP          0x0011
#define SREG_NTC6_TEMP          0x0012
#define SREG_NTC7_TEMP          0x0013
#define SREG_NTC8_TEMP          0x0014
#define SREG_NTC_ERROR_BITS     0x0015  // 8 bits: NTC1?8 error present

// --- K-Type Thermocouple (0x0016?0x0018) ---
#define SREG_KTYPE_TEMP         0x0016  // K-type probe temp (°C×10)
#define SREG_KTYPE_CJ_TEMP      0x0017  // Cold junction temp (°C×10)
#define SREG_KTYPE_ERROR        0x0018  // Error bitfield

// --- DHT22 (0x0019?0x001B) ---
#define SREG_DHT22_TEMP         0x0019  // Temperature (°C×10)
#define SREG_DHT22_HUMIDITY     0x001A  // Humidity (%×10)
#define SREG_DHT22_ERROR        0x001B  // Error bitfield

// --- DS18B20 (0x001C?0x001E) ---
#define SREG_DS18B20_1_TEMP     0x001C  // DS18B20 #1 (°C×100)
#define SREG_DS18B20_2_TEMP     0x001D  // DS18B20 #2 (°C×100)
#define SREG_DS18B20_ERROR1      0x001E  // 8 bits: sensor 1 (0?3), sensor 2 (4?7)
#define SREG_DS18B20_ERROR2      0x001F  // 8 bits: sensor 1 (0?3), sensor 2 (4?7)

/* ============================================================
 * EXPANSION BLOCK (0x001F?0x003F)
 * ============================================================ */
#define SREG_RESERVED_START     0x0020

/* ============================================================
 * BITFIELD DEFINITIONS
 * ============================================================ */

/* --- SREG_STATUS (0x0003) --- */
#define STATUS_POLL_ACTIVE      (1 << 0)
#define STATUS_SYSTEM_READY     (1 << 1)
#define STATUS_ERROR_PRESENT    (1 << 2)
#define STATUS_LOGGING_ACTIVE   (1 << 3)
#define STATUS_RS485_OK         (1 << 4)

/* --- SREG_SENSOR_ENABLE (0x0004) --- */
#define ENABLE_NTC1             (1 << 0)
#define ENABLE_NTC2             (1 << 1)
#define ENABLE_NTC3             (1 << 2)
#define ENABLE_NTC4             (1 << 3)
#define ENABLE_NTC5             (1 << 4)
#define ENABLE_NTC6             (1 << 5)
#define ENABLE_NTC7             (1 << 6)
#define ENABLE_NTC8             (1 << 7)
#define ENABLE_KTYPE            (1 << 8)
#define ENABLE_DHT11            (1 << 9)
#define ENABLE_DS18B20_1        (1 << 10)
#define ENABLE_DS18B20_2        (1 << 11)

/* --- SREG_NTC_ERROR_BITS (0x0015) --- */
#define NTC_ERR_1               (1 << 0)
#define NTC_ERR_2               (1 << 1)
#define NTC_ERR_3               (1 << 2)
#define NTC_ERR_4               (1 << 3)
#define NTC_ERR_5               (1 << 4)
#define NTC_ERR_6               (1 << 5)
#define NTC_ERR_7               (1 << 6)
#define NTC_ERR_8               (1 << 7)

/* --- SREG_KTYPE_ERROR (0x0018) --- */
#define KTYPE_ERR_OPEN          (1 << 0)
#define KTYPE_ERR_SHORT_GND     (1 << 1)
#define KTYPE_ERR_SHORT_VCC     (1 << 2)
#define KTYPE_ERR_SPI_FAIL      (1 << 3)
#define KTYPE_ERR_TEMP_OOR      (1 << 4)

/* --- SREG_DHT22_ERROR (0x001B) --- */
#define DHT11_ERR_CHECKSUM      (1 << 0)
#define DHT11_ERR_TIMEOUT       (1 << 1)
#define DHT11_ERR_NO_RESPONSE   (1 << 2)
#define DHT11_ERR_DATA_INVALID  (1 << 3)

/* --- SREG_DS18B20_ERROR1 (0x001E) --- */
#define DS18B20_1_ERR_NO_RESP   (1 << 0)
#define DS18B20_1_ERR_CRC_FAIL  (1 << 1)
#define DS18B20_1_ERR_CONV_TO   (1 << 2)
#define DS18B20_1_ERR_INVALID   (1 << 3)

/* --- SREG_DS18B20_ERROR2 (0x001F) --- */
#define DS18B20_2_ERR_NO_RESP   (1 << 0)
#define DS18B20_2_ERR_CRC_FAIL  (1 << 1)
#define DS18B20_2_ERR_CONV_TO   (1 << 2)
#define DS18B20_2_ERR_INVALID   (1 << 3)

/* --- SREG_COMMAND (0x000A) --- */
#define CMD_SYS_RESET           (1 << 0)
#define CMD_FORCE_REFRESH       (1 << 1)


/* ============================================================
 * SYSTEM REGISTERS FUNCTIONS
 * ============================================================ */
    void SYS_REGS_INIT(void);
    void enable_all_sensors(void);
    void enable_sensor(uint16_t flag);
    void disable_sensor(uint16_t flag);
//
    
#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_REGISTERS_H */



///**
// * @file sensor_modbus_map.h
// * @brief Modbus Register Map Definitions for Sensor Card
// *
// * This file defines the Modbus register addresses and bitmaps
// * for all sensors connected to the AVR128DB32-based sensor board.
// *
// * Control Card acts as Modbus Master, Sensor Card as Slave.
// *
// * Author: Ruhan Louw
// * Version: 1.0
// */
//
//#include <stdint.h>
//    
///* ============================================================
// * SYSTEM REGISTER INSTANCE
// * ============================================================ */
//#define SYS_REGS_COUNT 128
//extern volatile int16_t sys_regs[SYS_REGS_COUNT];
//
///* ============================================================
// * GENERAL SYSTEM REGISTERS
// * ============================================================ */
//#define MB_REG_FIRMWARE_VERSION        0x0000  // Firmware version (e.g., 105 = v1.05)
//#define MB_REG_UPTIME_LSW              0x0001  // System uptime low word
//#define MB_REG_UPTIME_MSW              0x0002  // System uptime high word
//#define MB_REG_SYSTEM_STATUS           0x0003  // System status flags (see bit definitions)
//#define MB_REG_SENSOR_ENABLE_FLAGS     0x0004  // Enable status bits for all sensors
//
///* ===========================================================
// * SENORS ENABLE FLAGS
//   =========================================================== */
//    /* --- Sensor Enable Flags (0x0004) --- */
//#define EN_FLAG_NTC1           (1 << 0)
//#define EN_FLAG_NTC2           (1 << 1)
//#define EN_FLAG_NTC3           (1 << 2)
//#define EN_FLAG_NTC4           (1 << 3)
//#define EN_FLAG_NTC5           (1 << 4)
//#define EN_FLAG_NTC6           (1 << 5)
//#define EN_FLAG_NTC7           (1 << 6)
//#define EN_FLAG_NTC8           (1 << 7)
//#define EN_FLAG_KTYPE          (1 << 8)
//#define EN_FLAG_DHT22          (1 << 9)
//#define EN_FLAG_DS18B20_1      (1 << 10)
//#define EN_FLAG_DS18B20_2      (1 << 11)
//
///* ============================================================
// * NTC TEMPERATURE REGISTERS (MCP3201)
// * ============================================================ */
//#define MB_REG_NTC1_TEMP               0x0010
//#define MB_REG_NTC2_TEMP               0x0011
//#define MB_REG_NTC3_TEMP               0x0012
//#define MB_REG_NTC4_TEMP               0x0013
//#define MB_REG_NTC5_TEMP               0x0014
//#define MB_REG_NTC6_TEMP               0x0015
//#define MB_REG_NTC7_TEMP               0x0016
//#define MB_REG_NTC8_TEMP               0x0017
//
//#define MB_REG_NTC1_ERROR              0x0020
//#define MB_REG_NTC2_ERROR              0x0021
//#define MB_REG_NTC3_ERROR              0x0022
//#define MB_REG_NTC4_ERROR              0x0023
//#define MB_REG_NTC5_ERROR              0x0024
//#define MB_REG_NTC6_ERROR              0x0025
//#define MB_REG_NTC7_ERROR              0x0026
//#define MB_REG_NTC8_ERROR              0x0027
//
///* ============================================================
// * K-TYPE THERMOCOUPLE REGISTERS (MAX31855)
// * ============================================================ */
//#define MB_REG_KTYPE_TEMP              0x0030  // Measured temperature
//#define MB_REG_KTYPE_CJ_TEMP           0x0031  // Cold junction temperature
//#define MB_REG_KTYPE_ERROR             0x0032  // Error bits (see bit definitions)
//
///* ============================================================
// * DHT22 TEMPERATURE & HUMIDITY REGISTERS
// * ============================================================ */
//#define MB_REG_DHT22_TEMP              0x0040
//#define MB_REG_DHT22_HUMIDITY          0x0041
//#define MB_REG_DHT22_ERROR             0x0042
//
///* ============================================================
// * DS18B20 TEMPERATURE REGISTERS
// * ============================================================ */
//#define MB_REG_DS18B20_1_TEMP          0x0050
//#define MB_REG_DS18B20_2_TEMP          0x0051
//#define MB_REG_DS18B20_1_ERROR         0x0052
//#define MB_REG_DS18B20_2_ERROR         0x0053
//
///* ============================================================
// * COMMAND & CONFIGURATION REGISTERS
// * ============================================================ */
//#define MB_REG_COMMAND                 0x0060  // Control bits (reset, force refresh)
//#define MB_REG_LOG_INTERVAL            0x0061  // Data logging interval (seconds)
//
///* ============================================================
// * RESERVED FOR FUTURE EXPANSION
// * ============================================================ */
//#define MB_REG_RESERVED_START          0x0070
//#define MB_REG_RESERVED_END            0x00FF
//
///* ============================================================
// * BIT DEFINITIONS
// * ============================================================ */
//
///* --- System Status Register (0x0003) --- */
//#define SYS_STAT_SENSOR_POLL_ACTIVE    (1 << 0)
//#define SYS_STAT_SYSTEM_READY          (1 << 1)
//#define SYS_STAT_ERROR_PRESENT         (1 << 2)
//#define SYS_STAT_LOGGING_ACTIVE        (1 << 3)
//#define SYS_STAT_RS485_OK              (1 << 4)
//
///* --- NTC Error Flags (0x0020?0x0027) --- */
//#define NTC_ERR_OPEN_CIRCUIT           (1 << 0)
//#define NTC_ERR_SHORT_CIRCUIT          (1 << 1)
//#define NTC_ERR_OUT_OF_RANGE           (1 << 2)
//#define NTC_ERR_CONVERSION_TIMEOUT     (1 << 3)
//
///* --- K-Type Error Flags (0x0032) --- */
//#define KTYPE_ERR_OPEN_THERMOCOUPLE    (1 << 0)
//#define KTYPE_ERR_SHORT_TO_GND         (1 << 1)
//#define KTYPE_ERR_SHORT_TO_VCC         (1 << 2)
//#define KTYPE_ERR_SPI_COMM_FAIL        (1 << 3)
//#define KTYPE_ERR_TEMP_OUT_OF_RANGE    (1 << 4)
//
///* --- DHT22 Error Flags (0x0042) --- */
//#define DHT22_ERR_CHECKSUM_FAIL        (1 << 0)
//#define DHT22_ERR_TIMEOUT              (1 << 1)
//#define DHT22_ERR_NO_RESPONSE          (1 << 2)
//#define DHT22_ERR_DATA_INVALID         (1 << 3)
//
///* --- DS18B20 Error Flags (0x0052) --- */
//#define DS18B20_ERR_NO_RESPONSE        (1 << 0)
//#define DS18B20_ERR_CRC_FAIL           (1 << 1)
//#define DS18B20_ERR_CONV_TIMEOUT       (1 << 2)
//#define DS18B20_ERR_INVALID_TEMP       (1 << 3)
//
///* --- Command Register (0x0060) --- */
//#define CMD_SYS_RESET                  (1 << 0)
//#define CMD_FORCE_REFRESH              (1 << 1)