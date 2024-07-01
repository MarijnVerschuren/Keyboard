//
// Created by marijn on 7/1/24.
//

#ifndef SOFT_CONFIG_H
#define SOFT_CONFIG_H


/*!<
 * config
 * */
#define HID_MIN_DELAY			20U

#define ROW_COUNT 5
#define COL_COUNT 15

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

#define LCD_SPI					SPI2
#define LCD_SPI_DIV				SPI_DIV_16	/* 17.5 Mbit / s */
#define LCD_PORT				GPIOB
#define LCD_CMD_PIN 			10
#define LCD_NRST_PIN			11
#define LCD_NCS_PIN				12
#define LCD_CLK_PIN				SPI2_SCK_B13
#define LCD_BL_PIN				14
#define LCD_MOSI_PIN			SPI2_MOSI_B15

#endif // SOFT_CONFIG_H
