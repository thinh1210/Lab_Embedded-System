/*
 * lab4.h
 *
 *  Created on: Nov 11, 2025
 *      Author: Divu
 */

#ifndef INC_LAB4_H_
#define INC_LAB4_H_
/*Include ---------------------------------------------*/
#include <software_timer.h>
#include <stdbool.h>
#include "led.h"
#include "software_timer.h"
#include "led_7seg.h"
#include "lcd.h"
#include "picture.h"
#include "button.h"
#include "ds3231.h"

/*Define ---------------------------------------------*/
#define NORMAL_MODE "NORMAL MODE"
#define DELETE_FILL "                           "
#define EDIT_TIME_MODE "EDIT TIME MODE"
#define EDIT_ALARM_MODE "EDIT ALARM MODE"
#define TIME_WARNING 10
/*Enum   --------------------------------------------*/
typedef enum
{
    NORMAL,
    EDIT_TIME,
    EDIT_ALARM,
    EDIT_TIME_BY_RS232
} lAB4_MAIN_STATE;
typedef enum
{
    TIME_SEC,
    TIME_MIN,
    TIME_HOUR,
    TIME_DATE,
    TIME_DAY,
    TIME_MONTH,
    TIME_YEAR,
} TIME_LAB4;

typedef enum
{
    EDIT_TIME_SEC,
    EDIT_TIME_MIN,
    EDIT_TIME_HOUR,
    EDIT_TIME_DATE,
    EDIT_TIME_DAY,
    EDIT_TIME_MONTH,
    EDIT_TIME_YEAR,
} EDIT_TIME_STATE;
// date auto update to use typef EDIT_TIME_STATE for two main state
typedef enum
{
    EDIT_ALARM_SEC,
    EDIT_ALARM_MIN,
    EDIT_ALARM_HOUR,
    EDIT_ALARM_DATE,
    EDIT_ALARM_DAY
} EDIT_ALARM_STATE;

typedef enum
{
    ALARM,
    NO_ALARM,
} ALARM_STATE;
/*Struct --------------------------------------------*/

/*Variables------------------------------------------*/
extern lAB4_MAIN_STATE lab4_state;
extern EDIT_TIME_STATE edit_time_state;
extern EDIT_ALARM_STATE edit_alarm_state;

extern uint8_t bufferTime[7];
extern uint8_t bufferAlarm[5];

/*Function -------------------------------------------*/
/*external function*/
void initLab4(void);
void runLab4(void);
/*Internal function*/
// Behavior
//  BUTTONEN -> save all -> next into MAIN STATE
//  BUTTON1 -> change edit element in two mode
// BUTTONBACK ->  don't need to check  -> alarm to ALARM SKIP -> next state when alarm skip

//  BUTTONNEXT-> next into MAIN STATE
//  BUTTONUP -> edit each  -> up for each kinds of time
//  longpress -> update every

void mainFsm(void);
void editTimeFsm(void);
void editAlarmFsm(void);

/*lcd function*/
void displayLCD(uint8_t number, TIME_LAB4 pos, uint8_t hiden);

/*init function*/
void initNormal(void);
void initEditTime(void);
void initEditAlarm(void);

/*support function*/

void updateTimeSec(void);
void updateTimeMin(void);
void updateTimeHour(void);
void updateTimeDate(void);
void updateTimeMonth(void);
void updateTimeYear(void);
void updateTimeDay(void);

void updateAlarmSec(void);
void updateAlarmMin(void);
void updateAlarmHour(void);
void updateAlarmDate(void);
void updateAlarmDay(void);

void checkNumber(EDIT_TIME_STATE state);

// other Task
void taskResend(); // Check the main state in
void blinkLed();
void updateTimeLCD();
void checkAlarm();
/*Example in the excercise */

#endif /* INC_LAB4_H_ */
