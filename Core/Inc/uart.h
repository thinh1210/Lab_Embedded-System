/*
 * uart.h
 *
 *  Created on: Nov 7, 2025
 *      Author: Divu
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "usart.h"
#include <stdio.h>
#include "utils.h"

    extern uint8_t receive_buffer1;
void uart_init_rs232();

void uart_Rs232SendString(const char* str);

void uart_Rs232SendBytes(uint8_t* bytes, uint16_t size);

void uart_Rs232SendNum(uint32_t num);

void uart_Rs232SendNumPercent(uint32_t num);

#endif /* INC_UART_H_ */
