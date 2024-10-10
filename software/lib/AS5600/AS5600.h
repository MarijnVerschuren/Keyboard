//
// Created by marijn on 9/24/24.
//

#ifndef STM32F412_AS5600_H
#define STM32F412_AS5600_H
#include "I2C.h"


#define AS5600_ADDRESS					0x36U
// AS5600 configuration registers
#define AS5600_REGISTER_ZMCO			0x00U
#define AS5600_REGISTER_ZPOS			0x01U
#define AS5600_REGISTER_MPOS			0x03U
#define AS5600_REGISTER_MANG			0x05U
#define AS5600_REGISTER_CONF			0x07U
// AS5600 output registers
#define AS5600_REGISTER_RAW_ANGLE		0x0CU
#define AS5600_REGISTER_ANGLE			0x0EU
// AS5600 status registers
#define AS5600_REGISTER_STATUS			0x0BU
#define AS5600_REGISTER_AGC				0x1AU
#define AS5600_REGISTER_MAGNITUDE_HIGH	0x1BU
#define AS5600_REGISTER_MAGNITUDE_LOW	0x1CU
#define AS5600_REGISTER_BURN			0xFFU


typedef enum {
	AS5600_POW_NOM =				0b00U << 8,
	AS5600_POW_LPM1 =				0b01U << 8,
	AS5600_POW_LPM2 =				0b10U << 8,
	AS5600_POW_LPM3 =				0b11U << 8
} AS5600_POW_t;

typedef enum {
	AS5600_HYST_OFF =				0b00U << 10,
	AS5600_HYST_1LSB =				0b01U << 10,
	AS5600_HYST_2LSB =				0b10U << 10,
	AS5600_HYST_3LSB =				0b11U << 10
} AS5600_HYST_t;

typedef enum {
	AS5600_MODE_FULL_ANALOG =		0b00U << 12,		// GND - VCC
	AS5600_MODE_REDUCED_ANALOG =	0b01U << 12,		// VCC(10%) - VCC(90%)
	AS5600_MODE_PWM =				0b10U << 12		// PWM
} AS5600_MODE_t;

typedef enum {
	AS5600_PWM_FREQ_115 =			0b00U << 14,
	AS5600_PWM_FREQ_230 =			0b01U << 14,
	AS5600_PWM_FREQ_460 =			0b10U << 14,
	AS5600_PWM_FREQ_920 =			0b11U << 14
} AS5600_PWM_FREQ_t;

typedef enum {
	AS5600_SFILTER_16 =				0b00U << 0,
	AS5600_SFILTER_8 =				0b01U << 0,
	AS5600_SFILTER_4 =				0b10U << 0,
	AS5600_SFILTER_2 =				0b11U << 0
} AS5600_SFILTER_t;

typedef enum {
	AS5600_FFILTER_OFF =			0b000U << 2,
	AS5600_FFILTER_6LSB =			0b001U << 2,
	AS5600_FFILTER_7LSB =			0b010U << 2,
	AS5600_FFILTER_9LSB =			0b011U << 2,
	AS5600_FFILTER_18LSB =			0b100U << 2,
	AS5600_FFILTER_21LSB =			0b101U << 2,
	AS5600_FFILTER_24LSB =			0b110U << 2,
	AS5600_FFILTER_10LSB =			0b111U << 2
} AS5600_FFILTER_t;

typedef enum {
	AS5600_WDG_OFF =				0b0 << 5,
	AS5600_WDG_ON =					0b1 << 5
} AS5600_WDG_t;


uint8_t config_AS5600(I2C_t* i2c, uint16_t flags, uint16_t timeout);
uint8_t AS5600_get_status(I2C_t* i2c, uint16_t timeout);
uint16_t AS5600_get_angle(I2C_t* i2c, uint16_t timeout);
uint8_t AS5600_set_start_position(I2C_t* i2c, uint16_t angle, uint16_t timeout);
uint8_t AS5600_set_stop_position(I2C_t* i2c, uint16_t angle, uint16_t timeout);
uint8_t AS5600_set_max_angle(I2C_t* i2c, uint16_t angle, uint16_t timeout);


#endif //STM32F412_AS5600_H
