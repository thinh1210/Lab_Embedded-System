/*
 * led_7seg.h
 *
 *  Created on: Jul 15, 2025
 *      Author: Divu
 */

#ifndef INC_LED_7SEG_H_
#define INC_LED_7SEG_H_
/*	Include */
#include "stdint.h"

/* Variable */

/* Function */
extern void led7segInit();
extern void led7segSetDigit(int num, int position, uint8_t showDot);
extern void led7segSetColon(uint8_t status);
extern void led7segDisplay();
extern void led7segOn(uint8_t index);
extern void led7segOff(uint8_t index);
void toggleColon(void);
void shiftRightLed7(void);
void initTime(uint8_t h, uint8_t m);
void updateTime(void);

#endif /* INC_LED_7SEG_H_ */
