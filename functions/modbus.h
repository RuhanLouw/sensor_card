/* 
 * File:   modbus.h
 * Author: Ruhan Louw
 *
 * Created on August 25, 2025, 8:42 PM
 */

#ifndef MODBUS_H
#define	MODBUS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
/* ============================================================
 * GLOBAL MODBUS REGISTER ARRAY
 * ============================================================ */
#define MODBUS_REG_COUNT 256

void RS485_TX_ENABLE(void);
void RS485_RX_ENABLE(void);
void modbus_process(void);
void modbus_receive(void);
void modbus_timer_expired(void);


#ifdef	__cplusplus
}
#endif

#endif	/* MODBUS_H */

