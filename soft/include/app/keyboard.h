//
// Created by marijn on 4/18/24.
//

#ifndef SOFT_KEYBOARD_H
#define SOFT_KEYBOARD_H
#include "usb/usb.h"
#include "usb/hid.h"


/*!<
 * defines
 * */
#define HID_MIN_DELAY	20U


/*!<
 * functions
 * */
void send_text(const uint8_t* str);


#endif // SOFT_KEYBOARD_H
