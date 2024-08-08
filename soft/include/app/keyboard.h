//
// Created by marijn on 4/18/24.
//

#ifndef SOFT_KEYBOARD_H
#define SOFT_KEYBOARD_H
#include "gpio.h"
#include "tim.h"
#include "encoder.h"
#include "exti.h"

#include "usb/usb.h"
#include "usb/hid.h"

#include "LCD.h"
#include "keymap.h"

#include "config.h"


/*!<
 * config
 * */
extern const uint16_t PROGMEM key_map[][ROW_COUNT][COL_COUNT];


/*!<
 * functions
 * */
/*!< grid */
void init_grid(void);
void start_scan(void);
void stop_scan(void);
/*!< encoder */
void init_encoders(void);
void start_encoders(void);
void stop_encoders(void);
/*!< misc */
void press_key(uint16_t code);
void release_key(uint16_t code);
void release_all(void);
void send_text(const uint8_t* str);


#endif // SOFT_KEYBOARD_H
