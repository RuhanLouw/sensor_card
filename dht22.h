/* 
 * File:   dht22.h
 * Author: Ruhan Louw
 *
 * Created on November 25, 2025, 3:42 PM
 */

#ifndef DHT22_H
#define	DHT22_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int16_t  temp;      // 0.1°C   (example: 236 = 23.6°C)
    uint16_t humidity;  // 0.1 %RH (example: 487 = 48.7%)
    uint16_t error;     // 0 = OK, bits below
} DHT22_SENSOR_t;

#define DHT22_ERROR_NONE        0
#define DHT22_ERROR_TIMEOUT     (1<<0)
#define DHT22_ERROR_CHECKSUM    (1<<1)
#define DHT22_ERROR_NO_RESPONSE (1<<2)

// Call from your 1 Hz PIT interrupt (every 1 second)
void DHT22_Tick_1Hz(void);

// Returns current state ? use exactly like your other sensors
// DHT22_READ_READY = new data available
typedef enum {
    DHT22_IDLE = 0,
    DHT22_READ_READY,
    DHT22_BUSY
} dht22_state_t;

dht22_state_t DHT22_STATE(void);

// Call only when DHT22_STATE() == DHT22_READ_READY
DHT22_SENSOR_t read_dht22(void);

// Call once at startup
void DHT22_Init(void);


#ifdef	__cplusplus
}
#endif

#endif	/* DHT22_H */

