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
/* MODBUS RTU Header File */
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// STATE TYPE - ADD THIS
// ============================================================================

// ============================================================================
// PUBLIC FUNCTIONS
// ============================================================================
/**
 * @brief Initialize Modbus RTU library
 * 
 * Sets up UART callbacks, timer, and RS485 transceiver
 */
void MB_Init(void);
/**
 * @brief Process received Modbus frames
 * 
 * Call this function regularly in your main loop.
 * It will handle frame validation and response generation.
 */
void modbus_process(void);
/**
 * @brief Enable RS485 transmitter
 */
void RS485_TX_ENABLE(void);
/**
 * @brief Enable RS485 receiver
 */
void RS485_RX_ENABLE(void);
/**
 * @brief Calculate Modbus CRC16
 * 
 * @param data Pointer to data buffer
 * @param length Number of bytes to calculate CRC for
 * @return Calculated CRC16 value
 */
static uint16_t modbus_crc16(const uint8_t *data, uint8_t length);

// ============================================================================
// DIAGNOSTIC FUNCTIONS - ADD THESE
// ============================================================================
/**
 * @brief Get current receive buffer index
 */
uint8_t MB_GetRxIndex(void);

/**
 * @brief Check if a frame is ready for processing
 */
bool MB_IsFrameReady(void);

/**
 * @brief Get current Modbus state machine state
 */

/**
 * @brief Get last received frame data
 */
void MB_GetLastFrame(uint8_t *buffer, uint8_t *length);

/**
 * @brief Echo last received frame with diagnostic info
 */
void MB_EchoLastFrame(void);

// ============================================================================
// INTERRUPT SERVICE ROUTINES
// ============================================================================
/**
 * @brief UART receive interrupt handler
 * 
 * Register this with UART1_RxCompleteCallbackRegister()
 */
void modbus_receive_isr(void);

void modbus_transmit_isr(void);
/**
 * @brief Timer timeout interrupt handler
 * 
 * Register this with TCB0_CaptureCallbackRegister()
 */

void modbus_timer_isr(void);
#ifdef	__cplusplus
}
#endif
#endif	/* MODBUS_H */