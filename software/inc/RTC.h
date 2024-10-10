//
// Created by marijn on 9/17/24.
//

#ifndef STM32F412_RTC_H
#define STM32F412_RTC_H
#include "periph.h"
#include "sys.h"
#include "EXTI.h"


// types
typedef enum {
	RTC_DAY_NONE =		0b000U,
	RTC_DAY_MONDAY =	0b001U,
	RTC_DAY_TEUSDAY =	0b010U,
	RTC_DAY_WEDNESDAY =	0b011U,
	RTC_DAY_THIRSDAY =	0b100U,
	RTC_DAY_FRIDAY =	0b101U,
	RTC_DAY_SATURDAY =	0b110U,
	RTC_DAY_SUNDAY =	0b111U
} RTC_day_t;

typedef struct {
	uint32_t year		: 8;		// 2000 - 2256
	uint32_t month		: 4;
	uint32_t day_unit	: 3;		// RTC_day_t
	uint32_t day		: 5;
	uint32_t min		: 6;
	uint32_t sec		: 6;
	uint32_t hour		: 5;
	uint32_t _ : 27;
} RTC_timestamp_t;

typedef enum {
	RTC_WAKEUP_DIV16 =				0b000U,
	RTC_WAKEUP_DIV8 =				0b001U,
	RTC_WAKEUP_DIV4 =				0b010U,
	RTC_WAKEUP_DIV2 =				0b011U,
	RTC_WAKEUP_RCC =				0b100U,
	RTC_WAKEUP_RCC_CNT_INCREASE =	0b110U
} RTC_wakeup_div_t;

typedef enum {
	RTC_WAKEUP_DISABLE =			0b00U,
	RTC_WAKEUP_ENABLE =				0b01U,
	RTC_WAKEUP_ENABLE_INT =			0b11U
} RTC_wakeup_t;

typedef enum {
	RTC_TS_POLARITY_RISING =		0b0U,
	RTC_TS_POLARITY_FALLING =		0b1U,
} RTC_TS_pol_t;


// init
void fconfig_RTC(
	uint8_t async_pre, uint16_t sync_pre, RTC_timestamp_t time,
	RTC_wakeup_t wakeup, RTC_wakeup_div_t wakeup_div, uint16_t wakeup_reload
);
void config_RTC(RTC_timestamp_t time, RTC_wakeup_t wakeup, RTC_wakeup_div_t wakeup_div, uint16_t wakeup_reload);
void uconfig_RTC(uint32_t time, RTC_wakeup_t wakeup, RTC_wakeup_div_t wakeup_div, uint16_t wakeup_reload);
void reset_RTC();
// extra
void config_RTC_ext_ts(uint8_t int_enable, RTC_TS_pol_t pol);

// misc
uint32_t RTC_unix(void);



#endif //STM32F412_RTC_H
