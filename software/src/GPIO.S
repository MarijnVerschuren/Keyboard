.syntax unified
.thumb

// include memory map defines with the C preprocessor
#include "memory_map.h"

// functions
.global GPIO_to_int
.global int_to_GPIO
.global enable_GPIO
.global disable_GPIO
.global reset_GPIO
.global fconfig_GPIO
.global aconfig_GPIO
.global config_GPIO
.global GPIO_write
.global GPIO_toggle
.global GPIO_read


/*!<
 * functions
 * */
.section .text.GPIO_to_int
.type GPIO_to_int, %function
GPIO_to_int:
	ldr r1, =GPIOA_BASE
	sub r0, r0, r1
	lsr r0, #10
	and r0, #0b111
	bx lr
.size GPIO_to_int, .-GPIO_to_int

.section .text.int_to_GPIO
.type int_to_GPIO, %function
int_to_GPIO:
	ldr r1, =GPIOA_BASE
	and r0, #0b111
	lsl r0, #10
	add r0, r0, r1
	bx lr
.size int_to_GPIO, .-int_to_GPIO

.section .text.enable_GPIO
.type enable_GPIO, %function
enable_GPIO:
	mov r2, lr
	bl GPIO_to_int
	mov r1, #1
	lsl r0, r1, r0
	ldr ip, =RCC_BASE
	ldr r1, [ip, RCC_AHB1ENR]
	orr r1, r1, r0 
	str r1, [ip, RCC_AHB1ENR]
	bx r2
.size enable_GPIO, .-enable_GPIO

.section .text.disable_GPIO
.type disable_GPIO, %function
disable_GPIO:
	mov r2, lr
	bl GPIO_to_int
	mov r1, #1
	lsl r0, r1, r0
	ldr ip, =RCC_BASE
	ldr r1, [ip, RCC_AHB1ENR]
	bic r1, r1, r0
	str r1, [ip, RCC_AHB1ENR]
	bx r2
.size disable_GPIO, .-disable_GPIO

.section .text.reset_GPIO
.type reset_GPIO, %function
reset_GPIO:
	push {r4, r5}
	mov ip, r0
	ldmia ip, {r2-r5}
	mov r0, #0b11
	lsl r0, r0, r1
	bic r3, r3, r0
	lsl r0, r0, r1
	bic r2, r2, r0
	bic r4, r4, r0
	bic r5, r5, r0
	stm ip, {r2-r5}
	pop {r4, r5}
	bx lr
.size reset_GPIO, .-reset_GPIO

.section .text.fconfig_GPIO
.type fconfig_GPIO, %function
fconfig_GPIO:
	push {r0-r3, lr}		// save R0-R3 and LR temporarily
	bl enable_GPIO			// call enable_GPIO, R0-R2 destroyed
	ldmia sp, {r0, r1}		// restore R0 and R1 (port, pin)
	bl reset_GPIO			// call reset_GPIO, R0-R3 destoyed
	pop {r0-r3, lr}			// restore R0-R3 and LR
	push {r4-r7}			// save non volatile registers
	mov ip, r0				// load port ptr to free R0
	ldmia ip, {r4-r7}		// load registers
	ubfx r0, r2, #6, #1		// load 'type' setting into R0
	lsl r0, r0, r1			// shift 'type' setting according to the pin number
	orr r5, r5, r0			// write 'type' setting
	lsl r1, r1, #1			// R1(pin) *= 2 to index 2bit mapped registers
	ubfx r0, r2, #0, #2		// load 'mode' setting
	lsl r0, r0, r1			// shift 'mode' setting according to the pin number
	orr r4, r4, r0			// write 'mode' setting
	ubfx r0, r2, #2, #2		// load 'pull' setting
	lsl r0, r0, r1			// shift 'pull' setting according to the pin number
	orr r7, r7, r0			// write 'pull' setting
	ubfx r0, r2, #4, #2		// load 'speed' setting
	lsl r0, r0, r1			// shift 'speed' setting according to the pin number
	orr r6, r6, r0			// write 'speed' setting
	stm ip, {r4-r7}			// store registers
	lsl r0, r1, #1			// R0 = R1(pin) * 2 to index 4bit mapped registers
	cmp r1, #0x10			// check if AF setting for pin resides in AF_L or AF_H
	itee lo					// start IT block based on the carry flag
	addcc ip, #0x20			// set ip to AFR[0] if 'pin' < 8
	addcs ip, #0x24			// set ip to AFR[1] if 'pin' >= 8
	subcs r0, #0x20			// subtract 0x20 if 'pin' >= 8 to get the offset in AFR[1]
	ldr r2, [ip]			// load AF register
	lsl r3, r3, r0			// shift the 'AF' setting according to the pin number
	orr r2, r2, r3			// write the 'AF' setting
	str r2, [ip]			// store AF register
	pop {r4-r7}				// restore non volatile registers
	bx lr
.size fconfig_GPIO, .-fconfig_GPIO

.section .text.config_GPIO
.type config_GPIO, %function
config_GPIO:
	mov r4, lr				// save LR
	mov r3, #0x00			// AF = 0x00
	bl fconfig_GPIO
	bx r4
.size config_GPIO, .-config_GPIO

.section .text.GPIO_write
.type GPIO_write, %function
GPIO_write:
	ldr r3, [r0, GPIO_BSRR]
	cmp r2, #0
	it eq
	addeq r1, #0x10
	mov r2, #1
	lsl r2, r2, r1
	orr r3, r2
	str r3, [r0, GPIO_BSRR]
	bx lr
.size GPIO_write, .-GPIO_write

.section .text.GPIO_toggle
.type GPIO_toggle, %function
GPIO_toggle:
	ldr r2, [r0, GPIO_ODR]
	mov r3, #1
	lsl r3, r3, r1
	eor r2, r2, r3
	str r2, [r0, GPIO_ODR]
	bx lr
.size GPIO_toggle, .-GPIO_toggle

.section .text.GPIO_read
.type GPIO_read, %function
GPIO_read:
	ldr r2, [r0, GPIO_IDR]
	lsr r0, r0, r1
	bx lr
.size GPIO_read, .-GPIO_read
