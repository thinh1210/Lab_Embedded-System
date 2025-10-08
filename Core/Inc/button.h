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

/* Variables */
extern uint16_t buttonCount[16];
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

extern void buttonInit(void);
extern void buttonScan(void);
extern uint16_t buttonGetState(BUTTON pos);

#endif /* INC_BUTTON_H_ */
