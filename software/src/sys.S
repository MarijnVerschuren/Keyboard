.syntax unified
.thumb

// include memory map defines with the C preprocessor
#include "memory_map.h"

// objects
.global tick
.global sys_config
// functions
.global sys_reset
.global sys_tick_handler
.global delay_ms

/*!<
 * variables
 * */
.section .data.tick
.type tick, %object				/* uint64_t tick; */
tick: .quad 0x0000000000000000
.size tick, .-tick

.section .data.sys_config
.type sys_config, %object
sys_config:
.word 0x00000000
.word 0x00000000
.word 0x00000000
.size sys_config, .-sys_config


/*!<
 * functions
 * */
.section .text.sys_reset
.type sys_reset, %function		/* void sys_reset(void); */
sys_reset:
	push {r4, r5}
	ldr r0, =RCC_BASE			// save RCC BASE
	ldr r1, =FLASH_BASE			// save FLASH BASE
	ldr r2, =SCB_BASE			// save SCB_BASE
	ldr r3, =PWR_BASE			// save PWR_BASE

	// RCC (enable HSI, set HSI as sys_clk, reset other config)
	mov r4, #0x00000081			// HSI enable, HSI_trim: 0x10
	strb r4, [r0, RCC_CR]		// save CR[0:8]
	// reset RCC_CFGR (select HSI as sys clk)
	mov r5, #0x00000000			// zero
	str r5, [r0, RCC_CFGR]		// reset CFGR
	// reset RCC_CR
	str r4, [r0, RCC_CR]		// clear CR[8:32]
	// reset RCC_CIR
	str r5, [r0, RCC_CIR]		// clear CIR
	// enable core devices
	mov r4, #0x10000000
	str r4, [r0, RCC_APB1ENR]	// enable PWR peripheral

	
	// FLASH (update flash latency for 16MHz)
	mov r4, #0x00				// default FLASH latency at 16MHz
	strb r4, [r1, FLASH_ACR]	// save ACR[0:8]
	
	// SCB (enable FPU)
	mov r4, #0x00F00000			// CP10 and CP11 mask
	ldr r5, [r2, SCB_CPACR]		// load CPACR
	orr r5, r4					// or value with mask
	str r5, [r2, SCB_CPACR]		// store CPACR

	// PWR (update VOS scale for 16MHz)
	mov r4, #0x00004000			// VOS scale 3 setting (mininum nominal)
	str r4, [r3, PWR_CR]		// save CR
	
	pop {r4-r5}
	bx lr						// return
.size sys_reset, .-sys_reset


.section .text.sys_tick_handler
.type sys_tick_handler, %function	/* void sys_tick_handler(void); */
sys_tick_handler:
	ldr ip, =tick				// load tick ptr 
	ldmia ip, {r0-r1}			// load multiple src  (2 * 32-bit)
	adds r0, r0, #1				// inc low word
	adc  r1, r1, #0				// add carry to high word
	stm ip, {r0-r1}				// store multiple dst (2 * 32-bit)
	bx lr						// return
.size sys_tick_handler, .-sys_tick_handler


.section .text.delay_ms
.type delay_ms, %function			/* void delay_ms(uint64_t); */
delay_ms:
	push {r4, r5}
	ldr ip, =tick
	ldmia ip, {r2, r3}
.delay_loop:	
	ldmia ip, {r4, r5}
	subs r4, r4, r2
	sbc  r5, r5, r3
	cmp  r4, r0
	sbcs r5, r5, r1
	bcc .delay_loop
	pop {r4, r5}
	bx lr
.size delay_ms, .-delay_ms

