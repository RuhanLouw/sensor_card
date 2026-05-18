/* 
 * File:   DS.h
 * Author: Ruhan Louw
 *
 * Created on November 6, 2025, 12:06 PM
 */

#ifndef DS_H
#define DS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------
// DS18B20 return structure
// -----------------------------------------------------------
typedef enum {
    DS_OK = 0,
    DS_TIMEOUT,
    DS_NO_DEVICE,
    DS_CRC_ERROR
} DS_error_t;

typedef struct {
    int16_t     temp;   // temperature in 0.01°C steps (e.g. 2518 = 25.18°C)
    DS_error_t  error;
} DS_SENSOR_t;

// -----------------------------------------------------------
// System state (one state for both sensors)
// -----------------------------------------------------------
typedef enum {
    DS_SYSTEM_IDLE,      // nothing running or waiting to start
    DS_SYSTEM_CONVERTING,
    DS_SYSTEM_READY      // conversion finished ? safe to read both sensors
} ds_system_state_t;

extern volatile ds_system_state_t ds_system_state;
// -----------------------------------------------------------
// Public functions
// -----------------------------------------------------------
void              DS_Init(void);
void              DS_StartConversion(void);
ds_system_state_t DS_GetSystemState(void);

DS_SENSOR_t       DS_ReadSensor1(void);   // call only when state == DS_SYSTEM_READY
DS_SENSOR_t       DS_ReadSensor2(void);   // call only when state == DS_SYSTEM_READY

#ifdef __cplusplus
}
#endif

#endif /* DS_H */