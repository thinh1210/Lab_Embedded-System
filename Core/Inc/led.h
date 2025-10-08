#ifndef LED_DEBUG_H_
#define LED_DEBUG_H_

/*Includes ------------------------------------------*/
#include <stdint.h>
#include "main.h"
// state of the LED
/*Define---------------------------------------------*/
#define LED_ON_TIME		2
#define LED_OFF_TIME	4

#define RED_TIME		5
#define GREEN_TiME		3
#define YELLOW_TIME 	1
/* Enum ---------------------------------------------*/
typedef enum{
	LED_ON,
	LED_OFF
} LED_STATE;

typedef enum{
	RED,
	GREEN,
	YELLOW
} TRAFFIC_STATE;
/* Struct -------------------------------------------*/


/* Variables ----------------------------------------*/


/* Functions ----------------------------------------*/
//Control thew  GPIO
void ledOn(void);
void ledOff(void);
void ledY0On(void);
void ledY0Off(void);
void ledY1On(void);
void ledY1Off(void);

//Init and global funtion for main call

void debugLed(void);
void initEx2(void);
void fsmEx2(void);
void initEx3(void);
void fsmEx3(void);
#endif
