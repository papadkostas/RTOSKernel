#include "osKernel.h"

#define SYSPRI3 (*((volatile uint32_t *)0xE000ED20))

void osSchedulerLaunch(void);

#define NUM_OF_THREADS 3
#define STACKSIZE 100

#define BUS_FREQ 16000000
uint32_t MILLIS_PRESCALER;

struct tcb {
	int32_t *stackPt;
	struct tcb *nextPt;
};

typedef struct tcb tcbType;

tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPt;

int32_t TCB_STACK[NUM_OF_THREADS][STACKSIZE];

void osKernelStackInit(int i) {
	tcbs[i].stackPt = &TCB_STACK[i][STACKSIZE - 16];
	/* Explicit state we're running into Thumb mode */
	TCB_STACK[i][STACKSIZE - 1] = 0x01000000;
}

uint8_t osKernelAddThread(void (*task0)(void), void (*task1)(void),
		void (*task2)(void)) {
	__disable_irq();

	tcbs[0].nextPt = &tcbs[1];
	tcbs[1].nextPt = &tcbs[2];
	tcbs[2].nextPt = &tcbs[0];

	/* Initialize the stack of task 0 */
	osKernelStackInit(0);
	/* Point the Program Counter(PC) to the function task0 */
	TCB_STACK[0][STACKSIZE - 2] = (int32_t) (task0);

	osKernelStackInit(1);
	TCB_STACK[1][STACKSIZE - 2] = (int32_t) (task1);

	osKernelStackInit(2);
	TCB_STACK[2][STACKSIZE - 2] = (int32_t) (task2);

	currentPt = &tcbs[0];
	__enable_irq();

	return 1;
}

void osKernelInit(void) {
	__disable_irq();
	MILLIS_PRESCALER = BUS_FREQ / 1000;
	__enable_irq();
}

void osKernelLaunch(uint32_t quanta) {
	SysTick->CTRL = 0;	//disable systick
	SysTick->VAL = 0;	//tick value to 0
	SYSPRI3 = (SYSPRI3 & 0x00FFFFFF) | 0xE0000000;	//set priority
	SysTick->LOAD = (quanta * MILLIS_PRESCALER) - 1;	//count down from quanta time(ms)
	SysTick->CTRL = 0x00000007;	//enable systick
	osSchedulerLaunch();
}

