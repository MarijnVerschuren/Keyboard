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


/*!<
 * defines
 * */
#define HID_MIN_DELAY			20U

#define ROW_PORT				GPIOE
#define COL_PORT				GPIOD
#define SCAN_TIM				TIM1		/* TIM1_UP_IRQHandler */
#define SCAN_TIM_KERNEL_FREQ	TIM_APB1_kernel_frequency

#define ENC0_TIM				TIM2
#define ENC1_TIM				TIM3
#define ENC0_A					TIM2_CH1_A0
#define ENC0_B					TIM2_CH2_A1
#define ENC1_A					TIM3_CH1_B4
#define ENC1_B					TIM3_CH2_B5
#define ENC0_SW_PORT			GPIOA
#define ENC0_SW_PIN				2U			/* EXTI2_IRQHandler */
#define ENC1_SW_PORT			GPIOB
#define ENC1_SW_PIN				3U			/* EXTI3_IRQHandler */


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
void send_text(const uint8_t* str);


#endif // SOFT_KEYBOARD_H
