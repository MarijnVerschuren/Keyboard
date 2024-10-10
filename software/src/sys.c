//
// Created by marijn on 9/10/24.
//
#include "sys.h"


/*!<
 * constants
 * */
const uint32_t	HSI_clock_frequency = 16000000;
const uint32_t	LSE_clock_frequency = 32768;
const uint32_t	LSI_clock_frequency = 32000;


/*!<
 * clock variables
 * */
uint32_t HSE_clock_frequency =		0;
uint32_t PLL_P_clock_frequency =	0;
uint32_t PLL_Q_clock_frequency =	0;
uint32_t PLL_R_clock_frequency =	0;
uint32_t RTC_clock_frequency =		0;
uint32_t SYS_clock_frequency =		16000000;
uint32_t AHB_clock_frequency =		16000000;
uint32_t APB1_clock_frequency =		16000000;
uint32_t APB2_clock_frequency =		16000000;



/*!<
 * config
 * */
volatile extern struct {
	// RCC_CR			config
	uint32_t HSI_enable					: 1;
	uint32_t HSI_trim					: 5;
	uint32_t HSE_enable					: 1;
	uint32_t HSE_bypass_enable			: 1;
	uint32_t CSS_enable					: 1;
	uint32_t PLL_enable					: 1;
	// RCC_PLL_CFGR		config TODO
	uint32_t PLL_src					: 1;  // PLL_CLK_SRC_t
	uint32_t PLL_M						: 6;
	uint32_t PLL_N						: 9;
	uint32_t PLL_P						: 2;  // PLL_P_t
	uint32_t PLL_Q						: 4;  // (0, 1 are invalid)
	uint32_t PLL_R						: 3;  // (0, 1 are invalid)
	// RCC_CFGR			config TODO
	uint32_t sys_clk_src				: 2;  // SYS_CLK_SRC_t
	uint32_t AHB_div					: 4;  // AHB_DIV_t
	uint32_t APB1_div					: 3;  // APBx_DIV_t
	uint32_t APB2_div					: 3;  // APBx_DIV_t
	uint32_t RTC_HSE_div				: 5;
	// RCC_BDCR			config TODO
	uint32_t LSE_enable					: 1;
	uint32_t LSE_bypass_enable			: 1;
	uint32_t LSE_low_power_disable		: 1;
	uint32_t RTC_enable					: 1;
	uint32_t RTC_src					: 2;  // RTC_SRC_t
	// PWR_CSR
	uint32_t BKUP_regulator_enable		: 1;
	// RCC_CSR			config
	uint32_t LSI_enable					: 1;
	// FLASH_ TODO
	uint32_t FLASH_prefetch				: 1;
	uint32_t FLASH_instruction_cache	: 1;
	uint32_t FLASH_data_cache			: 1;
	// SYS_TICK config
	uint32_t SYS_tick_enable			: 1;
	uint32_t SYS_tick_interrupt_enable	: 1;
	// hardware info
	uint32_t PWR_level					: 2;  // PWR_level_t
	uint32_t HSE_frequency				: 28;
} sys_config;



void set_SYS_hardware_config(PWR_level_t power_level, uint32_t HSE_frequency) {
	sys_config.PWR_level =					power_level;
	sys_config.HSE_frequency =				HSE_frequency;
}
void set_SYS_oscilator_config(
	uint8_t HSI_enable, uint8_t HSE_enable, uint8_t LSI_enable, uint8_t LSE_enable, uint8_t CSS_enable,
	uint8_t HSE_bypass_enable, uint8_t LSE_bypass_enable, uint8_t LSE_low_power_disable, uint8_t HSI_trim
) {
	sys_config.HSI_enable =					HSI_enable;
	sys_config.HSE_enable =					HSE_enable;
	sys_config.LSI_enable =					LSI_enable;
	sys_config.LSE_enable =					LSE_enable;
	sys_config.CSS_enable =					CSS_enable;
	sys_config.HSE_bypass_enable =			HSE_bypass_enable;
	sys_config.LSE_bypass_enable =			LSE_bypass_enable;
	sys_config.LSE_low_power_disable =		LSE_low_power_disable;
	sys_config.HSI_trim =					HSI_trim;
}
void set_SYS_PLL_config(PLL_CLK_SRC_t src, uint8_t PLL_M, uint8_t PLL_N, PLL_P_t PLL_P, uint8_t PLL_Q, uint8_t PLL_R) {
	sys_config.PLL_enable =					0b1U;
	sys_config.PLL_src =					src;
	sys_config.PLL_M =						PLL_M;
	sys_config.PLL_N =						PLL_N;
	sys_config.PLL_P =						PLL_P;
	sys_config.PLL_Q =						PLL_Q;
	sys_config.PLL_R =						PLL_R;
}
void set_SYS_clock_config(SYS_CLK_SRC_t src, AHB_DIV_t AHB_div, APBx_DIV_t APB1_div, APBx_DIV_t APB2_div) {
	sys_config.sys_clk_src =				src;
	sys_config.AHB_div =					AHB_div;
	sys_config.APB1_div =					APB1_div;
	sys_config.APB2_div =					APB2_div;
}
void set_SYS_backup_domain_config(void) {
	sys_config.BKUP_regulator_enable =		0b1U;
}
void set_SYS_RTC_config(RTC_SRC_t RTC_src, uint8_t RTC_HSE_div) {
	sys_config.RTC_enable =					0b1U;
	sys_config.RTC_src =					RTC_src;
	sys_config.RTC_HSE_div =				RTC_HSE_div;
}
void set_SYS_FLASH_config(uint8_t prefetch_enable, uint8_t instruction_cache_enable, uint8_t data_cache_enable) {
	sys_config.FLASH_prefetch =				prefetch_enable;
	sys_config.FLASH_instruction_cache =	instruction_cache_enable;
	sys_config.FLASH_data_cache =			data_cache_enable;
}
void set_SYS_tick_config(uint8_t interrupt_enable) {
	sys_config.SYS_tick_enable =			0b1U;
	sys_config.SYS_tick_interrupt_enable =	interrupt_enable;
}

void sys_init(void) {
	// TODO: make it possible to switch sys clock multiple times
	// NOTE: currently assuming sys_reset state
	// set clock variables
	HSE_clock_frequency = sys_config.HSE_frequency;
	if (sys_config.PLL_enable) {
		uint32_t PLL_freq = (sys_config.PLL_src ? HSE_clock_frequency : HSI_clock_frequency);
		PLL_freq = (PLL_freq / sys_config.PLL_M) * sys_config.PLL_N;
		if (PLL_freq % 1000) { PLL_freq += 1000 - (PLL_freq % 1000); }
		PLL_P_clock_frequency = PLL_freq / (2 * sys_config.PLL_P + 2);
		if (sys_config.PLL_Q) { PLL_Q_clock_frequency = PLL_freq / sys_config.PLL_Q; }
		if (sys_config.PLL_R) { PLL_R_clock_frequency = PLL_freq / sys_config.PLL_R; }
	}
	switch (sys_config.sys_clk_src) {
	case SYS_CLK_SRC_HSI:	SYS_clock_frequency = HSI_clock_frequency; break;
	case SYS_CLK_SRC_HSE:	SYS_clock_frequency = HSE_clock_frequency; break;
	case SYS_CLK_SRC_PLL_P:	SYS_clock_frequency = PLL_P_clock_frequency; break;
	}
	AHB_clock_frequency = SYS_clock_frequency;	if (sys_config.AHB_div) { AHB_clock_frequency /= (2 << (sys_config.AHB_div & 0b111U)); }
	APB1_clock_frequency = AHB_clock_frequency;	if (sys_config.APB1_div) { APB1_clock_frequency /= (2 << (sys_config.APB1_div & 0b11U)); }
	APB2_clock_frequency = AHB_clock_frequency;	if (sys_config.APB2_div) { APB2_clock_frequency /= (2 << (sys_config.APB2_div & 0b11U)); }
	switch (sys_config.RTC_src) {
	case RTC_SRC_NONE:	RTC_clock_frequency = 0x00000000;			break;
	case RTC_SRC_LSE:	RTC_clock_frequency = LSE_clock_frequency;	break;
	case RTC_SRC_LSI:	RTC_clock_frequency = LSI_clock_frequency;	break;
	case RTC_SRC_HSE:
		RTC_clock_frequency = sys_config.RTC_HSE_div > 1 ? \
			HSE_clock_frequency / sys_config.RTC_HSE_div : 0;
		break;
	}
	// PLL settings
	RCC->PLLCFGR = (
		(sys_config.PLL_M << 0U)					|
		(sys_config.PLL_N << 6U)					|
		(sys_config.PLL_P << 16U)					|
		(sys_config.PLL_Q << 24U)					|
		(sys_config.PLL_R << 28U)					|
		(sys_config.PLL_src << 22U)
	);
	// HS clock settings
	uint32_t clk_mask = (
		(sys_config.HSI_enable << 1U)				|
		(sys_config.HSE_enable << 17U)				|
		(sys_config.PLL_enable << 25U)
	);
	RCC->CR = (
		(sys_config.HSI_trim << 3U)					|
		(sys_config.HSE_bypass_enable << 18U)		|
		(sys_config.CSS_enable << 19U)				|
		(clk_mask >> 1U) | 0x00000001UL  // keep HSI enabled
	);
	while ((RCC->CR & clk_mask) != clk_mask);
	// PWR settings
	uint8_t tmp = 0b01U;
	if (SYS_clock_frequency > 84000000)			{ tmp = 0b11; }
	else if (SYS_clock_frequency > 64000000)	{ tmp = 0b10; }
	PWR->CR = (tmp << 14U);
	// FLASH settings
	switch (sys_config.PWR_level) {
	case PWR_LVL_1v7:	tmp = SYS_clock_frequency / 16000000; break;
	case PWR_LVL_2v1:	tmp = SYS_clock_frequency / 18000000; break;
	case PWR_LVL_2v4:	tmp = SYS_clock_frequency / 24000000; break;
	case PWR_LVL_2v7:	tmp = SYS_clock_frequency / 30000000; break;
	} FLASH->ACR = (
		(tmp << 0U)									|
		(sys_config.FLASH_prefetch << 8U)			|
		(sys_config.FLASH_instruction_cache << 8U)	|
		(sys_config.FLASH_data_cache << 8U)
	);
	// SYS clock settings
	RCC->CFGR = (
		(sys_config.sys_clk_src << 0U)				|
		(sys_config.AHB_div << 4U)					|
		(sys_config.APB1_div << 10U)				|
		(sys_config.APB2_div << 13U)				|
		(sys_config.RTC_HSE_div << 16U)
	);
	while (((RCC->CFGR >> 2U) & 0b11U) != sys_config.sys_clk_src);
	if (!sys_config.HSI_enable) { RCC->CR &= ~0x00000001; }  // disable HSI
	// backup domain settings
	PWR->CR |= 0x100UL;  // disable backup domain write protection
	while (!(PWR->CR & 0x100UL));
	PWR->CSR |= (sys_config.BKUP_regulator_enable << 9U);
	while (((PWR->CSR >> 9U) & 0b1U) < sys_config.BKUP_regulator_enable);
	// LS clock settings
	RCC->BDCR = (
		(sys_config.LSE_enable << 0U)				|
		(sys_config.LSE_bypass_enable << 2U)		|
		(sys_config.LSE_low_power_disable << 3U)	|
		(sys_config.RTC_src << 8U)					|
		(sys_config.RTC_enable << 15U)
	);
	while (((RCC->BDCR >> 1U) & 0b1U) != sys_config.LSE_enable);
	RCC->CSR = (sys_config.LSI_enable);
	while (((RCC->CSR >> 1U) & 0b1U) != sys_config.LSI_enable);
	PWR->CR &= ~0x100UL;  // enable backup domain write protection

	SYS_TICK->LOAD = (AHB_clock_frequency / 8000) - 1;						/* set reload register, clk src: AHB/8 */
	SYS_TICK->VAL  = 0;														/* load counter value  */
	SYS_TICK->CTRL = (														/* start SysTick timer */
			(sys_config.SYS_tick_enable << 0U)				|
			(sys_config.SYS_tick_interrupt_enable << 1U)
	);
	// set IRQ priority
	SCB->SHP[11U] = 0b11110000;
}
