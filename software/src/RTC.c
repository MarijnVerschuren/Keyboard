//
// Created by marijn on 9/17/24.
//
#include "RTC.h"


// static const
#define SEC_PER_MIN         0x3CUL
#define SEC_PER_HOUR        0xE10UL
#define SEC_PER_DAY         0x15180UL
#define SEC_PER_month
#define SEC_PER_YEAR		0x1E13380UL
#define UNIX_YEAR 1970

static uint8_t days_per_month[12U] = {
		31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

// static
static RTC_timestamp_t UNIX_BCD(uint32_t epoch) {
	uint16_t _year = (epoch / SEC_PER_YEAR);	epoch %= SEC_PER_YEAR;
	epoch -= (_year / 4 + (_year % 4 > 2)) * SEC_PER_DAY; _year += UNIX_YEAR;
	uint8_t _month, leap_flag = ((!(_year % 4) && _year % 100) || !(_year % 400));
	for (_month = 0; _month < 12; _month++) {
		uint32_t sec = ((days_per_month[_month] + (leap_flag && _month == 1)) * SEC_PER_DAY);
		if (sec > epoch) { break; } epoch -= sec;
	}
	RTC_timestamp_t t;
	t.year = _year - 2000; t.month = _month + 1;
	t.day =		(epoch / SEC_PER_DAY) + 1;	epoch %= SEC_PER_DAY;
	t.hour =	(epoch / SEC_PER_HOUR);		epoch %= SEC_PER_HOUR;
	t.min =		epoch / SEC_PER_MIN;		epoch %= SEC_PER_MIN;
	t.sec =		epoch;
	return t;
}


// init
void fconfig_RTC(
	uint8_t async_pre, uint16_t sync_pre, RTC_timestamp_t time,
	RTC_wakeup_t wakeup, RTC_wakeup_div_t wakeup_div, uint16_t wakeup_reload
) {
	RCC->APB1ENR |= 0x400;					// enable RCC bus clock
	PWR->CR |= 0x100UL;						// enable BDP
	while (!(PWR->CR & 0x100UL));
	RTC->WPR = 0xCAUL;						// write key 0 into the write protect register
	RTC->WPR = 0x53UL;						// write key 1 into the write protect register

	RTC->ISR |=	0x80UL;						// enter initialization mode (RTC is stopped)
	while (!(RTC->ISR & 0x40UL));			// wait until RTC enters initialization mode
	RTC->PRER = ((sync_pre - 1) & 0x7FFF);	// write synchronous prescaler		NOTE: there must be TWO writes to PRER
	RTC->PRER |= (((async_pre - 1) & 0x7F) << 16);	// asynchronous prescaler	NOTE: there must be TWO writes to PRER

	RTC->DR = (
		((time.year / 10U) << 20U)	|		// set year tens
		((time.year % 10U) << 16U)	|		// set year units
		((time.month / 10U) << 12U)	|		// set month tens
		((time.month % 10U) << 8U)	|		// set month units
		((time.day_unit) << 13U)	|		// set week day unit
		((time.day / 10U) << 4U)	|		// set day tens
		((time.day % 10U) << 0U)			// set day units
	);
	RTC->TR = (
		((time.hour / 10U) << 20U)	|		// set hour tens
		((time.hour % 10U) << 16U)	|		// set hour units
		((time.min / 10U) << 12U)	|		// set min tens
		((time.min % 10U) << 8U)	|		// set min units
		((time.sec / 10U) << 4U)	|		// set sec tens
		((time.sec % 10U) << 0U)			// set sec units
	);

	RTC->CR = (
			0x00000020UL					|	// enable shadow register bypass
			((wakeup & 0b1U) << 10U)		|	// set wake-up enable setting
			(((wakeup >> 1) & 0b1U) << 14U)	|	// set wake-up interrupt setting
			(wakeup_div << 0U)					// set wake-up clock setting
	);
	RTC->WUTR = wakeup_reload;				// set wake-up timer reload

	RTC->ISR &=	~0x80UL;					// exit initialization mode (RTC is started)
	while (RTC->ISR & 0x40UL);				// wait until RTC exits initialization mode
	RTC->WPR = 0x0UL;						// re-enable write protection
	PWR->CR &= ~0x100UL;					// disable BDP
}

void config_RTC(RTC_timestamp_t time, RTC_wakeup_t wakeup, RTC_wakeup_div_t wakeup_div, uint16_t wakeup_reload) {
	fconfig_RTC(
		RTC_clock_frequency / 0x100,
		0x100, time, wakeup, wakeup_div,
		wakeup_reload
	);
}
void uconfig_RTC(uint32_t epoch, RTC_wakeup_t wakeup, RTC_wakeup_div_t wakeup_div, uint16_t wakeup_reload) {
	fconfig_RTC(
		RTC_clock_frequency / 0x100U, 0x100U,
		UNIX_BCD(epoch), wakeup, wakeup_div,
		wakeup_reload
	);
}

void reset_RTC() {
	// TODO
}

void config_RTC_ext_ts(uint8_t int_enable, RTC_TS_pol_t pol) {
	PWR->CR |= 0x100UL;						// enable BDP
	while (!(PWR->CR & 0x100UL));
	RTC->WPR = 0xCAUL;						// write key 0 into the write protect register
	RTC->WPR = 0x53UL;						// write key 1 into the write protect register

	RTC->TAFCR &= ~(0b1UL << 17U);			// C13 selected as ts pin
	RTC->CR |= (pol << 3U);

	RTC->ISR &= ~0x00001800UL;				// clear TSF and TSOVF
	RTC->CR |= (
		0x00000800UL			|			// enable timestamp
		(int_enable << 15U)					// enable timestamp interrupt
	);

	RTC->WPR = 0x0UL;						// re-enable write protection
	PWR->CR &= ~0x100UL;					// disable BDP

	EXTI->RTSR |= (0b1u << 21U);
	EXTI->EMR |= (0b1u << 21U);  // unmask event
	EXTI->IMR |= (0b1u << 21U);  // unmask interrupt
	NVIC_set_IRQ_priority(RTC_STAMP_IRQn, 0);
	NVIC_enable_IRQ(RTC_STAMP_IRQn);
}

// misc
uint32_t RTC_unix(void) {
	uint32_t DR = RTC->DR;
	uint32_t TR = RTC->TR;
	uint16_t year =		((((DR >> 20U) & 0xFU) * 10) + ((DR >> 16U) & 0xFU)) + 2000;
	uint8_t	 month =	((((DR >> 12U) & 0x1U) * 10) + ((DR >> 8U) & 0xFU)) - 1;
	uint8_t	 day =		((((DR >> 4U) & 0x3U) * 10) + ((DR >> 0U) & 0xFU)) - 1;
	uint8_t	 hour =		((((TR >> 20U) & 0x3U) * 10) + ((TR >> 16U) & 0xFU));
	uint8_t	 min =		((((TR >> 12U) & 0x7U) * 10) + ((TR >> 8U) & 0xFU));
	uint8_t	 sec =		((((TR >> 4U) & 0x7U) * 10) + ((TR >> 0U) & 0xFU));

	uint8_t leap_flag = ((!(year % 4) && year % 100) || !(year % 400));
	year -= UNIX_YEAR; uint32_t epoch = year * SEC_PER_YEAR;
	epoch += (year / 4 + (year % 4 > 2) + (leap_flag && month > 1)) * SEC_PER_DAY;
	for (uint8_t i = 0; i < month; i++) { epoch += days_per_month[i] * SEC_PER_DAY; }
	epoch += day * SEC_PER_DAY;
	epoch += hour * SEC_PER_HOUR;
	epoch += min * SEC_PER_MIN;
	epoch += sec;
	return epoch;
}