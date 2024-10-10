.syntax unified
.thumb

.global reset_handler


// memory layout constants
.section .data
.word _data_load
.word _data_start
.word _data_end
.word _bss_start
.word _bss_end
.word _heap_start


// reset_handler
.section .text.reset_handler
.type reset_handler, %function
reset_handler:
	ldr sp, =_stack_end				// set stack pointer
	bl sys_reset					// reset system state

	// copy_data
	ldr r0, =_data_load				// src
	ldr r1, =_data_start			// dst
	ldr r2, =_data_end				// end
	mov r3, #0						// idx
	b copy_data_condition			// start copy_data loop
copy_data:
	ldr r4, [r0, r3]				// load from	src+idx
	str r4, [r1, r3]				// store to	dst+idx
	add r3, r3, #4					// idx += 4
copy_data_condition:
	add r4, r1, r3					// dst+idx
	cmp r4, r2						// compare with end
	bcc copy_data					// branch if r4 < r2

	// zero_bss
	ldr r0, =_bss_start				// dst
	ldr r1, =_bss_end				// end
	mov r2, #0						// zero
	b zero_bss_condition			// start zero_bss loop
zero_bss:
	str r2, [r0]					// set to zero
	add r0, r0, #4					// dst += 4
zero_bss_condition:
	cmp r0, r1						// compare with end
	bcc zero_bss					// branch if dst < end

	bl main							// call main
	bx lr							// return
.size reset_handler, .-reset_handler

