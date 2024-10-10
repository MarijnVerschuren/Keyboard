#include "base.h"
#include "periph.h"
#include "sys.h"
#include "GPIO.h"
#include "USART.h"
#include "RTC.h"
#include "I2C.h"
#include "ADC.h"
#include "tim.h"

#include "AS5600/AS5600.h"

#include "test.h"


volatile uint16_t angle = 0;


void TIM1_update_handler(void) {
	TIM1->SR &= ~0x00000001UL;
}

void RTC_stamp_handler(void) {
	EXTI->PR = 0x00200000UL;
	uint32_t ts = RTC_unix();
	USART_write(USART1, &ts, 4, 100);
	return;
}

void EXTI5_9_handler(void) {
	EXTI->PR = 0x00000001UL;
	return;
}


void ADC_handler(void) {
	uint8_t status = ADC1->SR;
	ADC1->SR = ~status;
	if (status & 0b000001) {
		return;  // watchdog
	} if (status & 0b000010) {
		return;  // EOC
	} if (status & 0b000100) {
		uint32_t a = ADC1->JDR1;
		angle = a + (a / 4);
		return;  // JEOC
	} if (status & 0b10000) {
		return;  // OVR
	}
}


// application
void main(void) {
	set_SYS_hardware_config(PWR_LVL_NOM, 25000000);						// 3v3, HSE@25MHz
	//set_SYS_oscilator_config(0, 1, 0, 1, 1, 0, 0, 0, 0);				// enable HSE, LSE, CSS and the backup-domain
	set_SYS_oscilator_config(0, 1, 0, 0, 1, 0, 0, 0, 0);				// enable HSE, CSS and the backup-domain
	set_SYS_PLL_config(PLL_CLK_SRC_HSE, 12U, 96U, PLL_P_DIV_4, 0, 0);	// enable PLL @ 25MHz / 12 * 96 / 2 -> 100MHz
	//set_SYS_backup_domain_config();										// enable backup domain
	//set_SYS_RTC_config(RTC_SRC_LSE, 0U);								// enable RTC
	set_SYS_clock_config(SYS_CLK_SRC_PLL_P, AHB_CLK_NO_DIV, APBx_CLK_DIV2, APBx_CLK_NO_DIV);
	set_SYS_tick_config(1);												// enable SYS_tick with interrupt
	sys_init();															// write settings

	// GPIO
	config_GPIO(GPIOA, 8, GPIO_output | GPIO_no_pull | GPIO_push_pull);

	// EXTI TODO: flags??
	//config_EXTI_GPIO(GPIOA, 0, 0, 1);
	//NVIC_set_IRQ_priority(EXTI0_IRQn, 0);
	//start_EXTI(0);

	// ADC
	config_ADC(ADC_CLK_DIV2 | ADC_INJ_DISC | ADC_RES_12B | ADC_EOC_SINGLE | ADC_INJ_TRIG_TIM1_TRGO | ADC_INJ_TRIG_MODE_RISING);
	config_ADC_watchdog(0, ADC_WDG_TYPE_INJECTED, 200, 3900);
	config_ADC_IRQ(1, ADC_IRQ_JEOC | ADC_IRQ_WDG);
	config_ADC_GPIO_inj_channel(GPIOA, 0, ADC_SAMPLE_28_CYCLES, 409, 0);

	// TIM
	config_TIM_master(TIM1, 10000, 100, TIM_TRGO_UPDATE);  // 100 Hz
	start_TIM_update_irq(TIM1);
	// TODO: is it possible to combine PWM and UPDATE triggers to make sure move and read are not done psudo-simultanuisly

	// USART
	config_UART(USART1_TX_A9, USART1_RX_A10, 115200);

	// I2C
	config_I2C(I2C2_B10_SCL, I2C2_B9_SDA, 0x00);

	//uint32_t ts = 1726832418;
	//uconfig_RTC(ts, RTC_WAKEUP_DISABLE, RTC_WAKEUP_DIV16, 0x0000U);
	//config_RTC_ext_ts(1U, RTC_TS_POLARITY_RISING);

	/*!< test apps */
	while (config_AS5600(
		I2C2, AS5600_POW_NOM | AS5600_HYST_2LSB | AS5600_MODE_REDUCED_ANALOG |
		AS5600_SFILTER_2 | AS5600_FFILTER_10LSB | AS5600_WDG_ON, 10
	)) { delay_ms(10); }
	volatile uint8_t stat = AS5600_get_status(I2C2, 10);

	start_ADC(0, 1);
	start_TIM(TIM1);

	while (1) {
		GPIO_toggle(GPIOA, 8);
		delay_ms(angle / 10);
	}
}
