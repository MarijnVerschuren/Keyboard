//
// Created by marijn on 4/18/24.
//

#include "app/keyboard.h"


/*!<
 * variables
 */
static volatile uint8_t scan_iter = 0;
static volatile uint8_t key_count = 0;
static volatile uint8_t HID_buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
 

/*!<
 * static functions
 * */
static inline void clear_HID_buffer() {
	*(uint64_t*)HID_buffer = 0x0000000000000000;
	key_count = 0;
}
static inline void scan_row(uint8_t row) {
	ROW_PORT->ODR = ~(0x1UL << row);
	uint16_t dat = ~COL_PORT->IDR;
	for (uint8_t i = 0; i < COL_COUNT && dat && key_count < 6; i++) {
		if (!(dat & (0x1UL << i))) { continue; }
		HID_buffer[2 + key_count++] = key_map[0][row][i] & 0xFF;
	} ROW_PORT->ODR = 0xFFFF;  // preemptively disable pin to provide time to reset row
}


/*!<
 * interrupt
 * */
extern void TIM1_UP_IRQHandler(void) {
	SCAN_TIM->SR &= ~TIM_SR_UIF;  // clear interrupt flag
	if (!scan_iter) {
		send_HID_report(&USB_handle, HID_buffer, 8);
		clear_HID_buffer();
	}
	scan_row(scan_iter++);
}
extern void EXTI2_IRQHandler(void) {
	EXTI->PR1 |= EXTI_PR1_PR2;
	// TODO: hit when turning rotary????
	// SW pin seems to be shorted out??
	// TODO: fix on next version
}
extern void EXTI3_IRQHandler(void) {
	EXTI->PR1 |= EXTI_PR1_PR3;
}


/*!<
 * functions
 * */
/*!< grid */
void init_grid(void) {
	enable_dev(ROW_PORT);
	ROW_PORT->MODER =	0x55555555UL;	// output
	ROW_PORT->OSPEEDR =	0xFFFFFFFFUL;	// very high speed
	ROW_PORT->PUPDR =	0x00000000UL;	// no_pull
	ROW_PORT->OTYPER =	0x00000000UL;	// push_pull
	ROW_PORT->ODR =		0xFFFFUL;		// high_default
	enable_dev(COL_PORT);
	COL_PORT->MODER =	0x00000000UL; // input
	COL_PORT->OSPEEDR =	0xFFFFFFFFUL; // very high speed
	COL_PORT->PUPDR =	0x55555555UL; // pull_up
	COL_PORT->OTYPER =	0x55555555UL; // open_drain (power savings)
	config_TIM(SCAN_TIM, (SCAN_TIM_KERNEL_FREQ / 1000000) - 1, 1000 / ROW_COUNT);
	start_TIM_update_irq(SCAN_TIM);
}
void start_scan(void)	{ start_TIM(SCAN_TIM); }
void stop_scan(void)	{ stop_TIM(SCAN_TIM); }

/*!< encoder */
void init_encoders(void) {
	config_encoder_S0S90(ENC0_A, ENC0_B);
	config_GPIO(ENC0_SW_PORT, ENC0_SW_PIN, GPIO_input, GPIO_no_pull, GPIO_open_drain);
	config_EXTI(ENC0_SW_PIN, ENC0_SW_PORT, 1, 0);
	config_encoder_S0S90(ENC1_A, ENC1_B);
	config_GPIO(ENC1_SW_PORT, ENC1_SW_PIN, GPIO_input, GPIO_no_pull, GPIO_open_drain);
	config_EXTI(ENC1_SW_PIN, ENC1_SW_PORT, 1, 0);
}
void start_encoders(void) {
	start_encoder_S0S90(ENC0_TIM);
	//start_EXTI(ENC0_SW_PIN);
	start_encoder_S0S90(ENC1_TIM);
	start_EXTI(ENC1_SW_PIN);
}
void stop_encoders(void) {
	stop_encoder_S0S90(ENC0_TIM);
	//stop_EXTI(ENC0_SW_PIN);
	stop_encoder_S0S90(ENC1_TIM);
	stop_EXTI(ENC1_SW_PIN);
}

/*!< misc */
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
