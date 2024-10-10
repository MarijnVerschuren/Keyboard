//
// Created by marijn on 9/4/24.
//

#ifndef STM32F412_BASE_H
#define STM32F412_BASE_H


typedef char				int8_t;
typedef short				int16_t;
typedef long				int32_t;
typedef long long			int64_t;

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned long		uint32_t;
typedef unsigned long long	uint64_t;

#define _I	volatile const
#define _O	volatile
#define _IO	volatile

#define NULL ((void*)0x00000000UL)

typedef struct {
	uint32_t	clk		: 4;		// clock bus
	uint32_t	periph	: 8;		// peripheral
	uint32_t	misc	: 8;		// misc info (tim channel number etc...)
	uint32_t	port	: 4;		// GPIO port
	uint32_t	pin		: 4;		// GPIO pin
	uint32_t	alt		: 4;		// alternate function
} dev_pin_t;  // 32 bit

typedef struct {
	volatile void*		ptr;
	uint32_t			size;
	volatile uint32_t	i;	// write
	volatile uint32_t	o;	// read
} io_buffer_t;


#endif // STM32F412_BASE_H
