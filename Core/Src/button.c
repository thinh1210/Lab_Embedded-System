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
uint8_t lastState[16] = {0};
static uint16_t buttonSpiBuffer = 0x0000;
uint8_t buttonPos[16] = {14, 0, 1, 2, 7, 6, 5, 8, 9, 10, 3, 4, 11, 12, 15, 13};
uint8_t buttonInx = 0;
uint8_t longPressFlag = 0;
uint8_t longPressTrigger = 0;
static uint8_t g_longPressFlag[16] = {0};
static uint16_t g_counterTime[16] = {0};

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
uint16_t buttonGetState(BUTTON pos)
{
    uint8_t stateIndex = buttonPos[pos];
    return buttonCount[stateIndex];
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

    buttonInx = 0;
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
        {
            buttonCount[buttonInx] = 0;
            lastState[buttonInx] = 0;
            longPressFlag = 0;
            longPressTrigger = 0;
        }
        else
            buttonCount[buttonInx]++;
        mask = mask >> 1;
    }

    checkLongPress(BUTTON_UP);
}

/**
 * @brief  Kiểm tra nhấn giữ (long press) và lặp lại (repeat) cho một nút
 * @param  pos: Nút logic (ví dụ: BTN_UP, BTN_DOWN)
 * @note   Gọi hàm này trong vòng lặp chính cho MỖI nút bạn muốn kiểm tra.
 * Ngưỡng long press: 40 * 50ms = 2 giây
 * Tốc độ lặp lại: 4 * 50ms = 200ms
 */
void checkLongPress(BUTTON pos)
{
    // --- SỬA LỖI 1: Lấy đúng index vật lý của nút ---
    uint8_t stateIndex = buttonPos[pos];
    uint16_t upState = buttonCount[stateIndex];

    // 1. Kiểm tra nếu nút được thả ra (upState == 0)
    if (upState == 0)
    {
        g_longPressFlag[stateIndex] = 0;
        g_counterTime[stateIndex] = 0;
        return;
    }

    if (upState <= TIME_TO_LONG_PRESS)
    {
        return;
    }

    if (g_longPressFlag[stateIndex] == 0)
    {
        g_longPressFlag[stateIndex] = 1;
        g_counterTime[stateIndex] = upState;
        return;
    }

    if (upState - g_counterTime[stateIndex] >= 4)
    {
        longPressTrigger = 1;                // Kích hoạt trigger lặp lại
        g_counterTime[stateIndex] = upState; // Reset mốc thời gian
        return;
    }
}
uint8_t buttonFirstPress(uint8_t pos)
{
    uint8_t stateIndex = buttonPos[pos];
    if (lastState[stateIndex] == 1)
        return 0;
    uint16_t nowState = buttonCount[stateIndex];
    if (nowState < 1)
        return 0;
    lastState[stateIndex] = 1;
    return 1;
}