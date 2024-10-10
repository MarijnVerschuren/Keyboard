//
// Created by marijn on 9/25/24.
//

#ifndef STM32F412_ADC_H
#define STM32F412_ADC_H
#include "periph.h"
#include "GPIO.h"
#include "NVIC.h"


// TODO: add ADC_clk var


typedef enum {	// 64-bit
	// kernel clock prescaler
	ADC_CLK_DIV2 =				0b00ULL << 0,	// APB2 / 2
	ADC_CLK_DIV4 =				0b01ULL << 0,	// APB2 / 4
	ADC_CLK_DIV6 =				0b10ULL << 0,	// APB2 / 6
	ADC_CLK_DIV8 =				0b11ULL << 0,	// APB2 / 8
	// resolution
	ADC_RES_12B =				0b00ULL << 24,	// 15 cycles for conversion
	ADC_RES_10B =				0b01ULL << 24,	// 13 cycles for conversion
	ADC_RES_8B =				0b10ULL << 24,	// 11 cycles for conversion
	ADC_RES_6B =				0b11ULL << 24,	// 9 cycles for conversion
	// special channels
	ADC_FEATURE_VBAT =			0b01ULL << 6,	// enable dedicated Vbat channel
	ADC_FEATURE_TEMP_VREF =		0b10ULL << 6,	// enable dedicated Vref and temp channel
	// EOC mode
	ADC_EOC_SEQUENCE =			0b1ULL << 42,		// EOC set after group conversion
	ADC_EOC_SINGLE =			0b1ULL << 42,		// EOC set after single conversion
	// mode
	ADC_SCAN_MODE =				0b1ULL << 8,
	ADC_CONT_MODE =				0b1ULL << 33,		// continuous conversion mode
	// data align
	ADC_ALIGN_RIGHT =			0b0ULL << 43,
	ADC_ALIGN_LEFT =			0b1ULL << 43,
	// discontinuous
	ADC_DISC1 =					0b00001ULL << 11,
	ADC_DISC2 =					0b00101ULL << 11,
	ADC_DISC3 =					0b01001ULL << 11,
	ADC_DISC4 =					0b01101ULL << 11,
	ADC_DISC5 =					0b10001ULL << 11,
	ADC_DISC6 =					0b10101ULL << 11,
	ADC_DISC7 =					0b11001ULL << 11,
	ADC_DISC8 =					0b11101ULL << 11,
	ADC_INJ_DISC=				0b1ULL << 12,
	// injected trigger
	ADC_INJ_TRIG_TIM1_CC4 =		0b0000ULL << 48,
	ADC_INJ_TRIG_TIM1_TRGO =	0b0001ULL << 48,
	ADC_INJ_TRIG_TIM2_CC1 =		0b0010ULL << 48,
	ADC_INJ_TRIG_TIM2_TRGO =	0b0011ULL << 48,
	ADC_INJ_TRIG_TIM3_CC2 =		0b0100ULL << 48,
	ADC_INJ_TRIG_TIM3_CC4 =		0b0101ULL << 48,
	ADC_INJ_TRIG_TIM4_CC1 =		0b0110ULL << 48,
	ADC_INJ_TRIG_TIM4_CC2 =		0b0111ULL << 48,
	ADC_INJ_TRIG_TIM4_CC3 =		0b1000ULL << 48,
	ADC_INJ_TRIG_TIM4_TRGO =	0b1001ULL << 48,
	ADC_INJ_TRIG_TIM5_CC4 =		0b1010ULL << 48,
	ADC_INJ_TRIG_TIM5_TRGO =	0b1011ULL << 48,
	ADC_INJ_TRIG_TIM8_CC2 =		0b1100ULL << 48,
	ADC_INJ_TRIG_TIM8_CC3 =		0b1101ULL << 48,
	ADC_INJ_TRIG_TIM8_CC4 =		0b1110ULL << 48,
	ADC_INJ_TRIG_EXTI_15 =		0b1111ULL << 48,
	ADC_INJ_TRIG_MODE_RISING =	0b01ULL << 52,
	ADC_INJ_TRIG_MODE_FALING =	0b10ULL << 52,
	ADC_INJ_TRIG_MODE_BOTH =	0b11ULL << 52,
	// regular trigger
	ADC_TRIG_TIM1_CC1 =			0b0000ULL << 56,
	ADC_TRIG_TIM1_CC2 =			0b0001ULL << 56,
	ADC_TRIG_TIM1_CC3 =			0b0010ULL << 56,
	ADC_TRIG_TIM2_CC2 =			0b0011ULL << 56,
	ADC_TRIG_TIM2_CC3 =			0b0100ULL << 56,
	ADC_TRIG_TIM2_CC4 =			0b0101ULL << 56,
	ADC_TRIG_TIM2_TRGO =		0b0110ULL << 56,
	ADC_TRIG_TIM3_CC1 =			0b0111ULL << 56,
	ADC_TRIG_TIM3_TRGO =		0b1000ULL << 56,
	ADC_TRIG_TIM4_CC4 =			0b1001ULL << 56,
	ADC_TRIG_TIM5_CC1 =			0b1010ULL << 56,
	ADC_TRIG_TIM5_CC2 =			0b1011ULL << 56,
	ADC_TRIG_TIM5_CC3 =			0b1100ULL << 56,
	ADC_TRIG_TIM8_CC1 =			0b1101ULL << 56,
	ADC_TRIG_TIM8_TRGO =		0b1110ULL << 56,
	ADC_TRIG_EXTI_11 =			0b1111ULL << 56,
	ADC_TRIG_MODE_RISING =		0b01ULL << 60,
	ADC_TRIG_MODE_FALING =		0b10ULL << 60,
	ADC_TRIG_MODE_BOTH =		0b11ULL << 60
} ADC_CFG_FLAG_t;


typedef enum {
	ADC_WDG_SCAN_ALL =			0b0UL << 9,
	ADC_WDG_SCAN_SINGLE =		0b1UL << 9,
	ADC_WDG_TYPE_INJECTED =		0b01UL << 22,
	ADC_WDG_TYPE_REGULAR =		0b10UL << 22,
	ADC_WDG_TYPE_BOTH =			0b11UL << 22
} ADC_WDG_FLAG_t;


typedef enum {
	ADC_IRQ_EOC =				0b1UL << 5,
	ADC_IRQ_WDG =				0b1UL << 6,
	ADC_IRQ_JEOC =				0b1UL << 7,
	ADC_IRQ_OVR =				0b1UL << 26
} ADC_IRQ_FLAG_t;


typedef enum {
	ADC_SAMPLE_3_CYCLES =		0b000UL,
	ADC_SAMPLE_15_CYCLES =		0b001UL,
	ADC_SAMPLE_28_CYCLES =		0b010UL,
	ADC_SAMPLE_56_CYCLES =		0b011UL,
	ADC_SAMPLE_84_CYCLES =		0b100UL,
	ADC_SAMPLE_112_CYCLES =		0b101UL,
	ADC_SAMPLE_144_CYCLES =		0b110UL,
	ADC_SAMPLE_480_CYCLES =		0b111UL
} ADC_SAMPLE_TIME_t;


void config_ADC(uint64_t flags);
void config_ADC_watchdog(uint8_t channel, uint32_t flags, uint16_t low_threshold, uint16_t high_threshold);
void config_ADC_IRQ(uint8_t priority, uint32_t flags);

void config_ADC_GPIO_inj_channel(
	GPIO_t* port, uint8_t pin, ADC_SAMPLE_TIME_t sample_time, uint16_t value_offset, uint8_t index
);

void start_ADC(uint8_t regular, uint8_t injected);


#endif //STM32F412_ADC_H
