//
// Created by marijn on 6/30/24.
//

#ifndef SOFT_LCD_H
#define SOFT_LCD_H
#include "spi/spi.h"


/*!<
 * defines
 * */
#define LCD_SPI			SPI2
#define LCD_SPI_DIV		SPI_DIV_32	/* 280 Mhz / 32 */
#define LCD_PORT		GPIOB
#define LCD_CMD_PIN 	10
#define LCD_NRST_PIN	11
#define LCD_NCS_PIN		12
#define LCD_CLK_PIN		SPI2_SCK_B13
#define LCD_BL_PIN		14
#define LCD_MOSI_PIN	SPI2_MOSI_B15


/*!<
 * types
 * */
#define LCD_INIT_DELAY 0x80
typedef enum {
	LCD_NOP =		0x00,
	LCD_SWRST =		0x01,

	LCD_SLPIN =		0x10,
	LCD_SLPOUT =	0x11,
	LCD_NORON =		0x12,

	LCD_INVOFF =	0x20,
	LCD_INVON =		0x21,
	LCD_DISPOFF =	0x28,
	LCD_DISPON =	0x29,
	LCD_CASET =		0x2A,
	LCD_PASET =		0x2B,
	LCD_RAMWR =		0x2C,
	LCD_RAMRD =		0x2E,
	LCD_MADCTL =	0x36,
	LCD_COLMOD =	0x3A,
} LCD_CMD_t;


/*!<
 * functions
 * */
void init_LCD(void);


#endif // SOFT_LCD_H
