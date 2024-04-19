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

#include "app/rom.h"
#include "app/keyboard.h"
#include "app/passwd.h"



/*!<
 * variables
 * */
I2C_setting_t I2C_setting = {
	.I2C_clock_frequency = 4000000,
	.scl_l_pre = 0x13U, .scl_h_pre = 0x0FU,
	.sda_delay = 0x02U, .scl_delay = 0x04U
};  // 100 KHz TODO: 400KHz
volatile uint8_t GO = 0;



extern void TIM8_UP_TIM13_IRQHandler(void) {
	TIM8->SR &= ~TIM_SR_UIF;  // clear interrupt flag
	GPIO_toggle(GPIOC, 1);
}

extern void EXTI15_10_IRQHandler(void) {	// button K2
	EXTI->PR1 |= EXTI_PR1_PR13;  // clear interrupt flag
	//stop_TIM(TIM8);
	GO = 1;
}


void app(void) {
	if (init_password_ROM(I2C3, ROM_BASE_ADDRESS, pass)) { for(;;); }

	/* structure TODO:
	 A buffer for 7 descriptors will be used where the center 3 are displayed on the screen
	 so that when scrolling new descriptors may be loaded to the buffer (each descriptor takes about 13ms).
	 Only when pressed will the password be read and decrypted (this takes ...ms).
	 Finally the password is typed out (this will take around 2n ms where n is the number of characters).
	 */

	uint32_t					page[ROM_PAGE_SIZE >> 2];
	ROM_password_descriptor_t*	descriptor =		(void*)page_buffer;
	ROM_encrypted_page_t*		password_page =		(void*)page_buffer;
	start_watchdog();

	// main loop
	for(;;) {
		reset_watchdog();
		if (!GO) { continue; }
			
		GO = 0;
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

	/* TIM config */
	config_TIM_kernel_clocks(
			TIM_MUL_2, LPTIM_CLK_SRC_APBx,
			LPTIM_CLK_SRC_APBx, LPTIM_CLK_SRC_APBx
	);
	config_TIM(TIM8, TIM_APB2_kernel_frequency / 10000, 10000);  // 1 Hz
	start_TIM_update_irq(TIM8);
	//start_TIM(TIM8);

	/* GPIO config */
	config_GPIO(GPIOC, 1, GPIO_output, GPIO_no_pull, GPIO_push_pull);	// user led C1
	config_GPIO(GPIOC, 13, GPIO_input, GPIO_pull_up, GPIO_open_drain);	// user button C13
	GPIO_write(GPIOC, 1, 1);

	/* EXTI config */
	config_EXTI(13, GPIOC, 0, 1);
	start_EXTI(13);

	/* RNG, CRC, HASH, CRYP config */
	config_RNG_kernel_clock(RNG_CLK_SRC_HSI48);
	start_RNG(); config_CRC(); config_HASH(); config_CRYP();

	/* UART config */
	config_USART_kernel_clocks(USART_CLK_SRC_APBx, USART_CLK_SRC_APBx, USART_CLK_SRC_APBx);
	config_UART(USART1_TX_A9, USART1_RX_A10, 115200, 1);

	/* I2C config */
	config_I2C_kernel_clocks(I2C_CLK_SRC_APBx, I2C_CLK_SRC_APBx);
	config_I2C(I2C3_SCL_A8, I2C3_SDA_C9, I2C_setting, 0x50);
	//A8 SCL, C9 SDA

	/* USB config */  // TODO: do low power later (when debugging is fixed)
	config_USB_kernel_clock(USB_CLK_SRC_HSI48);
	config_USB(USB1_OTG_HS, &HID_class, &FS_Desc, 0, 0);  // TODO low power doesnt work!!
	start_USB(USB1_OTG_HS);

	//Watchdog config (32kHz / (4 << prescaler))
	config_watchdog(0, 0xFFFUL);	// 1s

	// start the program
	app();
}

/* read password!
read_descriptor(I2C3, ROM_BASE_ADDRESS, 0x00, descriptor);  // 13 ms
read_password(I2C3, ROM_BASE_ADDRESS, 0x00, password_page);
 * */


/* read password 0 manually
read_page(I2C3, ROM_BASE_ADDRESS, ROM_INFO, page);
delay_ms(200);
read_page(I2C3, ROM_BASE_ADDRESS, ROM_PASSWORD_DESCRIPTORS, page);
delay_ms(200);
read_page(I2C3, ROM_BASE_ADDRESS, ROM_PASSWORD_DATA, page);
delay_ms(200);
read_page(I2C3, ROM_BASE_ADDRESS, ROM_CRC, page);
delay_ms(200);
read_page(I2C3, ROM_BASE_ADDRESS, ROM_CRC + 8, page);
delay_ms(200);
*/


// p.g. 346 RCC diagram
// TODO: RTC!!!
// TODO: PWR_CR1_SVOS (stop mode VOS)
// TODO: VBAT!!!
// TODO: make HASH, CRYP, RNG interrupt based for faster / parallel processing

// TODO: look at WKUPCR in PWR in USB wakup it