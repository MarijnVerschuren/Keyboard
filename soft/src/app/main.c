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


/*!<
 * defines
 * */
#define HID_MIN_DELAY	20U

#define ROM_PAGE_SIZE	0x80U
#define ROM_PAGE_COUNT	0x1FFU


/*!<
 * types
 * */


/*!<
 * variables
 * */
I2C_setting_t I2C_setting = {
	.I2C_clock_frequency = 4000000,
	.scl_l_pre = 0x13U, .scl_h_pre = 0x0FU,
	.sda_delay = 0x02U, .scl_delay = 0x04U
};  // 100 KHz
uint8_t HID_buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
volatile uint8_t GO = 0;

uint8_t cryp_IV[16] = {  // TODO: random and read
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};


extern void TIM8_UP_TIM13_IRQHandler(void) {
	TIM8->SR &= ~TIM_SR_UIF;  // clear interrupt flag
	GPIO_toggle(GPIOC, 1);
}

extern void EXTI15_10_IRQHandler(void) {	// button K2
	EXTI->PR1 |= EXTI_PR1_PR13;  // clear interrupt flag
	//stop_TIM(TIM8);
	GO = 1;
}

void send_text(const uint8_t* str) {
	while (*str) {
		HID_buffer[2] = 0x4 + (*str++ - 'a');
		send_HID_report(&USB_handle, HID_buffer, 8);
		delay_ms(HID_MIN_DELAY);
		HID_buffer[2] = 0;
		send_HID_report(&USB_handle, HID_buffer, 8);
		delay_ms(HID_MIN_DELAY);
	}
}


void write_encrypted_page(I2C_TypeDef* i2c, uint8_t ROM_address, uint16_t page_address, const void* key, CRYP_KEY_t key_type, void* buffer) {
	static uint8_t page_buffer[ROM_PAGE_SIZE];  // :(
	AES_CBC_encrypt_setup(cryp_IV, key, key_type);
	for (uint8_t i = 0; i < (ROM_PAGE_SIZE / AES_BLOCK_SIZE); i++) {
		AES_CBC_process_block(&buffer[i * AES_BLOCK_SIZE], &page_buffer[i * AES_BLOCK_SIZE]);
	} I2C_master_write_reg(i2c, ROM_address, page_address << 7U, I2C_REG_16, page_buffer, ROM_PAGE_SIZE, 100);
}

void read_encrypted_page(I2C_TypeDef* i2c, uint8_t ROM_address, uint8_t page_address, const void* key, CRYP_KEY_t key_type, void* buffer) {
	I2C_master_read_reg(i2c, ROM_address, page_address << 7U, I2C_REG_16, buffer, ROM_PAGE_SIZE, 100);
	AES_CBC_decrypt_setup(cryp_IV, key, key_type);  // TODO: read IV when format is defined
	for (uint8_t i = 0; i < (ROM_PAGE_SIZE / AES_BLOCK_SIZE); i++) {
		AES_CBC_process_block(&buffer[i * AES_BLOCK_SIZE], &buffer[i * AES_BLOCK_SIZE]);
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
	start_TIM(TIM8);

	/* GPIO config */
	config_GPIO(GPIOC, 1, GPIO_output, GPIO_no_pull, GPIO_push_pull);	// user led C1
	config_GPIO(GPIOC, 13, GPIO_input, GPIO_pull_up, GPIO_open_drain);	// user button C13
	GPIO_write(GPIOC, 1, 1);

	/* EXTI config */
	config_EXTI(13, GPIOC, 0, 1);
	start_EXTI(13);

	/* RNG, HASH, CRYP config */
	config_RNG_kernel_clock(RNG_CLK_SRC_HSI48);
	start_RNG(); config_HASH(); config_CRYP();

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
	start_watchdog();


	uint8_t hash_data[] = {0x00, 0x00, 0x00, 'a'};	// TODO: endian-ness swap??
	uint8_t hash_key[] = {0x00, 0x00, 0x00, 'b'};	// TODO: endian-ness swap??
	volatile uint32_t digest[8];
	process_HMAC(hash_data, 1, hash_key, 1, HASH_ALGO_SHA2_256);
	for (uint8_t i = 0; i < 8; i++) { digest[i] = ((__IO uint32_t*)HASH_digest)[i]; }


	uint8_t delay = 20;

	uint32_t rn = RNG_generate();
	(void)rn;

	uint8_t data[128] = "passwords and stuff here!!";
	uint8_t key[32] = "PASSWORD for KEYBOARD!!";

	uint8_t page[128];

	// main loop
	for(;;) {
		reset_watchdog();
		if (!GO) { continue; }

		//write_encrypted_page(I2C3, 0x50, 0x0, key, CRYP_KEY_256, data);
		read_encrypted_page(I2C3, 0x50, 0x0, key, CRYP_KEY_256, page);
		send_text(page);

		GO = 0;
	}


	/*!< ROM code */
}

// p.g. 346 RCC diagram
// TODO: RTC!!!
// TODO: PWR_CR1_SVOS (stop mode VOS)
// TODO: VBAT!!!
// TODO: make HASH, CRYP, RNG interrupt based for faster / parallel processing

// TODO: look at WKUPCR in PWR in USB wakup it