.syntax unified
.thumb

// include memory map defines with the C preprocessor
#include "memory_map.h"	// define include

// functions
.global NVIC_enable_IRQ
.global NVIC_disable_IRQ
.global NVIC_set_IRQ_priority



/*!<
 * functions
 * */
//void NVIC_enable_IRQ(IRQn_t);
.section .text.NVIC_enable_IRQ
.type NVIC_enable_IRQ, %function
NVIC_enable_IRQ:
	lsr r1, r0, #5				// byte index in the ISER register
	and r0, r0, #0x1F			// bit	index in the ISER register
	mov r2, #1					// load bit
	lsl r0, r2, r0				// shift bit
	ldr ip, =NVIC_BASE			// load NVIC base into IP
	str r0, [ip, NVIC_ISER]		// set IRQ enable bit
	bx lr						// return
.size NVIC_enable_IRQ, .-NVIC_enable_IRQ


//void NVIC_disable_IRQ(IRQn_t);
.section .text.NVIC_disable_IRQ
.type NVIC_disable_IRQ, %function
NVIC_disable_IRQ:
	lsr r1, r0, #5				// byte index in the ICER register
	and r0, r0, #0x1F			// bit	index in the ICER register
	mov r2, #1					// load bit
	lsl r0, r2, r0				// shift bit
	ldr ip, =NVIC_BASE			// load NVIC base into IP
	str r0, [ip, NVIC_ICER]		// set IRQ enable bit
	dsb	sy						// wait until write complete
	isb	sy						// wait until the CPU pipeline is clear
	bx lr						// return
.size NVIC_disable_IRQ, .-NVIC_disable_IRQ


// void NVIC_set_IRQ_priority(IRQn_t, uint8_t);
.section .text.NVIC_set_IRQ_priority
.type NVIC_set_IRQ_priority, %function
NVIC_set_IRQ_priority:
	ldr ip, =NVIC_BASE			// load NVIC base into IP
	add ip, ip, NVIC_IP			// add IP register offset
	lsl r1, r1, #4				// shift priority
	strb r1, [ip, r0]			// store priority in the IP register
	bx lr						// return
.size NVIC_set_IRQ_priority, .-NVIC_set_IRQ_priority



