/* 
 * File:   debug_uart2.h
 * Author: Ruhan Louw
 *
 * Created on August 26, 2025, 4:24 PM
 */

#ifndef DEBUG_UART2_H
#define	DEBUG_UART2_H

#ifdef	__cplusplus
extern "C" {
#endif

void debug_uart_init(void);
void debug_uart_send_string(const char *str);
void USART1_write_string(const char *str);
void debug1_send_string(const char *str);
//void debug_uart_send_sensor_data(void);


#ifdef	__cplusplus
}
#endif

#endif	/* DEBUG_UART2_H */

