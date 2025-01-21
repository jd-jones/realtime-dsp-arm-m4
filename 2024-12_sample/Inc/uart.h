/*
 * uart.h
 *
 *  Created on: Dec 16, 2024
 *      Author: jdjones
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#include "../Drivers/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h"


void uart2_tx_init(void);
void uart2_write(int ch);


#endif /* UART_H_ */
