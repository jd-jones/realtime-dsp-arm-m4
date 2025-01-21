/*
 * uart.c
 *
 *  Created on: Dec 16, 2024
 *      Author: jdjones
 */

#include "uart.h"


#define SYS_FREQ		16000000
#define APB1_CLK		SYS_FREQ
#define UART_BAUDRATE	115200


static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate) {
	return (periph_clk + baudrate / 2U) / baudrate;
}


void uart2_write(int ch);


// This is used to retarget printf to serial output
int __io_putchar(int ch) {
	uart2_write(ch);
	return ch;
}


void uart2_tx_init(void) {
	// Enable clock access to GPIOA
	SET_BIT(RCC->AHB1ENR, RCC_AHB1RSTR_GPIOARST);

	// Set PA2 mode to alternate function mode
	CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODER2_0);
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODER2_1);

	// Set PA2 alternate function type to UART_TX (AF07)
	SET_BIT(GPIOA->AFR[0], GPIO_AFRL_AFSEL2_0);
	SET_BIT(GPIOA->AFR[0], GPIO_AFRL_AFSEL2_1);
	SET_BIT(GPIOA->AFR[0], GPIO_AFRL_AFSEL2_2);
	CLEAR_BIT(GPIOA->AFR[0], GPIO_AFRL_AFSEL2_3);

	// Enable clock access to UART2
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);

	// Configure baud rate
	USART2->BRR = compute_uart_bd(APB1_CLK, UART_BAUDRATE);

	// Configure the transfer direction
	WRITE_REG(USART2->CR1, USART_CR1_TE);

	// Enable UART module
	SET_BIT(USART2->CR1, USART_CR1_UE);
}


void uart2_write(int ch) {
	// Block until transmit data register is empty
	while (!READ_BIT(USART2->SR, USART_SR_TXE)) {}

	// Write to the transmit data register
	USART2->DR = (ch & 0xFF);
}
