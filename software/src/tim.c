//
// Created by marijn on 9/24/24.
//

#include "tim.h"


/*!< static */
static inline uint32_t TIM_to_update_IRQn(TIM_t* tim) {
	if (tim == TIM1)											{ return TIM1_UP_TIM10_IRQn; }
	if ((((uint32_t)tim) - APB1PERIPH_BASE) >= 0x00010000UL)	{ return (((uint32_t)(tim - APB2PERIPH_BASE) >> 10u) & 0xfu) + TIM1_BRK_TIM9_IRQn; }
	uint32_t irqn = (((uint32_t)(tim - AHB1PERIPH_BASE) >> 10u) & 0xfu) + TIM2_IRQn;
	if (irqn > TIM4_IRQn)										{ return TIM5_IRQn; }
	return irqn;
}
static inline void enable_TIM(TIM_t* tim) {
	if ((((uint32_t)tim) - APB1PERIPH_BASE) >= 0x00010000UL)	{ RCC->APB2ENR |= (0b1u << (((uint32_t)tim - APB2PERIPH_BASE) >> 10u)); }
	else														{ RCC->APB1ENR |= (0b1u << (((uint32_t)tim - APB1PERIPH_BASE) >> 10u)); }
}


/*!< init / disable */
void config_TIM(TIM_t* tim, uint32_t prescaler, uint32_t limit) {
	enable_TIM(tim);
	tim->PSC = prescaler;
	tim->ARR = limit;
	tim->EGR = 0x00000001UL;	// update shadow registers
}
void config_TIM_master(TIM_t* tim, uint32_t prescaler, uint32_t limit, uint32_t flags) {
	enable_TIM(tim);
	tim->PSC = prescaler;
	tim->ARR = limit;

	tim->CR2 = (tim->CR2 & ~0x00000070UL) | (flags);
	tim->SMCR &= ~0x00000080UL;	// master mode

	tim->EGR = 0x00000001UL;	// update shadow registers

}
void config_TIM_slave(TIM_t* tim, uint32_t prescaler, uint32_t limit, uint32_t flags) {
	enable_TIM(tim);
	tim->PSC = prescaler;
	tim->ARR = limit;
	// TODO
	tim->SMCR |= 0x00000080UL;	// slave mode

	tim->EGR = 0x00000001UL;	// update shadow registers

}
void disable_TIM(TIM_t* tim) {
	if ((((uint32_t)tim) - APB1PERIPH_BASE) >= 0x00010000UL)	{ RCC->APB2ENR &= ~(0b1u << (((uint32_t)tim - APB2PERIPH_BASE) >> 10u)); }
	else														{ RCC->APB1ENR &= ~(0b1u << (((uint32_t)tim - APB1PERIPH_BASE) >> 10u)); }
}

/*!< actions */
void start_TIM(TIM_t* tim)		{ tim->CR1 |= 0x1UL; }
void stop_TIM(TIM_t* tim)		{ tim->CR1 &= ~0x1UL; }

/*!< irq */
void start_TIM_update_irq(TIM_t* tim) {
	uint32_t irqn = TIM_to_update_IRQn(tim);
	NVIC_enable_IRQ(irqn);
	tim->DIER |= 0x1UL;
}
void stop_TIM_update_irq(TIM_t* tim) {
	NVIC_disable_IRQ(TIM_to_update_IRQn(tim));
	tim->DIER &= ~0x1UL;
}
