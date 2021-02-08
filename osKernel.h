#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H

#include <stdint.h>
#include "stm32f4xx.h"

void osKernelInit(void);
void osKernelLaunch(uint32_t quanta);
uint8_t osKernelAddThread(void (*task0)(void), void (*task1)(void),void (*task2)(void));

uint8_t osKernelAddThread(
		void(*task0)(void),
		void(*task1)(void),
		void(*task2)(void));

#endif
