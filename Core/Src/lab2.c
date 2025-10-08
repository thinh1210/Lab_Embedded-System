/*
 * lab2.c
 *
 *  Created on: Oct 6, 2025
 *      Author: Divu
 */
/*Include ----------------------------------------------*/
#include <../Inc/lab2.h>

/*Define -----------------------------------------------*/

/*Variables --------------------------------------------*/
unsigned char statusLed_Debug = 0;
unsigned char statusLed_Y0 = 0;
unsigned char statusLed_Y1 = 0;
/*Function ---------------------------------------------*/
void init_system(void)
{
	timer2Init();
	timer2Setup(5);
	led7segInit();
}

void Led_Debug()
{
	statusLed_Debug = (statusLed_Debug + 1) % 20; // Period = 50 (ms) * 20 = 1000 (ms)
	if (statusLed_Debug == 0)					  // Turn on in (10 / 20) * 1000 = 500 (ms)
		HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
}

void Led_Y0()
{
	statusLed_Y0 = (statusLed_Y0 + 1) % 120; // Period = 50 (ms) * 120 = 6000 (ms)
	if (statusLed_Y0 < 40)
		HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 1); // Turn on 2s
	else
		HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0); // Turn off 4s
}

void Led_Y1()
{
	statusLed_Y1 = (statusLed_Y1 + 1) % 120; // Period = 50 (ms) * 120 = 6000 (ms)
	if (statusLed_Y1 < 100)
		HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 1); // Turn on 5s
	else
		HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0); // Turn off 1s
}

void test7Seg()
{
	led7segSetDigit(0, 0, 0);
	led7segSetDigit(0, 1, 0);
	led7segSetDigit(1, 2, 0);
	led7segSetDigit(1, 3, 0);
}
/*lab2 bai1*/
void initLab2Bai1(void)
{
	timer2Init();
	timer2Setup(50);
}
void runLab2Bai1(void)
{
	// use busy waiting to run the prog
	while (!timer2Flag)
		;
	timer2Flag = 0;
	Led_Debug();
	Led_Y0();
	Led_Y1();
}

/*lab2 bai2*/
void initLab2Bai2(void)
{
	timer2Init();
	initEx3();
	timerInit(0, 1000, 1000, fsmEx3);
}
void runLab2Bai2(void)
{
	doTask();
}

/*lab2 bai3*/
void initLab2Bai3(void)
{
	timer2Init();
	led7segInit();
	test7Seg();
	/* f=1hz ->each number: 250ms*/
	timerInit(0, 250, 250, led7segDisplay);
	/* f=25hz -> each number: 10ms*/
	// timerInit(1,10,10,led7segDisplay);
	/* f=100hz -> each number: 2.5ms*/ //-> Can update tick to 0.1ms to get this frequency or can get approximately by 2ms
									   // timerInit(1,2,2,led7segDisplay);
}
void runLab2Bai3(void)
{
	doTask();
}

/*Lab2 bai4*/
void initLab2Bai4(void)
{
	timer2Init();
	initTime(13, 0);
	timerInit(0, 5, 5, led7segDisplay);
	timerInit(1, 250, 250, toggleColon);  // f=2hz -> each number: 500ms
	timerInit(2, 1000, 1000, updateTime); // to simulate need to change the period and counter to 1000
}
void runLab2Bai4(void)
{
	doTask();
}

/* Lab2Bai5 */
void initLab2Bai5(void)
{
	timer2Init();
	initTime(13, 0);
	timerInit(0, 5, 5, led7segDisplay);
	timerInit(1, 1000, 1000, shiftRightLed7); // shift right each second
}
void runLab2Bai5(void)
{
	doTask();
}