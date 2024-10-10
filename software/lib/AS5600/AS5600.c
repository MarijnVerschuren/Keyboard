//
// Created by marijn on 9/24/24.
//

#include "AS5600.h"



uint8_t config_AS5600(I2C_t* i2c, uint16_t flags, uint16_t timeout) {
	if (I2C_master_write_reg(i2c, AS5600_ADDRESS, AS5600_REGISTER_CONF, (void*)&flags, 2, timeout) < 2) { return 0xFF; }
	uint8_t status = AS5600_get_status(i2c, timeout);
	if (status & 0/*0x18UL*/ || !(status & 0x20UL)) { return 0xFF; }
	return 0;
}

uint8_t AS5600_get_status(I2C_t* i2c, uint16_t timeout) {
	uint8_t status = 0;
	I2C_master_read_reg(i2c, AS5600_ADDRESS, AS5600_REGISTER_STATUS, &status, 1, timeout);
	return status;
}

uint16_t AS5600_get_angle(I2C_t* i2c, uint16_t timeout) {
	uint16_t angle = 0;
	I2C_master_read_reg(i2c, AS5600_ADDRESS, AS5600_REGISTER_ANGLE, &angle, 2, timeout);
	return (angle >> 8) | (angle << 8);
}

uint8_t AS5600_set_start_position(I2C_t* i2c, uint16_t angle, uint16_t timeout) {
	angle = (angle << 8) | (angle >> 8);
	return I2C_master_write_reg(i2c, AS5600_ADDRESS, AS5600_REGISTER_ZPOS, (void*)&angle, 2, timeout) != 2;
}

uint8_t AS5600_set_stop_position(I2C_t* i2c, uint16_t angle, uint16_t timeout) {
	angle = (angle << 8) | (angle >> 8);
	return I2C_master_write_reg(i2c, AS5600_ADDRESS, AS5600_REGISTER_MPOS, (void*)&angle, 2, timeout) != 2;
}

uint8_t AS5600_set_max_angle(I2C_t* i2c, uint16_t angle, uint16_t timeout) {
	angle = (angle << 8) | (angle >> 8);
	return I2C_master_write_reg(i2c, AS5600_ADDRESS, AS5600_REGISTER_MANG, (void*)&angle, 2, timeout) != 2;
}
