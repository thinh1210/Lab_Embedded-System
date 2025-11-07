/*
 * lab3.h
 *
 *  Created on: Nov 3, 2025
 *      Author: Divu
 */

#ifndef INC_LAB3_H_
#define INC_LAB3_H_
/*Include --------------------------------------------*/
#include <software_timer.h>
#include <stdbool.h>
#include "led.h"
#include "software_timer.h"
#include "led_7seg.h"
#include "lcd.h"
#include "picture.h"
#include "button.h"
/*Define ---------------------------------------------*/
#define DELETE_FILL "                    "
#define NORMAL_MODE "NORMAL MODE  "
#define SETTING_RED "SETTING RED  "
#define SETTING_GREEN "SETTING GREEN"
#define SETTING_YELLOW "SETTING YELLOW"

#define RED_TIME_LAB3 10
#define GREEN_TIME_LAB3 7
#define YELLOW_TIME_LAB3 3

/*
    define task 0: read button
    define task 1: fsm ( change state, update time, update display)
    define task 2: update real time
    define task 3: task for blink led when setting
    use button in 1 ,2,  4 in array button
    1 for change maode
*/
/*Enum   --------------------------------------------*/
typedef enum
{
    STATE_NS_GO,
    STATE_NS_YELLOW,
    STATE_EW_GO,
    STATE_EW_YELLOW
} TrafficState;

typedef enum
{
    SET_GREEN,
    SET_RED,
    SET_YELLOW,
    NORMAL
} MODE;
/*Struct --------------------------------------------*/

/*Variables------------------------------------------*/
char extern buffer[100];
int extern numberLight1;
int extern numberLight2;
/*Function -------------------------------------------*/
/*Example in the excercise */
/* Extenal function*/
void Background(void);
void initlab3(void);
void runlab3(void);

/*Internal function */
void fsmlab3(void);
void updateDisplayNumbers(void);
void switchTLToNextState(void);
void updateMode(void);
void saveMode(void);
void upTime(void);
void taskBlinkLed(void);
void NSGo(void);
void NSYellow(void);
void EWGo(void);
void EWYellow(void);
void updatetimer(void);
void comeBackTask(void);
void clearLed(void);
#endif /* INC_LAB3_H_ */
