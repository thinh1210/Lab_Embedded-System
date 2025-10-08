/*
 * button.c
 *
 *  Created on: Jul 16, 2025
 *      Author: Divu
 */

#include "button.h"

#include "spi.h"
#include "gpio.h"

/* Variables */
uint16_t buttonCount[16] = {0};
static uint16_t buttonSpiBuffer = 0x0000;
uint8_t buttonPos[16] = {14, 0, 1, 2, 7, 6, 5, 8, 9, 10, 3, 4, 11, 12, 15, 13};
int buttonInx=0;
/* Functions */
/**
 * @brief  	Init matrix button
 * @param  	None
 * @retval 	None
 */
void buttonInit()
{
    HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, 1);
}

/**
 * @brief  	Scan matrix button
 * @param  	None
 * @note  	Call every 50ms
 * @retval 	None
 */
void buttonScan()
{
    HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, 0);
    HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, 1);
    HAL_SPI_Receive(&hspi1, (void *)&buttonSpiBuffer, 2, 10);

    buttonInx=0;
    uint16_t mask = 0x8000;
    for (int i = 0; i < 16; i++)
    {
        if (i >= 0 && i <= 3)
        {
        	buttonInx = i + 4;
        }
        else if (i >= 4 && i <= 7)
        {
        	buttonInx = 7 - i;
        }
        else if (i >= 8 && i <= 11)
        {
        	buttonInx = i + 4;
        }
        else
        {
        	buttonInx = 23 - i;
        }
        if (buttonSpiBuffer & mask)
            buttonCount[buttonInx] = 0;
        else
            buttonCount[buttonInx]++;
        mask = mask >> 1;
    }
}
uint16_t buttonGetState(BUTTON pos){
	return buttonCount[buttonPos[pos]];
}
