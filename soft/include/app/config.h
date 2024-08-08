//
// Created by marijn on 7/1/24.
//

#ifndef SOFT_CONFIG_H
#define SOFT_CONFIG_H


/*!<
 * config
 * */
#define MIN_HID_DELAY			20U

#define ROW_COUNT				5
#define COL_COUNT				15 /* 16 is max!! */

#define NORMAL
#ifdef NORMAL
#define LCD_SPI					SPI2
#define LCD_SPI_DIV				SPI_DIV_16	/* 17.5 Mbit / s */
#define LCD_PORT				GPIOB
#define LCD_CMD_PIN 			10
#define LCD_NRST_PIN			11
#define LCD_NCS_PIN				12
#define LCD_CLK_PIN				SPI2_SCK_B13
#define LCD_BL_PIN				14
#define LCD_MOSI_PIN			SPI2_MOSI_B15
#else
#define LCD_SPI					SPI3
#define LCD_SPI_DIV				SPI_DIV_16	/* 17.5 Mbit / s */
#define LCD_PORT				GPIOC
#define LCD_CMD_PIN 			11
#define LCD_NRST_PIN			6
#define LCD_NCS_PIN				9
#define LCD_CLK_PIN				SPI3_SCK_C10
#define LCD_BL_PIN				13
#define LCD_MOSI_PIN			SPI3_MOSI_C12
#endif

#endif // SOFT_CONFIG_H
