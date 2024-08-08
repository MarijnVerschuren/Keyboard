//
// Created by marijn on 4/18/24.
//
#include "app/keyboard.h"


/*!<
 * variables
 */
static volatile uint8_t		scan_iter = 0;
static volatile uint8_t		HID_buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
static volatile uint16_t	matrix[ROW_COUNT];


/*!<
 * static functions
 * */
static inline void scan_row(uint8_t row) {
	GPIOE->ODR = ~(0x1UL << row);
	uint16_t code;
	uint16_t dat = ~GPIOD->IDR;
	for (uint8_t i = 0; i < COL_COUNT && dat; i++) {
		code = key_map[0][row][i];
		if (dat & (0x1UL << i))	{ press_key(code); }
		else					{ release_key(code); }
	} GPIOE->ODR = 0xFFFF;  // preemptively disable pin to provide time to reset row
}


/*!<
 * interrupt
 * */
extern void TIM1_UP_IRQHandler(void) {
	TIM1->SR &= ~TIM_SR_UIF;  // clear interrupt flag
	scan_row(scan_iter++ % ROW_COUNT);
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
	enable_dev(GPIOE);
	GPIOE->MODER =		0x55555555UL;	// output
	GPIOE->OSPEEDR =	0xFFFFFFFFUL;	// very high speed
	GPIOE->PUPDR =		0x00000000UL;	// no_pull
	GPIOE->OTYPER =		0x00000000UL;	// push_pull
	GPIOE->ODR =		0xFFFFUL;		// high_default
	enable_dev(GPIOD);
	GPIOD->MODER =		0x00000000UL;	// input
	GPIOD->OSPEEDR =	0xFFFFFFFFUL;	// very high speed
	GPIOD->PUPDR =		0x55555555UL;	// pull_up
	GPIOD->OTYPER =		0x55555555UL;	// open_drain (power savings)
	config_TIM(TIM1, (TIM_APB1_kernel_frequency / 1000000) - 1, 1000 / ROW_COUNT);
	start_TIM_update_irq(TIM1);
}
void start_scan(void)	{ start_TIM(TIM1); }
void stop_scan(void)	{ stop_TIM(TIM1); }

/*!< encoder */
void init_encoders(void) {
	config_encoder_S0S90(TIM2_CH1_A0, TIM2_CH2_A1);
	config_GPIO(GPIOA, 2U, GPIO_input, GPIO_no_pull, GPIO_open_drain);
	config_EXTI(2U, GPIOA, 1, 0);
	config_encoder_S0S90(TIM3_CH1_B4, TIM3_CH2_B5);
	config_GPIO(GPIOB, 3U, GPIO_input, GPIO_no_pull, GPIO_open_drain);
	config_EXTI(3U, GPIOB, 1, 0);
}
void start_encoders(void) {
	start_encoder_S0S90(TIM2);
	//start_EXTI(2U);
	start_encoder_S0S90(TIM3);
	start_EXTI(3U);
}
void stop_encoders(void) {
	stop_encoder_S0S90(TIM2);
	//stop_EXTI(2U);
	stop_encoder_S0S90(TIM3);
	stop_EXTI(3U);
}

/*!< misc */
void press_key(uint16_t code) {
	if (code & 0x100) {
		HID_buffer[0] |= (0b1U << (code & 0x7U));
	} else {
		code &= 0xFFU;
		for (uint8_t i = 0; i < 6; i++) {
			if (HID_buffer[i + 2] == code) { return; }
		}
		for (uint8_t i = 0; i < 6; i++) {
			if (HID_buffer[i + 2] == 0x00U) {
				HID_buffer[i + 2] = code; break;
			}
		}
	}
	send_HID_report(&USB_handle, HID_buffer, 8);
}
void release_key(uint16_t code) {
	if (code & 0x100) {
		uint8_t mask = (0b1U << (code & 0x7U));
		if (!(HID_buffer[0] & mask)) { return; }
		HID_buffer[0] &= ~mask;
	} else {
		uint8_t change = 0;
		for (uint8_t i = 0; i < 6; i++) {
			if (HID_buffer[i + 2] == (code & 0xFFU)) {
				HID_buffer[i + 2] = 0x00U; change = 1;
			}
		} if (!change) { return; }
	}
	send_HID_report(&USB_handle, HID_buffer, 8);
}
void release_all(void) {
	*(uint64_t*)HID_buffer = 0x0000000000000000;
}
void send_text(const uint8_t* str) {
	while (*str) {
		HID_buffer[2] = 0x4 + (*str++ - 'a');
		send_HID_report(&USB_handle, HID_buffer, 8);
		delay_ms(MIN_HID_DELAY);
		HID_buffer[2] = 0;
		send_HID_report(&USB_handle, HID_buffer, 8);
		delay_ms(MIN_HID_DELAY);
	}
}