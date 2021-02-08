	.cpu cortex-m4
	.text
	.align 2
	.syntax unified
	.extern currentPt
	.global SysTick_Handler
	.global osSchedulerLaunch

SysTick_Handler:				@ Invocking interrupt saves automatically the stack frame(r0,r1,r2,r3,r12,lr,pc,psr) onto the stack
	cpsid		i			@ Disable Interrupts
	push		{R4-R11}		@ Save registers r4-r11 onto the stack
	ldr		r0, =currentPt		@ Load currentPt to R0
	ldr		r1, [r0]		@ r1 = currentPt
	str		sp, [r1]		@
	ldr		r1, [r1,#4]		@ r1 = currentPt->next
	str		r1, [r0]		@ currentPt = r1
	ldr		sp, [r1]		@ SP = currentPt = r1
	pop		{r4-r11}		@ Restore r4-r11
	cpsie		i			@ Enable Interrupts
	bx		lr			@ Return. Restores automatically the stack frame(r0,r1,r2,r3,r12,lr,pc,psr) from the stack

osSchedulerLaunch:
	ldr		r0, =currentPt		@
	ldr		r2, [r0]		@ R2 = currentPt
	ldr		sp, [r2]		@ SP = currentPt->stackPt
	pop		{r4-r11}		@ Restore registers r4-r11 from stack
	pop		{r0-r3}			@ Restore registers r0-r3 from stack
	pop		{r12}			@ Restore register R12 from stack
	add		sp,sp,#4		@
	pop		{lr}			@
	add		sp,sp,#4		@
	cpsie		i			@ Enable Interrupts
	bx		lr			@

	.align
	.end
