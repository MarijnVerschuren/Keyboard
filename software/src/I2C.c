//
// Created by marijn on 9/24/24.
//

#include "I2C.h"


/*!< static */
static inline void enable_I2C_clock(I2C_t* i2c) {
	RCC->APB1ENR |= (0b1u << (((uint32_t)i2c - APB1PERIPH_BASE) >> 10u));
}

static inline void I2C_reset_flags(I2C_t* i2c) {
	(void)i2c->SR1;
	(void)i2c->SR2;
}

static inline uint8_t I2C_wait_idle(I2C_t* i2c, uint32_t timeout) {
	uint64_t start = tick;
	while (i2c->SR2 & 0x0002UL) { if (tick - start > timeout) { I2C_reset_flags(i2c); return -1; } }
	I2C_reset_flags(i2c);
	return 0;
}

static inline uint32_t I2C_transmit(I2C_t* i2c, const uint8_t* buffer, uint32_t size, uint32_t timeout) {
	uint64_t start = tick;
	for (uint32_t i = 0; i < size; i++) {
		while (!(i2c->SR1 & 0x0080UL || i2c->SR1 & 0x0004UL)) {
			if (tick - start > timeout || i2c->SR1 & 0x4000UL) { return i; }
		} i2c->DR = buffer[i];
	} while (!(i2c->SR1 & 0x0004UL)) {  // wait for BTF to set
		if (tick - start > timeout || i2c->SR1 & 0x4000UL) { return size - 1; }
	}
	I2C_reset_flags(i2c);
	return size;
}
static inline uint32_t I2C_receive(I2C_t* i2c, uint8_t* buffer, uint32_t size, uint32_t timeout) {
	uint64_t start = tick;
	for (uint32_t i = 0; i < size; i++) {
		// if (size - i == 1) { i2c->CR1 &= ~I2C_CR1_ACK; i2c->CR1 |= I2C_CR1_STOP; }  // stop transmission after next byte (TODO: check if needed)
		while (!(i2c->SR1 & 0x0040UL || i2c->SR1 & 0x0004UL)) {
			if (tick - start > timeout || i2c->SR1 & 0x4000UL) { return i; }
		} buffer[i] = i2c->DR;
	}
	I2C_reset_flags(i2c);
	return size;
}

static inline uint8_t I2C_start_master(I2C_t* i2c, uint8_t address, uint32_t timeout) {  // start and address
	uint64_t start = tick;
	i2c->CR1 |= 0x501UL;				// enable peripheral, RX ack and generate start bit
	while (!(  // wait until the start condition is sent
			(i2c->SR1 & 0x1UL) &&		// start bit detected
			(i2c->SR2 & 0x1UL) &&		// master mode (set when SR1_SB is set)
			(i2c->SR2 & 0x2UL)			// data transfer over bus started
	)) { if (tick - start > timeout || i2c->SR1 & 0x4000UL) { return -1; } }

	i2c->DR = address;  // send the address
	while (!(i2c->SR1 & 0x2UL)) {  // wait until the address is sent
		if (tick - start > timeout || i2c->SR1 & 0x4000UL) { return -1; }
	}

	I2C_reset_flags(i2c);
	return 0;
}

static inline void I2C_stop_master(I2C_t* i2c) {
	i2c->CR1 &= ~0x400UL;
	i2c->CR1 |= 0x200UL;  // send stop condition
	I2C_reset_flags(i2c);
}


/*!< init / enable / disable */
void disable_I2C_clock(I2C_t* i2c) {
	RCC->APB1ENR &= ~(0b1u << (((uint32_t)i2c - APB1PERIPH_BASE) >> 10u));
}
void fconfig_I2C(I2C_GPIO_t scl, I2C_GPIO_t sda, uint16_t own_address, I2C_address_t address_type, uint8_t dual_address) {
	uint8_t freq = APB1_clock_frequency / 1000000ul;
	if (freq < 2 || scl == I2C_PIN_DISABLE || sda == I2C_PIN_DISABLE) { return; }
	dev_pin_t	dscl = *((dev_pin_t*)&scl), dsda = *((dev_pin_t*)&sda);
	I2C_t		*scl_i2c = (I2C_t*)(APB1PERIPH_BASE + (dscl.periph << 10)),
				*sda_i2c = (I2C_t*)(APB1PERIPH_BASE + (dsda.periph << 10)),
				*i2c = NULL;
	if (scl_i2c != sda_i2c) { return; }; i2c = scl_i2c;
	fconfig_GPIO(int_to_GPIO(dscl.port), dscl.pin, GPIO_alt_func | GPIO_pull_up | GPIO_open_drain | GPIO_medium_speed, dscl.alt);
	fconfig_GPIO(int_to_GPIO(dsda.port), dsda.pin, GPIO_alt_func | GPIO_pull_up | GPIO_open_drain | GPIO_medium_speed, dsda.alt);
	enable_I2C_clock(i2c);
	reset_I2C(i2c);
	i2c->OAR1 = (address_type << 15) | (0x3ff & (own_address << (1 - address_type)));
	if (dual_address) { i2c->OAR2 = (0xff & ((dual_address << 1) | 0b1u)); }  // enable dual addressing
	i2c->OAR1 |= 0b1UL << 14;		// bit 14 should always be kept at one
	disable_I2C(i2c);
	i2c->CR1 |= 0x80UL;	// disable clock stretching
	i2c->CR2 = freq & 0x1FUL;		// set frequency
	i2c->CCR = 5 * freq;			// set scl freq to 1 MHz
	i2c->TRISE = freq + 1;			// set max rise time
	i2c->CR2 |= 0x700;
	enable_I2C(i2c);
	i2c->CR1 |= 0x400;
}
void config_I2C(I2C_GPIO_t scl, I2C_GPIO_t sda, uint8_t own_address) {
	fconfig_I2C(scl, sda, own_address, I2C_ADDR_7BIT, 0x00);
}
void reset_I2C(I2C_t* i2c) {
	disable_I2C(i2c);
	i2c->CR1 |= 0x00008000UL;
	while (!(i2c->CR1 & 0x00008000UL));
	i2c->CR1 &= ~0x00008000UL;
	while (i2c->CR1 & 0x00008000UL);
}
void enable_I2C(I2C_t* i2c) {
	i2c->CR1 |= 0x1UL;
	while (!(i2c->CR1 & 0x1UL));  // infinite loop if device is broken
}
void disable_I2C(I2C_t* i2c) {
	i2c->CR1 &= ~0x1UL;
	while (i2c->CR1 & 0x1UL);  // infinite loop if device is broken
}


/*!< master input / output */
uint8_t I2C_master_address(I2C_t* i2c, uint8_t i2c_address, uint32_t timeout) {
	uint64_t start = tick;
	if (I2C_wait_idle(i2c, timeout)) { return -1; }
	uint8_t ret = I2C_start_master(i2c, i2c_address << 1, timeout - (tick - start));
	I2C_stop_master(i2c);
	return ret;
}
uint32_t I2C_master_write(I2C_t* i2c, uint8_t i2c_address, const uint8_t* buffer, uint32_t size, uint32_t timeout) {
	uint64_t start = tick;
	if (I2C_wait_idle(i2c, timeout)) { return 0; }
	if (I2C_start_master(i2c, i2c_address << 1, timeout - (tick - start))) {
		I2C_stop_master(i2c);
		return 0;
	}
	uint32_t tx_size = I2C_transmit(i2c, buffer, size, timeout - (tick - start));
	I2C_stop_master(i2c);
	return tx_size;
}
uint32_t I2C_master_read(I2C_t* i2c, uint8_t i2c_address, uint8_t* buffer, uint32_t size, uint32_t timeout) {
	uint64_t start = tick;
	if (I2C_wait_idle(i2c, timeout)) { return 0; }
	if (I2C_start_master(i2c, (i2c_address << 1) | 0b1ul, timeout - (tick - start))) {
		I2C_stop_master(i2c);
		return 0;
	}
	uint32_t rx_size = I2C_receive(i2c, buffer, size, timeout - (tick - start));
	I2C_stop_master(i2c);
	return rx_size;
}
uint32_t I2C_master_write_reg(I2C_t* i2c, uint8_t i2c_address, uint8_t reg_address, const uint8_t* buffer, uint32_t size, uint32_t timeout) {
	uint64_t start = tick;
	if (I2C_wait_idle(i2c, timeout)) { return 0; }
	if (I2C_start_master(i2c, i2c_address << 1, timeout - (tick - start)) ||
		(!I2C_transmit(i2c, &reg_address, 1, timeout - (tick - start)))) {
		I2C_stop_master(i2c);
		return 0;
	}
	uint32_t tx_size = I2C_transmit(i2c, buffer, size, timeout - (tick - start));
	I2C_stop_master(i2c);
	return tx_size;
}
uint32_t I2C_master_read_reg(I2C_t* i2c, uint8_t i2c_address, uint8_t reg_address, uint8_t* buffer, uint32_t size, uint32_t timeout) {
	uint64_t start = tick;
	if (I2C_wait_idle(i2c, timeout)) { return 0; }

	uint8_t a = I2C_start_master(i2c, i2c_address << 1, timeout);
	uint8_t b = (!I2C_transmit(i2c, &reg_address, 1, timeout));
	uint8_t c = I2C_start_master(i2c, (i2c_address << 1) | 0b1ul, timeout);
	if (a || b || c) {
		I2C_stop_master(i2c);
		return 0;
	}
	uint32_t rx_size = I2C_receive(i2c, buffer, size, timeout - (tick - start));
	I2C_stop_master(i2c);
	return rx_size;
}