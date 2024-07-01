//
// Created by marijn on 6/30/24.
//
#include "app/LCD.h"


/*!<
 * constants
 * */
const uint8_t PROGMEM init_sequence[] = {
	8,
	LCD_SLPOUT,	LCD_INIT_DELAY,									250,
	LCD_COLMOD,	LCD_INIT_DELAY + 1,	0x55,						5,
	LCD_MADCTL,	1,					0x00,
	LCD_CASET,	4,					0x00, 0x00, 0x00, 0xF0,
	LCD_PASET,	4,					0x00, 0x00, 0x00, 0xF0,
	LCD_INVON,	LCD_INIT_DELAY,									5,
	LCD_NORON,	LCD_INIT_DELAY,									5,
	LCD_DISPON,	LCD_INIT_DELAY,									250
};


/*!<
 * static functions
 * */
static inline void send_LCD_cmd(const uint8_t* commands) {
	uint8_t count = *commands++;
	uint8_t arg_count, delay;

	GPIO_write(LCD_PORT, LCD_CMD_PIN, 0);
	while (count--) {
		SPI_master_transmit(LCD_SPI, LCD_PORT, LCD_NCS_PIN, commands++, 1, 10);
		arg_count = *commands++;
		delay = arg_count & LCD_INIT_DELAY;
		arg_count &= ~LCD_INIT_DELAY;
		while (arg_count--) {
			SPI_master_transmit(LCD_SPI, LCD_PORT, LCD_NCS_PIN, commands++, 1, 10);
		} if (delay) { delay_ms(delay << 1); }
	}
}


/*!<
 * functions
 * */
void init_LCD(void) {
	config_GPIO(LCD_PORT, LCD_CMD_PIN, GPIO_output, GPIO_no_pull, GPIO_push_pull);
	config_GPIO(LCD_PORT, LCD_NRST_PIN, GPIO_output, GPIO_no_pull, GPIO_push_pull);
	config_GPIO(LCD_PORT, LCD_NCS_PIN, GPIO_output, GPIO_no_pull, GPIO_push_pull);
	config_GPIO(LCD_PORT, LCD_BL_PIN, GPIO_output, GPIO_no_pull, GPIO_push_pull);
	config_SPI_master(LCD_CLK_PIN, LCD_MOSI_PIN, SPI_PIN_DISABLE, LCD_SPI_DIV);

	GPIO_write(LCD_PORT, LCD_CMD_PIN, 1);
	GPIO_write(LCD_PORT, LCD_NRST_PIN, 1);
	GPIO_write(LCD_PORT, LCD_NCS_PIN, 1);
	GPIO_write(LCD_PORT, LCD_BL_PIN, 1);

	send_LCD_cmd(init_sequence);
}