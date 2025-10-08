#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_
/*Include --------------------------------------------*/

#include <stdint.h>
#include <tim.h>
/*Define --------------------------------------------*/
/* Define time cycle*/
#define TIMER_CYCLE_2 1
#define TIMER_CYCLE_3 1
#define TIMER_CYCLE_4 1
#define MAX_TASK 	  5


/*Enum   --------------------------------------------*/

/*Struct --------------------------------------------*/
typedef struct{
	uint16_t period;
	uint16_t counter;
	uint8_t 	 active;
	uint8_t 	 flag;
	void (*callback)(void);
}Softwaretimer;

/*Variables------------------------------------------*/



extern Softwaretimer task[MAX_TASK];
//extern Softwaretimer task0;
extern uint8_t timer2Flag;
extern uint8_t timer3Flag;
extern uint8_t timer4Flag;

extern uint8_t globalFlag;

/*Function INIT timer */

/*Function -------------------------------------------*/

void timer2Init(void);
void timer3Init(void);
void timer4Init(void);


/*Function setup Timer*/
void timer1Setup(int ms);
void timer2Setup(int ms);
void timer3Setup(int ms);

void initfullTask(void);
void timerInit(uint8_t index, uint16_t period, uint16_t counter, void (*callback) (void));
void doTask();

#endif
