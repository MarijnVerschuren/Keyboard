//
// Created by marijn on 4/18/24.
//

#include "app/keyboard.h"

/*!<
 * variables
 */
static uint8_t HID_buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};




/*!<
 * functions
 * */
void init_grid(void) {
	// TODO: improve?
	enable_dev(ROW_PORT);
	ROW_PORT->MODER =	0x55555555UL;	// output
	ROW_PORT->OSPEEDR =	0x00000000UL;	// TODO?
	ROW_PORT->PUPDR =	0x00000000UL;	// no_pull
	ROW_PORT->OTYPER =	0x00000000UL;	// push_pull
	ROW_PORT->ODR =		0xFFFFUL;		// high_default
	enable_dev(COL_PORT);
	COL_PORT->MODER =	0x00000000UL; // input
	COL_PORT->OSPEEDR =	0x00000000UL; // TODO?
	COL_PORT->PUPDR =	0x55555555UL; // pull_up
	COL_PORT->OTYPER =	0x55555555UL; // open_drain (power savings)
}
void scan_grid(void) {
	// TODO: output?
	// TODO: key prio?
	// TODO: timing?
	// TODO: debounce?
	// TODO: function v interrupt?
	for (uint8_t row = 0; row < ROW_COUNT; row++) {
		ROW_PORT->BSRR = (0x10000UL << row) | (0x1UL << (row - 1));
		uint16_t dat = COL_PORT->IDR;
		for (uint8_t i = 0; i < COL_COUNT && dat; i++) {
			if (!(dat & (0x1UL << i))) {
				last_key = key_map[0][row][i];
			}
		}
	}
}

void send_text(const uint8_t* str) {
	// TODO: improve!
	while (*str) {
		HID_buffer[2] = 0x4 + (*str++ - 'a');
		send_HID_report(&USB_handle, HID_buffer, 8);
		delay_ms(HID_MIN_DELAY);
		HID_buffer[2] = 0;
		send_HID_report(&USB_handle, HID_buffer, 8);
		delay_ms(HID_MIN_DELAY);
	}
}
