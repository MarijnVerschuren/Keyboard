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
 * config
 * */
#define ROW_PORT		GPIOE
#define ROW_COUNT		5
#define COL_PORT		GPIOD
#define COL_COUNT		15
extern const uint16_t PROGMEM key_map[][ROW_COUNT][COL_COUNT];


// tmp
uint16_t last_key;


// TODO: keymap

/*!<
 * functions
 * */
void init_grid(void);
void scan_grid(void);
void send_text(const uint8_t* str);


#endif // SOFT_KEYBOARD_H
