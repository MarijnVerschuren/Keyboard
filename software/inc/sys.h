//
// Created by marijn on 9/9/24.
//

#ifndef STM32F412_SYS_H
#define STM32F412_SYS_H
#include "periph.h"


/*!<
 * types
 * */
typedef enum {
	PWR_LVL_1v7 = 0b00U,	// 1v71	- 2v1
	PWR_LVL_2v1 = 0b01U,	// 2v1	- 2v4
	PWR_LVL_2v4 = 0b10U,	// 2v4	- 2v7
	PWR_LVL_2v7 = 0b11U,	// 2v7	- 3v6
	PWR_LVL_NOM =			PWR_LVL_2v7
} PWR_level_t;

typedef enum {
	PLL_CLK_SRC_HSI = 0b0U,
	PLL_CLK_SRC_HSE = 0b1U
} PLL_CLK_SRC_t;

typedef enum {
	PLL_P_DIV_2 = 0b00U,
	PLL_P_DIV_4 = 0b01U,
	PLL_P_DIV_6 = 0b10U,
	PLL_P_DIV_8 = 0b11U
} PLL_P_t;

typedef enum {
	SYS_CLK_SRC_HSI =	0b00U,
	SYS_CLK_SRC_HSE =	0b01U,
	SYS_CLK_SRC_PLL_P =	0b10U
} SYS_CLK_SRC_t;

typedef enum {
	AHB_CLK_NO_DIV =	0b0000U,
	AHB_CLK_DIV2 =		0b1000U,
	AHB_CLK_DIV4 =		0b1001U,
	AHB_CLK_DIV8 =		0b1010U,
	AHB_CLK_DIV16 =		0b1011U,
	AHB_CLK_DIV64 =		0b1100U,
	AHB_CLK_DIV128 =	0b1101U,
	AHB_CLK_DIV256 =	0b1110U,
	AHB_CLK_DIV512 =	0b1111U
} AHB_DIV_t;

typedef enum {
	APBx_CLK_NO_DIV =	0b000U,
	APBx_CLK_DIV2 =		0b100U,
	APBx_CLK_DIV4 =		0b101U,
	APBx_CLK_DIV8 =		0b110U,
	APBx_CLK_DIV16 =	0b111U
} APBx_DIV_t;

typedef enum {
	RTC_SRC_NONE =	0b00U,
	RTC_SRC_LSE =	0b01U,
	RTC_SRC_LSI =	0b10U,
	RTC_SRC_HSE =	0b11U
} RTC_SRC_t;


/*!<
 * constants
 * */
extern const uint32_t HSI_clock_frequency;
extern const uint32_t LSE_clock_frequency;
extern const uint32_t LSI_clock_frequency;


/*!<
 * clock variables
 * */
extern uint32_t HSE_clock_frequency;
extern uint32_t PLL_P_clock_frequency;
extern uint32_t PLL_Q_clock_frequency;
extern uint32_t PLL_R_clock_frequency;
extern uint32_t RTC_clock_frequency;
extern uint32_t SYS_clock_frequency;
extern uint32_t AHB_clock_frequency;
extern uint32_t APB1_clock_frequency;
extern uint32_t APB2_clock_frequency;

volatile extern uint64_t tick;


/*!<
 * functions
 * */
extern void sys_reset(void);
void sys_init(void);
extern void delay_ms(uint64_t ms);


/*!<
 * config functions
 * */
void set_SYS_hardware_config(PWR_level_t power_level, uint32_t HSE_frequency);
void set_SYS_oscilator_config(
	uint8_t HSI_enable, uint8_t HSE_enable, uint8_t LSI_enable, uint8_t LSE_enable, uint8_t CSS_enable,
	uint8_t HSE_bypass_enable, uint8_t LSE_bypass_enable, uint8_t LSE_low_power_disable, uint8_t HSI_trim
);
void set_SYS_PLL_config(PLL_CLK_SRC_t src, uint8_t PLL_M, uint8_t PLL_N, PLL_P_t PLL_P, uint8_t PLL_Q, uint8_t PLL_R);
void set_SYS_clock_config(SYS_CLK_SRC_t src, AHB_DIV_t AHB_div, APBx_DIV_t APB1_div, APBx_DIV_t APB2_div);
void set_SYS_backup_domain_config(void);
void set_SYS_RTC_config(RTC_SRC_t RTC_src, uint8_t RTC_HSE_div);
void set_SYS_tick_config(uint8_t interrupt_enable);


#endif //STM32F412_SYS_H
