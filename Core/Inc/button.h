/*
 * button.h
 *
 *  Created on: Jul 16, 2025
 *      Author: Divu
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_
/* Includes */
#include <stdint.h>

/* Define */
#define TIME_TO_LONG_PRESS 40
#define TIME_TRIGGER_LONG_PRESS 4
/* Variables */
extern uint16_t buttonCount[16];
extern uint8_t longPressFlag;
extern uint8_t longPressTrigger;
extern uint8_t lastState[16];
typedef enum
{
    BUTTON_0 = 0,
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4,
    BUTTON_5,
    BUTTON_6,
    BUTTON_7,
    BUTTON_8,
    BUTTON_9,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_EN,
    BUTTON_BACK,
} BUTTON;

/* Function */

void buttonInit(void);
void buttonScan(void);
uint16_t buttonGetState(BUTTON pos);
void checkLongPress(BUTTON pos);
uint8_t buttonFirstPress(uint8_t pos);
#endif /* INC_BUTTON_H_ */
