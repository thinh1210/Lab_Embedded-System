#include <../Inc/led.h>

LED_STATE state = LED_ON;
uint8_t count= LED_ON_TIME;

TRAFFIC_STATE traffic= RED;
void ledOn(void)
{
    HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, SET);
}
void ledOff(void)
{
    HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, RESET);
}
void ledY0On(void)
{
	HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, SET);
}

void ledY0Off(void)
{
	HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, RESET);
}
void ledY1On(void)
{
	HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, SET);
}
void ledY1Off(void)
{
	HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, RESET);
}

void debugLed(void)
{
    switch (state)
    {

    case 1:
        ledOn();
        state = 1 - state;
        break;
    case 0:
        ledOff();
        state = 1 - state;
        break;
    default:
        break;
    }
}


void initEx2 (void){
	state=LED_ON;
	count=LED_ON_TIME;
	ledOn();
}

void fsmEx2(void)
{
	count-=1;
	if(count <= 0){
		switch(state){
		case LED_ON:
			ledOff();
			state=LED_OFF;
			count= LED_OFF_TIME;
			break;
		case LED_OFF:
			ledOn();
			state= LED_ON;
			count= LED_ON_TIME;
			break;
		}
	}
}
void redState(void){
	ledOn();
	ledY0Off();
	ledY1Off();
	count=RED_TIME;
}

void greenState(void){
	ledOff();
	ledY0On();
	ledY1Off();
	count=GREEN_TiME;
}
void yellowState(void){
	ledOff();
	ledY0Off();
	ledY1On();
	count=YELLOW_TIME;
}
void initEx3(void)
{
	traffic= RED;
	redState();

}
void fsmEx3(void)
{
	count--;
	if(count<=0){
		switch(traffic){
		case RED:
			traffic=GREEN;
			greenState();
			break;
		case GREEN:
			traffic=YELLOW;
			yellowState();
			break;
		case YELLOW:
			traffic= RED;
			redState();
			break;
		}
	}
}



