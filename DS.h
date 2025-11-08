/* 
 * File:   DS.h
 * Author: Ruhan Louw
 *
 * Created on November 6, 2025, 12:06 PM
 */

#ifndef DS_H
#define	DS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "functions/definitions.h"

typedef enum { 
    DS_IDLE = 0, 
    DS_CONVERTING, 
    DS_READY 
} ds_state_t;
    
typedef enum {
    DS_OK = 0,
    DS_NO_DEVICE,
    DS_TIMEOUT,
    DS_CRC_ERROR
} DS_error_t;

typedef struct {
    int16_t temp;   // °C × 10
    uint8_t error;  // DS_error_t
} DS_SENSOR_t;

void DS_Timeout(void);
void DS_Init(void);
void DS_StartConversion(void);
DS_SENSOR_t DS_Read(uint8_t sensor);  // 1 or 2
ds_state_t DS_Check_State(uint8_t sensor);  // 1 = ready


#ifdef	__cplusplus
}
#endif

#endif	/* DS_H */

