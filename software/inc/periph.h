//
// Created by marijn on 9/7/24.
//

#ifndef STM32F412_PERIPH_H
#define STM32F412_PERIPH_H
#include "memory_map.h"
#include "base.h"


/*!< core peripherals */
#define SCB						((SCB_t*)SCB_BASE)
#define NVIC					((NVIC_t*)NVIC_BASE)
#define SYS_TICK				((SYS_TICK_t*)SYS_TICK_BASE)


/*!< APB1 peripherals */
#define TIM2					((TIM_t*)TIM2_BASE)
#define TIM3					((TIM_t*)TIM3_BASE)
#define TIM4					((TIM_t*)TIM4_BASE)
#define TIM5					((TIM_t*)TIM5_BASE)
#define TIM6					((TIM_t*)TIM6_BASE)
#define TIM7					((TIM_t*)TIM7_BASE)
#define TIM12					((TIM_t*)TIM12_BASE)
#define TIM13					((TIM_t*)TIM13_BASE)
#define TIM14					((TIM_t*)TIM14_BASE)
#define RTC						((RTC_t*)RTC_BASE)
#define USART2					((USART_t*)USART2_BASE)
#define USART3					((USART_t*)USART3_BASE)
#define I2C1					((I2C_t*)I2C1_BASE)
#define I2C2					((I2C_t*)I2C2_BASE)
#define I2C3					((I2C_t*)I2C3_BASE)
#define PWR						((PWR_t*)PWR_BASE)


/*!< APB2 peripherals */
#define TIM1					((TIM_t*)TIM1_BASE)
#define TIM8					((TIM_t*)TIM8_BASE)
#define USART1					((USART_t*)USART1_BASE)
#define USART6					((USART_t*)USART6_BASE)
#define ADC1					((ADC_t*)ADC1_BASE)
#define ADC_COMMON				((ADC_COMMON_t*)ADC_COMMON_BASE)
#define SYSCFG					((SYSCFG_t*)SYSCFG_BASE)
#define EXTI					((EXTI_t *)EXTI_BASE)
#define TIM9					((TIM_t*)TIM9_BASE)
#define TIM10					((TIM_t*)TIM10_BASE)
#define TIM11					((TIM_t*)TIM11_BASE)


/*!< AHB1 peripherals */
#define GPIOA					((GPIO_t*)GPIOA_BASE)
#define GPIOB					((GPIO_t*)GPIOB_BASE)
#define GPIOC					((GPIO_t*)GPIOC_BASE)
#define GPIOH					((GPIO_t*)GPIOH_BASE)
#define RCC						((RCC_t*)RCC_BASE)
#define FLASH					((FLASH_t*)FLASH_BASE)

/*!< AHB2 peripherals */


/*!<
 * core peripheral types
 * */
typedef struct {
	_IO uint32_t	CTRL;			/* control and status                0x00 */
	_IO uint32_t	LOAD;			/* reload value                      0x04 */
	_IO uint32_t	VAL;			/* current value                     0x08 */
	_I  uint32_t	CALIB;			/* calibration                       0x0C */
} SYS_TICK_t;

typedef struct {
	_IO uint32_t ISER[8U];			/* interrupt enable                 0x000 */
	uint32_t _0[24U];
	_IO uint32_t ICER[8U];			/* interrupt disable                0x080 */
	uint32_t _1[24U];
	_IO uint32_t ISPR[8U];			/* interrupt set pending            0x100 */
	uint32_t _2[24U];
	_IO uint32_t ICPR[8U];			/* interrupt clear pending			0x180 */
	uint32_t _3[24U];
	_IO uint32_t IABR[8U];			/* interrupt active bit             0x200 */
	uint32_t _4[56U];
	_IO uint8_t  IP[240U];			/* interrupt priority               0x300 */
	uint32_t _5[644U];
	_O	uint32_t STIR;				/* software trigger interrupt       0xE00 */
} NVIC_t;

typedef struct {
	_I  uint32_t	CPUID;			/* CPUID base                        0x00 */
	_IO uint32_t	ICSR;			/* interrupt control and state       0x04 */
	_IO uint32_t	VTOR;			/* vector table offset               0x08 */
	_IO uint32_t	AIRCR;			/* software interrupt, reset control 0x0C */
	_IO uint32_t	SCR;			/* system control                    0x10 */
	_IO uint32_t	CCR;			/* configuration control             0x14 */
	_IO uint8_t 	SHP[12U];		/* system handler priority      0x18-0x23 */
	_IO uint32_t	SHCSR;			/* system handler control and state  0x24 */
	_IO uint32_t	CFSR;			/* configurable fault status         0x28 */
	_IO uint32_t	HFSR;			/* hard fault status                 0x2C */
	_IO uint32_t	DFSR;			/* debug fault status                0x30 */
	_IO uint32_t	MMFAR;			/* memory manage fault address       0x34 */
	_IO uint32_t	BFAR;			/* bus fault address                 0x38 */
	_IO uint32_t	AFSR;			/* auxiliary fault status            0x3C */
	_I  uint32_t	PFR[2U];		/* processor feature            0x40-0x44 */
	_I  uint32_t	DFR;			/* debug feature                     0x48 */
	_I  uint32_t	ADR;			/* auxiliary feature                 0x4C */
	_I  uint32_t	MMFR[4U];		/* memory model feature         0x50-0x5C */
	_I  uint32_t	ISAR[5U];		/* instruction set attributes   0x60-0x70 */
	uint32_t		_0[5U];			/*                              0x74-0x84 */
	_IO uint32_t	CPACR;			/* coprocessor access control        0x88 */
} SCB_t;


/*!<
 * peripheral types
 * */
/*!< PWR */
typedef struct {
	_IO uint32_t	CR;				/* power control                     0x00 */
	_IO uint32_t	CSR;			/* power control and status          0x04 */
} PWR_t;

/*!< FLASH */
typedef struct {
	_IO uint32_t	ACR;			/* access control                    0x00 */
	_IO uint32_t	KEYR;			/* key                               0x04 */
	_IO uint32_t	OPTKEYR;		/* option key                        0x08 */
	_IO uint32_t	SR;				/* status                            0x0C */
	_IO uint32_t	CR;				/* control                           0x10 */
	_IO uint32_t	OPTCR;			/* option control                    0x14 */
	_IO uint32_t	OPTCR1;			/* option control 1                  0x18 */
} FLASH_t;

/*!< RCC */
typedef struct {
	_IO uint32_t	CR;				/* clock control                     0x00 */
	_IO uint32_t	PLLCFGR;		/* PLL configuration                 0x04 */
	_IO uint32_t	CFGR;			/* clock configuration               0x08 */
	_IO uint32_t	CIR;			/* clock interrupt                   0x0C */
	_IO uint32_t	AHB1RSTR;		/* AHB1 peripheral disable           0x10 */
	_IO uint32_t	AHB2RSTR;		/* AHB2 peripheral disable           0x14 */
		uint32_t	_0[2];			/*                              0x18-0x1C */
	_IO uint32_t	APB1RSTR;		/* APB1 peripheral disable           0x20 */
	_IO uint32_t	APB2RSTR;		/* APB2 peripheral disable           0x24 */
		uint32_t	_1[2];			/*                              0x28-0x2C */
	_IO uint32_t	AHB1ENR;		/* AHB1 peripheral enable            0x30 */
	_IO uint32_t	AHB2ENR;		/* AHB2 peripheral enable            0x34 */
		uint32_t	_2[2];			/*                              0x38-0x3C */
	_IO uint32_t	APB1ENR;		/* APB1 peripheral enable            0x40 */
	_IO uint32_t	APB2ENR;		/* APB2 peripheral enable            0x44 */
		uint32_t	_3[2];			/*                              0x48-0x4C */
	_IO uint32_t	AHB1LPENR;		/* AHB1 peripheral low power enable  0x50 */
	_IO uint32_t	AHB2LPENR;		/* AHB2 peripheral low power enable  0x54 */
		uint32_t	_4[2];			/*                              0x58-0x5C */
	_IO uint32_t	APB1LPENR;		/* APB1 peripheral low power enable  0x60 */
	_IO uint32_t	APB2LPENR;		/* APB2 peripheral low power enable  0x64 */
		uint32_t	_5[2];			/*                              0x68-0x6C */
	_IO uint32_t	BDCR;			/* backup domain control             0x70 */
	_IO uint32_t	CSR;			/* clock control & status            0x74 */
		uint32_t	_6[2];			/*                              0x78-0x7C */
	_IO uint32_t	SSCGR;			/* spread spectrum clock generation  0x80 */
	_IO uint32_t	PLLI2SCFGR;		/* PLLI2S configuration              0x84 */
		uint32_t	_7;				/*                                   0x88 */
	_IO uint32_t	DCKCFGR;		/* dedicated clocks configuration    0x8C */
	_IO uint32_t	CKGATENR;		/* clocks gated enable               0x90 */
	_IO uint32_t	DCKCFGR1;		/* dedicated clocks configuration 1  0x94 */
} RCC_t;

/*!< SYSCFG */
typedef struct {
	_IO uint32_t MEMRMP;			/* memory remap                      0x00 */
	_IO uint32_t PMC;				/* peripheral mode configuration     0x04 */
	_IO uint32_t EXTICR[4];			/* external interrupt configuration  0x08-0x14 */
		uint32_t _;
	_IO uint32_t CFGR2;				/* configuration                     0x1C */
	_IO uint32_t CMPCR;				/* compensation cell control         0x20 */
	_IO uint32_t CFGR;				/* configuration                     0x24 */
} SYSCFG_t;

/*!< GPIO */
typedef struct {
	_IO uint32_t	MODER;			/* port mode                         0x00 */
	_IO uint32_t	OTYPER;			/* port output type                  0x04 */
	_IO uint32_t	OSPEEDR;		/* port output speed                 0x08 */
	_IO uint32_t	PUPDR;			/* port pull-up/pull-down            0x0C */
	_IO uint32_t	IDR;			/* port input data                   0x10 */
	_IO uint32_t	ODR;			/* port output data                  0x14 */
	_IO uint32_t	BSRR;			/* port bit set/reset                0x18 */
	_IO uint32_t	LCKR;			/* port configuration lock           0x1C */
	_IO uint32_t	AFR[2];			/* alternate function           0x20-0x24 */
} GPIO_t;

/*!< EXTI */
typedef struct {
	_IO uint32_t IMR;				/* interrupt mask                    0x00 */
	_IO uint32_t EMR;				/* event mask                        0x04 */
	_IO uint32_t RTSR;				/* rising trigger selection          0x08 */
	_IO uint32_t FTSR;				/* falling trigger selection         0x0C */
	_IO uint32_t SWIER;				/* software interrupt event          0x10 */
	_IO uint32_t PR;				/* pending                           0x14 */
} EXTI_t;

/*!< TIM */
typedef struct {
	_IO uint32_t CR1;				/* control 1                         0x00 */
	_IO uint32_t CR2;				/* control 2                         0x04 */
	_IO uint32_t SMCR;				/* slave mode control                0x08 */
	_IO uint32_t DIER;				/* DMA/interrupt enable              0x0C */
	_IO uint32_t SR;				/* status                            0x10 */
	_IO uint32_t EGR;				/* event generation                  0x14 */
	_IO uint32_t CCMR1;				/* capture/compare mode 1            0x18 */
	_IO uint32_t CCMR2;				/* capture/compare mode 2            0x1C */
	_IO uint32_t CCER;				/* capture/compare enable            0x20 */
	_IO uint32_t CNT;				/* counter                           0x24 */
	_IO uint32_t PSC;				/* prescaler,                        0x28 */
	_IO uint32_t ARR;				/* auto-reload                       0x2C */
	_IO uint32_t RCR;				/* repetition counter                0x30 */
	_IO uint32_t CCR1;				/* capture/compare 1                 0x34 */
	_IO uint32_t CCR2;				/* capture/compare 2                 0x38 */
	_IO uint32_t CCR3;				/* capture/compare 3                 0x3C */
	_IO uint32_t CCR4;				/* capture/compare 4                 0x40 */
	_IO uint32_t BDTR;				/* break and dead-time               0x44 */
	_IO uint32_t DCR;				/* DMA control                       0x48 */
	_IO uint32_t DMAR;				/* DMA address for full transfer     0x4C */
	_IO uint32_t OR;				/* option                            0x50 */
} TIM_t;

/*!< RTC */
typedef struct {
	_IO uint32_t	TR;				/* time                              0x00 */
	_IO uint32_t	DR;				/* date                              0x04 */
	_IO uint32_t	CR;				/* control                           0x08 */
	_IO uint32_t	ISR;			/* initialization and status         0x0C */
	_IO uint32_t	PRER;			/* prescaler                         0x10 */
	_IO uint32_t	WUTR;			/* wakeup timer                      0x14 */
	_IO uint32_t	CALIBR;			/* calibration                       0x18 */
	_IO uint32_t	ALRMAR;			/* alarm A                           0x1C */
	_IO uint32_t	ALRMBR;			/* alarm B                           0x20 */
	_IO uint32_t	WPR;			/* write protection                  0x24 */
	_IO uint32_t	SSR;			/* sub second                        0x28 */
	_IO uint32_t	SHIFTR;			/* shift control                     0x2C */
	_IO uint32_t	TSTR;			/* time stamp time                   0x30 */
	_IO uint32_t	TSDR;			/* time stamp date                   0x34 */
	_IO uint32_t	TSSSR;			/* time-stamp sub second             0x38 */
	_IO uint32_t	CALR;			/* calibration                       0x3C */
	_IO uint32_t	TAFCR;			/* tamper and af configuration       0x40 */
	_IO uint32_t	ALRMASSR;		/* alarm A sub second                0x44 */
	_IO uint32_t	ALRMBSSR;		/* alarm B sub second                0x48 */
		uint32_t	_0;				/*                                   0x88 */
	_IO uint32_t	BKPR[20];		/* backup registers             0x50-0x9C */
} RTC_t;

/*!< USART */
typedef struct {
	_IO uint32_t	SR;				/* status                            0x00 */
	_IO uint32_t	DR;				/* data                              0x04 */
	_IO uint32_t	BRR;			/* baud rate                         0x08 */
	_IO uint32_t	CR1;			/* Control 1                         0x0C */
	_IO uint32_t	CR2;			/* Control 2                         0x10 */
	_IO uint32_t	CR3;			/* Control 3                         0x14 */
	_IO uint32_t	GTPR;			/* Guard time and prescaler          0x18 */
} USART_t;

/*!< IWDG TODO */

/*!< CRC TODO */

/*!< RNG TODO */

/*!< ADC */
typedef struct {
	_IO uint32_t SR;				/* status                            0x00 */
	_IO uint32_t CR1;				/* control 1                         0x04 */
	_IO uint32_t CR2;				/* control 2                         0x08 */
	_IO uint32_t SMPR1;				/* sample time 1                     0x0C */
	_IO uint32_t SMPR2;				/* sample time 2                     0x10 */
	_IO uint32_t JOFR1;				/* injected channel data offset 1    0x14 */
	_IO uint32_t JOFR2;				/* injected channel data offset 2    0x18 */
	_IO uint32_t JOFR3;				/* injected channel data offset 3    0x1C */
	_IO uint32_t JOFR4;				/* injected channel data offset 4    0x20 */
	_IO uint32_t HTR;				/* watchdog higher threshold         0x24 */
	_IO uint32_t LTR;				/* watchdog lower threshold          0x28 */
	_IO uint32_t SQR1;				/* regular sequence 1                0x2C */
	_IO uint32_t SQR2;				/* regular sequence 2                0x30 */
	_IO uint32_t SQR3;				/* regular sequence 3                0x34 */
	_IO uint32_t JSQR;				/* injected sequence                 0x38 */
	_IO uint32_t JDR1;				/* injected data 1                   0x3C */
	_IO uint32_t JDR2;				/* injected data 2                   0x40 */
	_IO uint32_t JDR3;				/* injected data 3                   0x44 */
	_IO uint32_t JDR4;				/* injected data 4                   0x48 */
	_IO uint32_t DR;				/* regular data                      0x4C */
} ADC_t;

typedef struct {
	_IO uint32_t CSR;				/* status                    ADC1 + 0x300 */
	_IO uint32_t CCR;				/* control                   ADC1 + 0x304 */
	_IO uint32_t CDR;				/* dual / triple mode data   ADC1 + 0x308 */
} ADC_COMMON_t;

/*!< I2C */
typedef struct {
	_IO uint32_t CR1;				/* control 1                         0x00 */
	_IO uint32_t CR2;				/* control 2                         0x04 */
	_IO uint32_t OAR1;				/* own address 1                     0x08 */
	_IO uint32_t OAR2;				/* own address 2                     0x0C */
	_IO uint32_t DR;				/* data                              0x10 */
	_IO uint32_t SR1;				/* status 1                          0x14 */
	_IO uint32_t SR2;				/* status 2                          0x18 */
	_IO uint32_t CCR;				/* clock control                     0x1C */
	_IO uint32_t TRISE;				/* TRISE                             0x20 */
	_IO uint32_t FLTR;				/* FLTR                              0x24 */
} I2C_t;


#endif // STM32F412_PERIPH_H
