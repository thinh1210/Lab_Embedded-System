/*
 * uart.c
 *
 * Created on: Nov 7, 2025
 * Author: Divu
 */
#include "uart.h"
#include <string.h> // For strlen()
#include <stdio.h>  // For sprintf()

// Global buffer for formatting strings
uint8_t msg[100];
uint8_t receive_buffer1 = 0;
// Note: You must declare huart1 in uart.h or pass it as an argument
extern UART_HandleTypeDef huart1;

void uart_init_rs232() {
    // Assuming receive_buffer1 is declared in uart.h or globally
    extern uint8_t receive_buffer1;
	HAL_UART_Receive_IT(&huart1, &receive_buffer1, 1);
}

/**
 * @brief Sends a null-terminated string over UART.
 * @param str: The C-string (const char*) to send.
 */
void uart_Rs232SendString(const char* str) {
	HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 10);
}

/**
 * @brief Sends raw bytes over UART.
 */
void uart_Rs232SendBytes(uint8_t* bytes, uint16_t size) {
	HAL_UART_Transmit(&huart1, bytes, size, 10);
}

/**
 * @brief Formats and sends an unsigned 32-bit integer as a string.
 */
void uart_Rs232SendNum(uint32_t num) {
    // %lu is the correct format specifier for uint32_t (unsigned long)
	int len = sprintf((char*)msg, "%lu", num);
	HAL_UART_Transmit(&huart1, msg, len, 10);
}

/**
 * @brief Formats and sends a number as "XX.YY".
 * @param num: An integer representing the value (e.g., 12345 -> "123.45")
 */
void uart_Rs232SendNumPercent(uint32_t num) {
    // %02lu pads the fractional part with a leading zero (e.g., 5 -> "05")
	int len = sprintf((char*)msg, "%lu.%02lu", num / 100, num % 100);
	HAL_UART_Transmit(&huart1, msg, len, 10);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	if(huart->Instance == USART1) {
		// rs232 isr
		// This is a simple echo-back
		HAL_UART_Transmit(&huart1, &receive_buffer1, 1, 10);

		// Re-arm the receive interrupt
		HAL_UART_Receive_IT(&huart1, &receive_buffer1, 1);
	}
}
