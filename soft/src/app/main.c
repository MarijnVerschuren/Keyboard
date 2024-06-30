#include "gpio.h"
#include "exti.h"
#include "tim.h"
#include "pwm.h"
#include "encoder.h"
#include "usart.h"
#include "i2c.h"
#include "crc.h"
#include "rng.h"
#include "watchdog.h"
#include "mco.h"
#include "usb/usb.h"
#include "usb/hid.h"
#include "hash.h"
#include "cryp.h"
#include "spi/ospi.h"
#include "spi/W25Q64.h"

#include "app/keyboard.h"


#define STATUS_PORT GPIOC
#define STATUS_PIN 0


extern void TIM8_UP_TIM13_IRQHandler(void) {
	TIM8->SR &= ~TIM_SR_UIF;  // clear interrupt flag
	GPIO_toggle(STATUS_PORT, STATUS_PIN);
}



void run(void) {
	init_grid();
	init_encoders();			// TODO: validate ROT0 connection!!
	// W25Q64_init(OCTOSPI1);	// TODO: validate connection!!
	// init_LCD();				// TODO

	start_USB(USB1_OTG_HS);		// TODO: re-solder FL1
	start_scan();
	start_encoders();
	start_watchdog();

	// main loop
	for(;;) {
		reset_watchdog();

	}
}


int main(void) {
	/* clock config */
	set_PLL_config(
		0, 1, 1, 1, 0, 0,								// enable PLL1 (P, Q)
		PLL_IN_4MHz_8MHz, PLL_VCO_WIDE,					// 5MHz in, 192MHz < VCO < 960MHz
		5/*M*/, 2/*P*/, 4/*Q*/, 2/*R*/, 112/*N*/, 0		// M = 5, P = 2, Q = 2, R = 2, N = 112, N_frac = 0
	);  // 25MHz / 5 * 112 / (2, 2, 2)	=>	(280Mhz, 280Mhz, *280Mhz)
	set_PLL_config(
		1, 0, 0, 0, 0, 0,								// disable PLL2
		PLL_IN_4MHz_8MHz, PLL_VCO_WIDE,					// 5MHz in, 192MHz < VCO < 960MHz
		5/*M*/, 2/*P*/, 4/*Q*/, 2/*R*/, 112/*N*/, 0		// M = 5, P = 2, Q = 2, R = 2, N = 112, N_frac = 0
	);  // 25MHz / 5 * 112 / (2, 2, 2)	=>	(280Mhz, 280Mhz, *280Mhz)
	set_PLL_config(
		2, 0, 0, 0, 0, 0,								// disable PLL3
		PLL_IN_4MHz_8MHz, PLL_VCO_WIDE,					// 5MHz in, 192MHz < VCO < 960MHz
		5/*M*/, 2/*P*/, 4/*Q*/, 2/*R*/, 112/*N*/, 0		// M = 5, P = 2, Q = 2, R = 2, N = 112, N_frac = 0
	);  // 25MHz / 5 * 112 / (2, 2, 2)	=>	(280Mhz, 280Mhz, *280Mhz)
	set_RTC_config(0, RCC_SRC_DISABLED, 0);				// disable RTC
	set_clock_config(
		0, 1, 0, 0, 0, 1,								// disable HSI, enable HSE, enable HSI48
		0, 0, 1, 0, HSI_DIV_1, 25000000,				// enable HSE_CSS, HSE_freq = 25MHz
		PLL_SRC_HSE										// set HSE as PLL source clock
	);
	set_SYS_config(
		SYS_CLK_SRC_PLL1_P, SYS_CLK_DIV_1,				// SYS_CLK = PLL1_P
		CORE_VOS_1, FLASH_LATENCY3						// FLASH and PWR settings for AXI_freq = 280MHz, CORE_freq = 280MHz
	);
	set_domain_config(
		AHB_CLK_DIV_2, APB_CLK_DIV_2,					// AHB and AXI = 280MHz
		APB_CLK_DIV_2, APB_CLK_DIV_2, APB_CLK_DIV_2		// APB1-4 = 140MHz
	);
	set_systick_config(
		1, 1, SYSTICK_CLK_SRC_AXI_CLK_DIV_1				// SysTick (IRQ) enable at 280MHz
	);
	sys_clock_init();

	/* GPIO config */
	config_GPIO(STATUS_PORT, STATUS_PIN, GPIO_output, GPIO_no_pull, GPIO_push_pull);
	GPIO_write(STATUS_PORT, STATUS_PIN, 1);

	/* TIM config */
	config_TIM_kernel_clocks(
		TIM_MUL_2, LPTIM_CLK_SRC_APBx,
		LPTIM_CLK_SRC_APBx, LPTIM_CLK_SRC_APBx
	);
	config_TIM(TIM8, TIM_APB2_kernel_frequency / 10000, 10000);  // 1 Hz
	start_TIM_update_irq(TIM8);
	start_TIM(TIM8);

	/* RNG, CRC, HASH, CRYP config */
	config_RNG_kernel_clock(RNG_CLK_SRC_HSI48);
	start_RNG(); config_CRC(); config_HASH(); config_CRYP();

	/* SPI config */
	config_SPI_kernel_clocks(SPI123_CLK_SRC_PLL1_Q, SPI456_CLK_SRC_APBx, SPI456_CLK_SRC_APBx);

	/* QSPI config */
	config_OSPI_kernel_clock(OSPI_CLK_SRC_PLL1_Q);
	config_QSPI(
		OCTOSPI1, OSPIM_PORT1_SCK_B2,
		OSPIM_PORT1_IO0_B1, OSPIM_PORT1_IO1_B0,
		OSPIM_PORT1_IO2_A7, OSPIM_PORT1_IO3_A6,
		OSPIM_PORT1_NSS_B6, 0U, OPSI_MEMORY_MICRON, 31
	);

	/* USB config */  // TODO: do low power later (when debugging is fixed)
	config_USB_kernel_clock(USB_CLK_SRC_HSI48);
	config_USB(USB1_OTG_HS, &HID_class, &FS_Desc, 0, 0);  // TODO low power doesnt work!!
	// TODO: RESOLDER FL1!!!!!

	/* watchdog config */
	config_watchdog(0, 0xFFFUL);	// 1s

	// TODO: RTC!!!

	// start the program
	run();
}



// p.g. 346 RCC diagram
// TODO: RTC!!!
// TODO: PWR_CR1_SVOS (stop mode VOS)
// TODO: VBAT!!!
// TODO: make HASH, CRYP, RNG interrupt based for faster / parallel processing

// TODO: look at WKUPCR in PWR in USB wakup it