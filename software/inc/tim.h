//
// Created by marijn on 9/24/24.
//

#ifndef STM32F412_TIM_H
#define STM32F412_TIM_H
#include "periph.h"
#include "NVIC.h"


typedef enum {
	TIM_PIN_DISABLE =	0x00000000,
	// TIM1
	// TODO
} TIM_GPIO_t;

typedef enum {
	TIM_TRGO_RESET =	0b000 << 4,
	TIM_TRGO_ENABLE =	0b001 << 4,
	TIM_TRGO_UPDATE =	0b010 << 4,
	TIM_TRGO_CC1IF =	0b011 << 4,
	TIM_TRGO_OC1REF =	0b100 << 4,
	TIM_TRGO_OC2REF =	0b101 << 4,
	TIM_TRGO_OC3REF =	0b110 << 4,
	TIM_TRGO_OC4REF =	0b111 << 4
} TIM_TRGO_t;

//typedef enum {
//	TIM_TRGO_RESET =	0b0 << 7,
//	TIM_TRGO_RESET =	0b0 << 7
//} TIM_TI1_t;



/*!< init / enable / disable */
void config_TIM(TIM_t* tim, uint32_t prescaler, uint32_t limit);
void config_TIM_master(TIM_t* tim, uint32_t prescaler, uint32_t limit, uint32_t flags);	// TODO: config channels
void config_TIM_slave(TIM_t* tim, uint32_t prescaler, uint32_t limit, uint32_t flags);	// TODO: config trigger
void disable_TIM(TIM_t* tim);
/*!< actions */
void start_TIM(TIM_t* tim);
void stop_TIM(TIM_t* tim);
/*!< irq */
void start_TIM_update_irq(TIM_t* tim);
void stop_TIM_update_irq(TIM_t* tim);


#endif //STM32F412_TIM_H
