#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"



void enable_fpu(void) {
	// Enable FPU coprocessor at index 10
	SET_BIT(SCB->CPACR, 1UL << 20);
	SET_BIT(SCB->CPACR, 1UL << 21);

	// Enable FPU coprocessor at index 11
	SET_BIT(SCB->CPACR, 1UL << 22);
	SET_BIT(SCB->CPACR, 1UL << 23);
}


void setup(void) {
	enable_fpu();
	uart2_tx_init();
	init_sampling();
}


int main(void) {
	setup();

	printf("PA0_unfiltered\tPA0_filtered\r\n");


	while(1){}
}
