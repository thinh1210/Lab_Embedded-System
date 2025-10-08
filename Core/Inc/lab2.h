/*
 * lab2.h
 *
 *  Created on: Oct 6, 2025
 *      Author: Divu
 */

#ifndef INC_LAB2_H_
#define INC_LAB2_H_
/*Include --------------------------------------------*/
#include <software_timer.h>
#include <led_7seg.h>
#include <led.h>
/*Define ---------------------------------------------*/

/*Enum   --------------------------------------------*/

/*Struct --------------------------------------------*/

/*Variables------------------------------------------*/

/*Function -------------------------------------------*/
/*Example in the excercise */
void init_system(void);
void Led_Debug(void);
void Led_Y0(void);
void Led_Y1(void);
void test7Seg(void);

/*lab2 bai1*/
void initLab2Bai1(void);
void runLab2Bai1(void);

/*lab2 bai2*/
void initLab2Bai2(void);
void runLab2Bai2(void);

/*lab2 bai3*/
void initLab2Bai3(void);
void runLab2Bai3(void);

/*lab2 bai4*/
void initLab2Bai4(void);
void runLab2Bai4(void);

/*lab2 bai5*/
void intLab2Bai5(void);
void runLab2Bai5(void);
#endif /* INC_LAB2_H_ */
