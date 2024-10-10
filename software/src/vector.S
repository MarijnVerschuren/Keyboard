.syntax unified
.thumb

.global IVT


.section .vector, "a", %progbits
.type IVT, %object
IVT:
.word _stack_end
.word reset_handler
.word NMI_handler
.word hard_fault_handler
.word mem_fault_handler
.word bus_fault_handler
.word exception_handler
.word 0
.word 0
.word 0
.word 0
.word SVC_handler
.word debug_handler
.word 0
.word pending_SV_handler
.word sys_tick_handler
// TODO: more interrupts
.word 0				// WWDG_IRQn
.word 0				// PVD_IRQn
.word RTC_stamp_handler
.word 0				// RTC_WKUP_IRQn
.word 0				// FLASH_IRQn
.word 0				// RCC_IRQn
.word EXTI0_handler
.word EXTI1_handler
.word EXTI2_handler
.word EXTI3_handler
.word EXTI4_handler
.word 0				// DMA1_stream0_IRQn
.word 0				// DMA1_stream1_IRQn
.word 0				// DMA1_stream2_IRQn
.word 0				// DMA1_stream3_IRQn
.word 0				// DMA1_stream4_IRQn
.word 0				// DMA1_stream5_IRQn
.word 0				// DMA1_stream6_IRQn
.word ADC_handler
.word 0				// CAN1_TX_IRQn
.word 0				// CAN1_RX0_IRQn
.word 0				// CAN1_RX1_IRQn
.word 0				// CAN1_SCE_IRQn
.word EXTI5_9_handler
.word 0				// TIM1_BRK_TIM9_IRQn
.word TIM1_update_handler


.size IVT, .-IVT



/*!<
 * functions
 * */
.section .text.default_handler, "ax", %progbits
default_handler:
	bkpt #1
	b default_handler
.size default_handler, .-default_handler



/*!<
 * aliases
 * */
.weak RTC_stamp_handler
.thumb_set RTC_stamp_handler,	RTC_tamper_handler
.weak TIM1_update_handler
.thumb_set TIM1_update_handler,	TIM10_handler



/*!<
 * weak definitions
 * */
.weak NMI_handler
.thumb_set NMI_handler,			default_handler
.weak hard_fault_handler
.thumb_set hard_fault_handler,	default_handler
.weak mem_fault_handler
.thumb_set mem_fault_handler,	default_handler
.weak bus_fault_handler
.thumb_set bus_fault_handler,	default_handler
.weak exception_handler
.thumb_set exception_handler,	default_handler
.weak SVC_handler
.thumb_set SVC_handler,			default_handler
.weak debug_handler
.thumb_set debug_handler,		default_handler
.weak pending_SV_handler
.thumb_set pending_SV_handler,	default_handler
.weak sys_tick_handler
.thumb_set sys_tick_handler,	default_handler

.weak RTC_tamper_handler		// RTC_stamp_hander
.thumb_set RTC_tamper_handler,	default_handler
.weak EXTI0_handler
.thumb_set EXTI0_handler,		default_handler
.weak EXTI1_handler
.thumb_set EXTI1_handler,		default_handler
.weak EXTI2_handler
.thumb_set EXTI2_handler,		default_handler
.weak EXTI3_handler
.thumb_set EXTI3_handler,		default_handler
.weak EXTI4_handler
.thumb_set EXTI4_handler,		default_handler
.weak EXTI5_9_handler
.thumb_set EXTI5_9_handler,		default_handler

.weak ADC_handler
.thumb_set ADC_handler,			default_handler

.weak TIM10_handler				// TIM1_update_handler
.thumb_set TIM10_handler,		default_handler
