.syntax unified
.thumb

// include memory map defines with the C preprocessor
#include "memory_map.h"	// define include
.include "base.s"		// macro include

// externals
.global APB1_clock_frequency
.global APB2_clock_frequency

// functions
.global enable_USART
.global disable_USART
.global reset_USART
.global fconfig_UART
.global config_UART
// TODO: USART init
.global USART_write
.global USART_read
.global USART_print



/*!<
 * functions
 * */
 // void enable_USART(USART_t*);
.section .text.enable_USART
.type enable_USART, %function
enable_USART:
	ldr ip, =RCC_BASE

	ldr r1, =APB2PERIPH_BASE
	cmp r0, r1
	ite lo
	subcc r0, r0, APB1PERIPH_BASE		// subtract APB1 base
	subcs r0, r0, r1					// subtract APB2 base
	lsr r0, r0, #10

	ite lo
	addcc ip, RCC_APB1ENR
	addcs ip, RCC_APB2ENR
	mov r2, #1
	lsl r2, r2, r0
	ldr r1, [ip]
	orr r1, r1, r2
	str r1, [ip]
	bx lr
.size enable_USART, .-enable_USART


// void disable_USART(USART_t*);
.section .text.disable_USART
.type disable_USART, %function
disable_USART:
	ldr ip, =RCC_BASE

	ldr r1, =APB2PERIPH_BASE
	cmp r0, r1
	ite lo
	subcc r0, r0, APB1PERIPH_BASE		// subtract APB1 base
	subcs r0, r0, r1					// subtract APB2 base
	lsr r0, r0, #10

	ite lo
	addcc ip, RCC_APB1ENR
	addcs ip, RCC_APB2ENR
	mov r2, #1
	lsl r2, r2, r0
	ldr r1, [ip]
	bic r1, r1, r2
	str r1, [ip]
	bx r3
.size disable_USART, .-disable_USART


// void reset_USART(USART_t*);
.section .text.reset_USART
.type reset_USART, %function
reset_USART:
	add ip, r0, USART_BRR
	ldmia ip, {r0, r1}
	mov r0, #0x00000000
	mov r1, #0x00000000
	stm ip, {r0, r1}
	bx lr
.size reset_USART, .-reset_USART


// void fconfig_USART(USART_GPIO_t, USART_GPIO_t, uint32_t, USART_OS_t);
.section .text.fconfig_UART
.type fconfig_UART, %function
fconfig_UART:
	push {r2-r7, lr}				// TODO
	mov r7, #0x00002008				// USART, TX enable TODO: r8
	// check pin inputs
	cmp r1, #0x00000000				// RX enabled?
	beq fconfig_simplex				// configure for TX only
	orr r8, r8, #0x00000004			// RX enable
	cmp r0, #0x00000000				// TX enabled?
	itt eq
	moveq r0, r1					// move R1 to R0
	biceq r8, r8, #0x00000008		// TX disable
	beq fconfig_simplex				// configure for RX only
	push {r1}						// save RX pin
	// configure TX
	dpin_alt r3, r0					// AF
	dpin_pin r4, r0					// pin
	dpin_port r0, r0				// port (int)
	bl int_to_GPIO					// port (GPIO_t*)
	mov r1, r4						// pin
	mov r2, #0b0000010				// GPIO_AF_MODE, NO_PULL, LOW_SPEED, PUSH_PULL
	bl fconfig_GPIO					// init pin
	pop {r0}						// load RX
fconfig_simplex:
	// calculate dev ptr
	dpin_clk r1, r0					// clock
	cmp r1, #0x1					// compare clock
	ittee lo
	ldrcc r1, =APB1_clock_frequency	// &APB1_clock_frequency
	ldrcc r5, =APB1PERIPH_BASE		// usart peripheral ptr base = APB1
	ldrcs r1, =APB2_clock_frequency	// &APB2_clock_frequency
	ldrcs r5, =APB2PERIPH_BASE		// usart peripheral ptr base = APB2
	ldr r6, [r1]					// APBx_clock_frequency
	dpin_periph r1, r0				// usart (int)
	add r5, r5, r1, lsl #10		// usart (USART_t*), R5 += usart(int) << 10
	// configure RX or TX (simplex)
	dpin_alt r3, r0					// AF
	dpin_pin r4, r0					// pin
	dpin_port r0, r0				// port (int)
	bl int_to_GPIO					// port (GPIO_t*)
	mov r1, r4						// pin
	mov r2, #0b0000010				// GPIO_AF_MODE, NO_PULL, LOW_SPEED, PUSH_PULL
	bl fconfig_GPIO					// init pin
	// enable usart
	mov r0, r5						// load USART into R0
	bl enable_USART					// enable USART
	// config
	pop {r3, r4}					// load baud and OS
	udiv r2, r6, r3					// divide APBx_clock_frequency by the baud setting
	lsl r1, r2, r4					// shift divisor by OS
	mov r0, #0xFFF0					// define upper part
	and r0, r1, r0					// sample upper part
	and r1, r1, #0xF				// sample lower nibble
	lsr r1, r1, r4					// shift lower nibble based on OS
	orr r0, r0, r1					// assemble baud setting
	str r0, [r5, USART_BRR]			// store baud setting in BRR
	str r7, [r5, USART_CR1]			// store enable flags in CR1
	pop {r4-r7, lr}					// restore non-volatile registers
	bx lr							// return
.size fconfig_UART, .-fconfig_UART


// void config_USART(USART_GPIO_t, USART_GPIO_t, uint32_t);
.section .text.config_UART
.type config_UART, %function
config_UART:
	mov r4, lr
	mov r3, #0b0
	bl fconfig_UART
	bx r4
.size config_UART, .-config_UART


// uint32_t USART_write(USART_t*, const uint8_t*, uint32_t, uint32_t);
.section .text.USART_write
.type USART_write, %function
USART_write:
	push {r4-r6}					// store non-volatile registers
	ldr r4, =tick					// load tick address
	ldr r5, [r4]					// load tick start
	mov ip, r0						// save USART_t*
	mov r0, 0x00000000				// use R0 as counter (and return count value)
USART_write_loop:
	cmp r0, r2						// compare counter with buffer size
	bcs USART_write_end				// break if counter >= size
USART_write_TXE_loop:
	ldr r6, [r4]					// load tick
	subs r6, r6, r5					// tick - start
	bcc USART_write_end				// tick < start (32-bit roll-over)
	cmp r6, r3						// compare timeout
	bcs USART_write_end				// (tick - start) > timeout
	ldr r6, [ip, USART_SR]			// load SR
	tst r6, #0x80					// TXE (TX data register empty)
	beq USART_write_TXE_loop		// loop if TXE not set
	ldrb r6, [r1, r0]				// load byte from buffer
	strb r6, [ip, USART_DR]			// write byte to DR
	add r0, r0, #1					// increment R0
	b USART_write_loop				// loop
USART_write_end:
	pop {r4-r6}						// restore non-volatile registers
	bx lr							// return
.size USART_write, .-USART_write


// uint32_t USART_read(USART_t*, const uint8_t*, uint32_t, uint32_t);
.section .text.USART_read
.type USART_read, %function
USART_read:
	push {r4-r6}					// store non-volatile registers
	ldr r4, =tick					// load tick address
	ldr r5, [r4]					// load tick start
	mov ip, r0						// save USART_t*
	mov r0, 0x00000000				// use R0 as counter (and return count value)
USART_read_loop:
	cmp r0, r2						// compare counter with buffer size
	bcs USART_read_end				// break if counter >= size
USART_read_RXNE_loop:
	ldr r6, [r4]					// load tick
	subs r6, r6, r5					// tick - start
	bcc USART_read_end				// tick < start (32-bit roll-over)
	cmp r6, r3						// compare timeout
	bcs USART_read_end				// (tick - start) > timeout
	ldr r6, [ip, USART_SR]			// load SR
	tst r6, #0x20					// RXNE (RX data register not empty)
	beq USART_read_RXNE_loop		// loop if RXNE not set
	ldrb r6, [ip, USART_DR]			// load byte from DR
	strb r6, [r1, r0]				// write byte to buffer
	add r0, r0, #1					// increment R0
	b USART_read_loop				// loop
USART_read_end:
	pop {r4-r6}						// restore non-volatile registers
	bx lr							// return
.size USART_read, .-USART_read


// uint32_t USART_write(USART_t*, const uint8_t*, uint32_t);
.section .text.USART_print
.type USART_print, %function
USART_print:
	push {r4-r5}					// store non-volatile registers
	ldr r4, =tick					// load tick address
	ldr r5, [r4]					// load tick start
	mov ip, r0						// save USART_t*
	mov r0, 0x00000000				// use R0 as counter (and return count value)
USART_print_loop:
	ldr r3, [r1, r0]				// load current char
	cmp r3, #0x00					// check char
	beq USART_print_end				// break if char is null terminator
USART_print_TXE_loop:
	ldr r6, [r4]					// load tick
	subs r6, r6, r5					// tick - start
	bcc USART_print_end				// tick < start (32-bit roll-over)
	cmp r6, r2						// compare timeout
	bcs USART_print_end				// (tick - start) > timeout
	ldr r6, [ip, USART_SR]			// load SR
	tst r6, #0x80					// TXE (TX data register empty)
	beq USART_print_TXE_loop		// loop if TXE not set
	strb r3, [ip, USART_DR]			// write byte to DR
	add r0, r0, #1					// increment R0
	b USART_print_loop				// loop
USART_print_end:
	pop {r4-r5}						// restore non-volatile registers
	bx lr							// return
.size USART_print, .-USART_print



