//
// Created by marijn on 9/17/24.
//
#include "USART.h"


/*!< irq vars */
typedef struct {
	io_buffer_t*	rx_buf;
	uint32_t		rx_fifo;
} USART_IRQ_IO_t;

USART_IRQ_IO_t usart_buf_1;
USART_IRQ_IO_t usart_buf_2;
USART_IRQ_IO_t usart_buf_6;


/*!< irq handlers */
void USART_irq_handler(USART_t* usart, USART_IRQ_IO_t* io) {
	if(usart->SR & 0x00000020UL && io->rx_buf->ptr) {
		((uint8_t*)io->rx_buf->ptr)[io->rx_buf->i] = usart->DR;
		io->rx_buf->i++;
		if (io->rx_buf->i == io->rx_buf->size) {  // this is assuming that 'i' never skips
			if (io->rx_fifo) { io->rx_buf->i = 0; return; }	// reset offset
			usart->CR1 &= ~0x00000020UL;					// turn off irq
		}
	}
	// TODO: other events
}

extern void USART1_IRQHandler(void) { USART_irq_handler(USART1, &usart_buf_1); }
extern void USART2_IRQHandler(void) { USART_irq_handler(USART2, &usart_buf_2); }
extern void USART6_IRQHandler(void) { USART_irq_handler(USART6, &usart_buf_6); }


/*!< irq */
//void start_USART_read_irq(USART_TypeDef* usart, io_buffer_t* buffer, uint8_t fifo) {
//	usart->CR1 |= USART_CR1_RXNEIE;
//	uint32_t irqn;
//	USART_IRQ_IO_t* usart_buf;
//	if (usart == USART1)		{ irqn = USART1_IRQn; usart_buf = &usart_buf_1; }
//	else if (usart == USART2)	{ irqn = USART2_IRQn; usart_buf = &usart_buf_2; }
//	else if (usart == USART6)	{ irqn = USART6_IRQn; usart_buf = &usart_buf_6; }
//	else { return; }  // error
//	usart_buf->rx_buf = buffer;	usart_buf->rx_fifo = fifo;
//	NVIC->ISER[((irqn) >> 5UL)] = (uint32_t)(1UL << ((irqn) & 0x1FUL));  // NVIC_EnableIRQ
//}
//void stop_USART_read_irq(USART_TypeDef* usart) { usart->CR1 &= ~USART_CR1_RXNEIE; }
//void disable_USART_irq(USART_TypeDef* usart) {
//	uint32_t irqn;
//	if (usart == USART1) { irqn = USART1_IRQn; }
//	else if (usart == USART2) { irqn = USART2_IRQn; }
//	else if (usart == USART6) { irqn = USART6_IRQn; }
//	else { return; }  // error
//	NVIC->ICER[((irqn) >> 5UL)] = (uint32_t)(1UL << ((irqn) & 0x1FUL));  // NVIC_DisableIRQ
//	__DSB(); __ISB();  // flush processor pipeline before fetching
//}