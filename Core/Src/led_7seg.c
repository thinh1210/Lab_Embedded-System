/*
 * led_7seg.c
 *
 *  Created on: Jul 15, 2025
 *      Author: Divu
 */
/* Include */
#include <led_7seg.h>
#include <spi.h>

/* Variable */
uint8_t led7seg[4] = {0, 1, 2, 3};
uint8_t led7segMap[10] = {0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41,
						  0x1f, 0x01, 0x09};
// the signal =0 the led on , =1 the led off
uint8_t led7segInx = 0;
uint16_t spiBuffer = 0;
uint8_t hour = 0;
uint8_t minute = 0;
/* Function */

/**
 * @brief Init led
 * @param None
 * @return None
 */
void led7segInit()
{
	HAL_GPIO_WritePin(LD_LATCH_GPIO_Port, LD_LATCH_Pin, SET);
}

/**
 * @brief	Scan led 7 segment
 * @param	None
 * @note	Call in 1ms interrupt
 * @retval 	None
 */
void led7segDisplay()
{
	spiBuffer &= 0x00ff;
	spiBuffer |= led7seg[led7segInx] << 8; // shift left map 8bit then or it spibffer= 0xLLFF with LL is value of map

	/*
	 //(1011)= 0xb -> LED1
	  (1101) = 0xd -> LED2
	  (1110) = 0xe -> LED3
	  (0111) = 0x7 -> LED4
	 */

	switch (led7segInx)
	{
	case 0:
		spiBuffer |= 0x00b0;
		spiBuffer &= 0xffbf;
		break;
	case 1:
		spiBuffer |= 0x00d0;
		spiBuffer &= 0xffdf;
		break;
	case 2:
		spiBuffer |= 0x00e0;
		spiBuffer &= 0xffef;
		break;
	case 3:
		spiBuffer |= 0x0070;
		spiBuffer &= 0xff7f;
		break;
	default:
		break;
	}
	led7segInx = (led7segInx + 1) % 4;

	HAL_GPIO_WritePin(LD_LATCH_GPIO_Port, LD_LATCH_Pin, 0);
	HAL_SPI_Transmit(&hspi1, (void *)&spiBuffer, 2, 1);
	HAL_GPIO_WritePin(LD_LATCH_GPIO_Port, LD_LATCH_Pin, 1);
}

/**
 * @brief  	Display a digit at a position of led 7-segment
 * @param  	num	Number displayed
 * @param  	pos	The position displayed (index from 0)
 * @param  	show_dot Show dot in the led or not
 * @retval 	None
 */
void led7segSetDigit(int num, int position, uint8_t showDot)
{
	if (num >= 0 && num <= 9)
	{
		led7seg[position] = led7segMap[num] - showDot;
	}
}

/**
 * @brief	Control the colon led
 * @param	status Status applied to the colon (1: turn on, 0: turn off)
 *          This parameter can be one of the following values:
 *				@arg 0: Turn off
 *				@arg 1: Turn on
 * @retval None
 */
void led7segSetColon(uint8_t status)
{
	if (status == 1)
		spiBuffer &= ~(1 << 3);
	else
		spiBuffer |= (1 << 3);
}

/**
 * @brief  	Turn on one of three led at top-right corner of the board
 * @param  	index Index of the led
 *          This parameter can be one of the following values:
 *            	@arg 6
 *            	@arg 7
 *            	@arg 8
 * @retval None
 */
void leg7segOn(uint8_t index)
{
	if (index >= 6 && index <= 8)
	{
		spiBuffer |= 1 << (index - 6);
	}
}

/**
 * @brief  	Turn off one of three led at top-right corner of the board
 * @param  	index Index of the led
 *			This parameter can be one of the following values:
 *            	@arg 6
 *            	@arg 7
 *            	@arg 8
 * @retval None
 */
void leg7segOff(uint8_t index)
{
	if (index >= 6 && index <= 8)
	{
		spiBuffer &= ~(1 << (index - 6));
	}
}

/**
 * @brief   Toggle the colon led
 */
void toggleColon()
{
	spiBuffer ^= (1 << 3);
}
/**
 * @brief   Shift Right the number displayed on led 7-segment
 */
void shiftRightLed7()
{
	uint8_t temp = led7seg[3];
	for (int i = 3; i > 0; i--)
	{
		led7seg[i] = led7seg[i - 1];
	}
	led7seg[0] = temp;
}
void initTime(uint8_t h, uint8_t m)
{
	hour = h;
	minute = m;
	led7segSetDigit(hour / 10, 0, 0);
	led7segSetDigit(hour % 10, 1, 0);
	led7segSetDigit(minute / 10, 2, 0);
	led7segSetDigit(minute % 10, 3, 0);
}
void updateTime(void)
{
	minute++;
	if (minute >= 60)
	{
		minute = 0;
		hour++;
		if (hour >= 24)
		{
			hour = 0;
		}
	}
	led7segSetDigit(hour / 10, 0, 0);
	led7segSetDigit(hour % 10, 1, 0);
	led7segSetDigit(minute / 10, 2, 0);
	led7segSetDigit(minute % 10, 3, 0);
}