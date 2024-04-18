//
// Created by marijn on 4/18/24.
//

#include "app/keyboard.h"


/*!<
 * functions
 * */
uint8_t HID_buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
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
