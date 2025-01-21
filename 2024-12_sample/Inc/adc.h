/*
 * adc.h
 *
 *  Created on: Dec 23, 2024
 *      Author: jdjones
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx.h"


void init_sampling(void);


uint8_t get_buffer_0_full(void);
uint8_t get_buffer_1_full(void);


void reset_buffer_0_full(void);
void reset_buffer_1_full(void);


void print_buffer_0(void);
void print_buffer_1(void);

void DMA2_Stream0_IRQHandler(void);


#endif /* ADC_H_ */
