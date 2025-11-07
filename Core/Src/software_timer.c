/*Include -----------------------------------------------------*/
#include "software_timer.h"
#include "tim.h"

/*Variables -----------------------------------------------------*/

uint8_t timer2Flag = 0;
uint16_t timer2Count = 0;
uint16_t timer2Mul = 0;

// uint8_t timer3Flag = 0;
// uint16_t timer3Count = 0;
// uint16_t timer3Mul = 0;
//
// uint8_t timer4Flag = 0;
// uint16_t timer4Count = 0;
// uint16_t timer4Mul = 0;

Softwaretimer task[MAX_TASK];

uint8_t globalFlag;
/**
 * @brief Init timer interupt
 * @param None
 * @return None
 */
/*Function -----------------------------------------------------*/

void timer2Init(void)
{
	HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @brief Setup timer interupt
 */

void timer2Setup(int ms)
{
	timer2Mul = ms / TIMER_CYCLE_2;
	timer2Flag = 0;
	timer2Count = timer2Mul;
}

void initfullTask(void)
{
	for (int i = 0; i < MAX_TASK; i++)
	{
		task[i].period = 0;
		task[i].counter = 0;
		task[i].callback = NULL;
		task[i].active = 0;
		task[i].flag = 0;
	}
}

/**
 * @brief	Init time
 * */
void timerInit(uint8_t index, uint16_t period, uint16_t counter, void (*callback)(void))
{

	if (index < MAX_TASK && index >= 0)
	{
		task[index].period = period;
		task[index].counter = counter;
		task[index].callback = callback;
		task[index].active = 1;
		task[index].flag = 0;
	}
}

/** @brief do task with flag
 * */
void doTask()
{

	if (globalFlag)
	{
		for (uint8_t i = 0; i < MAX_TASK; i++)
		{
			if (task[i].flag & task[i].active)
			{
				if (task[i].callback != NULL)
					task[i].callback();
				task[i].flag = 0;
			}
		}
		//		task0.callback();
		//		task0.flag=0;
	}
	globalFlag = 0;
}

void disableTask(uint8_t pos)
{
	if (pos < MAX_TASK && pos >= 0)
	{
		task[pos].active = 0;
	}
}
void enableTask(uint8_t pos)
{
	if (pos < MAX_TASK && pos >= 0)
	{
		task[pos].active = 1;
	}
}

/**
 * @brief  	Timer interrupt routine
 * @param  	htim TIM Base handle
 * @note	This callback function is called by default
 * @retval 	None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{
		if (timer2Count > 0)
		{
			timer2Count--;
			if (timer2Count == 0)
			{
				timer2Flag = 1;
				timer2Count = timer2Mul;
			}
		}
		for (uint8_t i = 0; i < MAX_TASK; i++)
		{
			if (task[i].active == 0)
				continue;
			task[i].counter -= 1;
			if (task[i].counter <= 0 && task[i].period > 0)
			{
				task[i].counter = task[i].period;
				task[i].flag = 1;
				globalFlag = globalFlag | task[i].flag;
			}
		}
		//		task0.counter--;
		//		if(task0.counter<=0){
		//			task0.flag=1;
		//			task0.counter=task0.period;
		//			globalFlag=task0.flag;
		//		}
	}
}
