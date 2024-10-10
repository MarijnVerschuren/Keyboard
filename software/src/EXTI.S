.syntax unified
.thumb

// include memory map defines with the C preprocessor
#include "memory_map.h"	// define include

// functions
.global config_EXTI 
.global config_EXTI_GPIO
.global start_EXTI


/*!<
 * functions
 * */
// static uint32_t EXTI_to_IRQn(uint8_t);
.section .text.EXTI_to_IRQn
.type EXTI_to_IRQn, %function
EXTI_to_IRQn:
	cmp r0, #5					// line 5
	itt lo
	addcc r0, r0, #6			// += EXTI0_IRQn
	bxcc lr						// return
	cmp r0, #10					// line 10
	itt lo
	movcc r0, #23				// = EXTI9_5_IRQn
	bxcc lr						// return
	cmp r0, #16					// line 16
	itt lo
	movcc r0, #40				// = EXTI15_10_IRQn
	bxcc lr						// return
	itt eq
	moveq r0, #1				// = PVD_IRQn
	bxeq lr						// return
	cmp r0, #17					// line 17
	itt eq
	moveq r0, #41				// = RTC_ALARM_IRQn
	bxeq lr						// return
	cmp r0, #18					// line 18
	itt eq
	moveq r0, #42				// = OTG_FS_WKUP_IRQn
	bxeq lr						// return
	cmp r0, #21					// line 21
	itt eq
	moveq r0, #2				// = RTC_STAMP_IRQn
	bxeq lr						// return
	cmp r0, #22					// line 22
	itt eq
	moveq r0, #3				// = RTC_WKUP_IRQn
	bxeq lr						// return
	mov r0, #0					// error
	bx lr						// return

/*!<
 * functions
 * */
//void config_EXTI(uint8_t, uint8_t, uint8_t);
.section .text.config_EXTI
.type config_EXTI, %function
config_EXTI:
	ldr ip, =EXTI_BASE			// add EXTI_BASE
	add ip, ip, EXTI_RTSR		// add RTSR offset
	lsl r1, r1, r0				// shift rising edge trigger setting
	lsl r0, r2, r0				// shift falling edge trigger setting
	ldmia ip, {r2, r3}			// load RTSR and FTSR
	orr r2, r2, r1				// set rising edge trigger setting
	orr r3, r3, r0				// set falling edge trigger setting
	stm ip, {r2, r3}			// store RTSR and FTSR
	orr r0, r0, r1				// create enable mask from both triggers
	ldr r1, [ip, -EXTI_RTSR]	// load IMR
	orr r1, r1, r0				// set interrupt enable bit
	str r1, [ip, -EXTI_RTSR]	// store IMR
	bx lr						// return
.size config_EXTI, .-config_EXTI


// void config_EXTI_GPIO(GPIO_t*, uint8_t, uint8_t, uint8_t);
.section .text.config_EXTI_GPIO
.type config_EXTI_GPIO, %function
config_EXTI_GPIO:
	push {r1-r3, lr}			// push arguments and LR to the stack
	ldr ip, =RCC_BASE			// load RCC_BASE
	mov r2, #0x00004000			// load SYSCFG enable mask
	ldr r3, [ip, RCC_APB2ENR]	// load APB2ENR
	orr r3, r3, r2				// add mask
	str r3, [ip, RCC_APB2ENR]	// store APB2ENR
	and r2, r1, #0b11			// bit	index
	lsl r2, r2, #2				// bit	index
	lsr r3, r1, #2				// byte	index
	bl GPIO_to_int				// port (int)
	lsl r0, r0, r2				// EXTICR setting mask
	mov r1, #0xF				// EXTICR reset mask
	lsl r1, r1, r2				// EXTICR reset mask
	ldr ip, =SYSCFG_BASE		// load SYSCFG_BASE
	add ip, SYSCFG_EXTICR		// add EXTICR offset
	ldr r2, [ip, r3]			// load EXTICR register
	bic r2, r2, r1				// clear current setting
	orr r2, r2, r0				// set setting
	str r2, [ip, r3]			// store EXTICR register
	pop {r0-r2}					// restore arguments
	bl config_EXTI				// call config_EXTI
	pop {lr}					// restore LR
	bx lr						// return
.size config_EXTI_GPIO, .-config_EXTI_GPIO


// void start_EXTI(uint8_t);
.section .text.start_EXTI
.type start_EXTI, %function
start_EXTI:
	mov r3, lr
	bl EXTI_to_IRQn
	bl NVIC_enable_IRQ
	bx r3
.size start_EXTI, .-start_EXTI


// void stop_EXTI(uint8_t);
.section .text.stop_EXTI
.type stop_EXTI, %function
stop_EXTI:
	mov r3, lr
	bl EXTI_to_IRQn
	bl NVIC_disable_IRQ
	bx r3
.size stop_EXTI, .-stop_EXTI

