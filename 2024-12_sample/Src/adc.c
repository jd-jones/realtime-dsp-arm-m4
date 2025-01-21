#include <stdio.h>
#include "stm32f4xx.h"
#include "adc.h"
#include "arm_math.h"


#define BUFFER_LEN 						16
#define FIR_FILTER_LEN					29
#define FIR_FILTER_STATE_BUFFER_LEN 	FIR_FILTER_LEN + BUFFER_LEN - 1

/*----------------------------------------------------------------------------+
 | SAMPLE RATE COMPUTATION FROM ADC CLOCK FREQUENCY
 |
 | AHPB2 clock frequency (default; can be prescaled):
 | f_pclk2 = 60 MHz
 |
 | ADC clock frequency
 |   ADCPRE: ADC prescaler
 |   Set and cleared by software to select the frequency of the clock to the ADC.
 |     00: PCLK2 divided by 2	(default)
 |     01: PCLK2 divided by 4
 |     10: PCLK2 divided by 6
 |     11: PCLK2 divided by 8
 | f_adc = f_pclk2 / ADCPRE_cycles
 |
 | ADC sample frequency for each channel
 |   SMPx[2:0]: Channel x sampling time selection
 |   These bits are written by software to select the sampling time individually for each channel.
 |   During sampling cycles, the channel selection bits must remain unchanged.
 |     000:   3 cycles	(default)
 |     001:  15 cycles
 |     010:  28 cycles
 |     011:  56 cycles
 |     100:  84 cycles
 |     101: 112 cycles
 |     110: 144 cycles
 |     111: 480 cycles
 | f_s_chx = f_adc / SMPx_cycles
 |
 | EXAMPLE 1: Default settings
 |   f_pclk2 = 60000000 Hz
 |   ADCPRE_cycles = 2
 |   SMPx_cycles = 3
 |   --> f_s_chx = (60 MHz / 2) / 3 = 10 MHz
 |
 | EXAMPLE 2: Minimum settings
 |   f_pclk2 = 60000000 Hz
 |   ADCPRE_cycles = 8
 |   SMPx_cycles = 480
 |   --> f_s_chx = (60 MHz / 8) / 480 = 15.6 KHz
 |
 *---------------------------------------------------------------------------*/

uint16_t ADC_RESOLUTION = 12;
float32_t MAX_ADC_OUTPUT = 4095.0;  // 2 ^ 12 - 1
float32_t ADC_VREF = 3.3; 		    // In Volts

uint16_t input_buffer_0[BUFFER_LEN];
uint16_t input_buffer_1[BUFFER_LEN];
float32_t unfiltered_buffer[BUFFER_LEN];
float32_t filtered_buffer[BUFFER_LEN];
float32_t fir_filter_state_buffer[FIR_FILTER_STATE_BUFFER_LEN];

arm_fir_instance_f32 fir_filter;
const float32_t fir_coeffs[FIR_FILTER_LEN] = {
		0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f,
		0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f,
		0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f,
		0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f, 0.03448275862f,
		0.03448275862f
};


float32_t adc_bits_to_volts(uint16_t adc_bits) {
	return (float32_t) adc_bits / MAX_ADC_OUTPUT * ADC_VREF;
}


void print_buffer(const float32_t * buffer, int buffer_len) {
	for (int i = 0; i < buffer_len; i++) {
		printf("%f\r\n", buffer[i]);
	}
}


void print_buffers(const float32_t * buffer1, const float32_t * buffer2, int buffer_len) {
	for (int i = 0; i < buffer_len; i++) {
		printf("%f\t%f\r\n", buffer1[i], buffer2[i]);
	}
}


void process_buffer(const uint16_t * in_buffer, int in_buffer_len, float32_t * unfiltered_buffer, float32_t * filtered_buffer) {
	for (int i = 0; i < in_buffer_len; i++) {
		unfiltered_buffer[i] = adc_bits_to_volts(in_buffer[i]);
	}

	arm_fir_f32(
		&fir_filter,
		unfiltered_buffer,
		filtered_buffer,
		in_buffer_len
	);
}


void set_pa0_analog() {
	// Enable clock access to GPIOA
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);

	// Set the mode of PA0 to analog
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODER0_0);
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODER0_1);
}


void init_adc_pa0(void) {
	// Enable clock access to ADC
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);

	// Set first conversion to Channel 0
	// CLEAR_BIT(ADC1->SQR3, ADC_SQR3_SQ1_0);
	// CLEAR_BIT(ADC1->SQR3, ADC_SQR3_SQ1_1);
	// CLEAR_BIT(ADC1->SQR3, ADC_SQR3_SQ1_2);
	// CLEAR_BIT(ADC1->SQR3, ADC_SQR3_SQ1_3);
	// CLEAR_BIT(ADC1->SQR3, ADC_SQR3_SQ1_4);

	// Conversion sequence length
	// CLEAR_BIT(ADC1->SQR1, ADC_SQR1_L_0);
	// CLEAR_BIT(ADC1->SQR1, ADC_SQR1_L_1);
	// CLEAR_BIT(ADC1->SQR1, ADC_SQR1_L_2);
	// CLEAR_BIT(ADC1->SQR1, ADC_SQR1_L_3);

	// Set sample resolution
	//   00: 12-bit (15 ADCCLK cycles)
	//   01: 10-bit (13 ADCCLK cycles)
	//   10:  8-bit (11 ADCCLK cycles)
	//   11:  6-bit (9 ADCCLK cycles)
	// CLEAR_BIT(ADC1->CR1, ADC_CR1_RES_0);
	// CLEAR_BIT(ADC1->CR1, ADC_CR1_RES_1);

	// Set sample time
	//   000: 3 cycles
	//   001: 15 cycles
	//   010: 28 cycles
	//   011: 56 cycles
	//   100: 84 cycles
	//   101: 112 cycles
	//   110: 144 cycles
	//   111: 480 cycles
	// CLEAR_BIT(ADC1->SMPR1, ADC_SMPR2_SMP0_0);
	// CLEAR_BIT(ADC1->SMPR1, ADC_SMPR2_SMP0_1);
	// CLEAR_BIT(ADC1->SMPR1, ADC_SMPR2_SMP0_2);

	// Enable DMA
	SET_BIT(ADC1->CR2, ADC_CR2_DMA);
	SET_BIT(ADC1->CR2, ADC_CR2_DDS);

	// Enable external timer control on rising edge, triggered by Timer 2 TRGO event
	SET_BIT(ADC1->CR2, ADC_CR2_EXTEN_0);
	SET_BIT(ADC1->CR2, ADC_CR2_EXTSEL_1);
	SET_BIT(ADC1->CR2, ADC_CR2_EXTSEL_2);
}


void init_timer(void) {
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);

	// The counter clock frequency (CK_CNT) is equal to fCK_PSC / (PSC[15:0] + 1).
	// PSC contains the value to be loaded in the active prescaler register at each update event
	// (including when the counter is cleared through UG bit of TIMx_EGR register or through
	// trigger controller when configured in “reset mode”).
	TIM2->PSC = 1600 - 1;  //  16 000 000 / 16 00 = 10 000

	// ARR is the value to be loaded in the actual auto-reload register.
	// Refer to Section 12.3.1: Time-base unit for more details about ARR update and behavior.
	// The counter is blocked while the auto-reload value is null.
	TIM2->ARR = 10 - 1;		// 10 000 / 10 = 1 000
	TIM2->CNT = 0;

	// These bits allow to select the information to be sent in master mode to slave timers for
	// synchronization (TRGO). The combination is as follows:
	// 000: Reset - the UG bit from the TIMx_EGR register is used as trigger output (TRGO). If the
	// reset is generated by the trigger input (slave mode controller configured in reset mode) then
	// the signal on TRGO is delayed compared to the actual reset.
	// 001: Enable - the Counter Enable signal CNT_EN is used as trigger output (TRGO). It is
	// useful to start several timers at the same time or to control a window in which a slave timer is
	// enable. The Counter Enable signal is generated by a logic OR between CEN control bit and
	// the trigger input when configured in gated mode. When the Counter Enable signal is
	// controlled by the trigger input, there is a delay on TRGO, except if the master/slave mode is
	// selected (see the MSM bit description in TIMx_SMCR register).
	// 010: Update - The update event is selected as trigger output (TRGO). For instance a master
	// timer can then be used as a prescaler for a slave timer.
	// 011: Compare Pulse - The trigger output send a positive pulse when the CC1IF flag is to be
	// set (even if it was already high), as soon as a capture or a compare match occurred.
	// (TRGO).
	// 100: Compare - OC1REF signal is used as trigger output (TRGO)
	// 101: Compare - OC2REF signal is used as trigger output (TRGO)
	// 110: Compare - OC3REF signal is used as trigger output (TRGO)
	// 111: Compare - OC4REF signal is used as trigger output (TRGO)
	SET_BIT(TIM2->CR2, TIM_CR2_MMS_1);
}


/*------------------------------------------------------*
 | Enable ADC DMA access on DMA 2, Stream 0, Channel 0
 *------------------------------------------------------*/
void init_dma_adc(void) {
	// Enable clock access to DMA
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2EN);

	// Disable DMA2 Stream 0
	CLEAR_BIT(DMA2_Stream0->CR, DMA_SxCR_EN);
    while(DMA2_Stream0->CR == DMA_SxCR_EN){;}

	// Clear all interrupt flags of Stream 0
	// CLEAR_BIT(DMA2->LIFCR, DMA_LIFCR_CTCIF0);
	// CLEAR_BIT(DMA2->LIFCR, DMA_LIFCR_CHTIF0);
	// CLEAR_BIT(DMA2->LIFCR, DMA_LIFCR_CTEIF0);
	// CLEAR_BIT(DMA2->LIFCR, DMA_LIFCR_CDMEIF0);
	// CLEAR_BIT(DMA2->LIFCR, DMA_LIFCR_CFEIF0);

	// Select DMA channel for Stream 0
	// (The ADC is on Channel 0 of Stream 0 or Channel 0 of Stream 4)
	//   000: channel 0 selected
	//   001: channel 1 selected
	//   010: channel 2 selected
	//   011: channel 3 selected
	//   100: channel 4 selected
	//   101: channel 5 selected
	//   110: channel 6 selected
	//   111: channel 7 selected
	CLEAR_BIT(DMA2_Stream0->CR, DMA_SxCR_CHSEL_0);
	CLEAR_BIT(DMA2_Stream0->CR, DMA_SxCR_CHSEL_1);
	CLEAR_BIT(DMA2_Stream0->CR, DMA_SxCR_CHSEL_2);

	// Set the data registers
	DMA2_Stream0->PAR = (uint32_t) (&(ADC1->DR));
	DMA2_Stream0->M0AR = (uint32_t) input_buffer_0;
	DMA2_Stream0->M1AR = (uint32_t) input_buffer_1;

	// Set number of transfers as the length of one array
	DMA2_Stream0->NDTR = BUFFER_LEN;

	// Set transfer direction to peripheral-to-memory
	//   00: Peripheral-to-memory
	//   01: Memory-to-peripheral
	//   10: Memory-to-memory
	//   11: reserved
	CLEAR_BIT(DMA2_Stream0->CR, DMA_SxCR_DIR_0);
	CLEAR_BIT(DMA2_Stream0->CR, DMA_SxCR_DIR_1);

	// Enable double buffer mode (also enables circular mode)
	SET_BIT(DMA2_Stream0->CR, DMA_SxCR_DBM);
	SET_BIT(DMA2_Stream0->CR, DMA_SxCR_CIRC);

	// Set the starting target of double buffer mode to Memory 0
	CLEAR_BIT(DMA2_Stream0->CR, DMA_SxCR_CT);

	// Enable memory increment mode
	SET_BIT(DMA2_Stream0->CR, DMA_SxCR_MINC);

	// Set peripheral and memory size to 16 bits
	SET_BIT(DMA2_Stream0->CR, DMA_SxCR_MSIZE_0);
	SET_BIT(DMA2_Stream0->CR, DMA_SxCR_PSIZE_0);

	// Enable DMA transfer and half-transfer complete interrupt
	SET_BIT(DMA2_Stream0->CR, DMA_SxCR_TCIE);
	SET_BIT(DMA2_Stream0->CR, DMA_SxCR_HTIE);

	// Enable direct mode (disables FIFO)
	// DMA2_Stream0->FCR = 0;

	// DMA Interrupt enable in NVIC
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}


/*---------------------------------------------------------------------------*
 | Set up the device to sample from PA1 in continuous mode, using DMA with
 | double buffering.
 *---------------------------------------------------------------------------*/
void init_sampling() {
	// Initialize FIR filter
	arm_fir_init_f32(
		&fir_filter,
		FIR_FILTER_LEN,
		(float32_t *) &fir_coeffs[0],
		fir_filter_state_buffer,
		BUFFER_LEN
	);

	set_pa0_analog();
	init_adc_pa0();
	init_dma_adc();
	init_timer();

	// Enable ADC module
	SET_BIT(ADC1->CR2, ADC_CR2_ADON);

	// Enable DMA2 Stream 0
	SET_BIT(DMA2_Stream0->CR, DMA_SxCR_EN);

	// Enable the timer
	SET_BIT(TIM2->CR1, TIM_CR1_CEN);
}


void half_transfer_complete_callback(void) {
	process_buffer(input_buffer_0, BUFFER_LEN, unfiltered_buffer, filtered_buffer);
	print_buffers(unfiltered_buffer, filtered_buffer, BUFFER_LEN);
}


void transfer_complete_callback(void) {
	process_buffer(input_buffer_1, BUFFER_LEN, unfiltered_buffer, filtered_buffer);
	print_buffers(unfiltered_buffer, filtered_buffer, BUFFER_LEN);
}


void DMA2_Stream0_IRQHandler(void) {
	// Check for half-transfer complete interrupt by reading status register
	if READ_BIT(DMA2->LISR, DMA_LISR_HTIF0) {
		// Clear flag by writing to control register
		SET_BIT(DMA2->LIFCR, DMA_LIFCR_CHTIF0);
		half_transfer_complete_callback();
	}

	// Check for transfer complete interrupt by reading status register
	if READ_BIT(DMA2->LISR, DMA_LISR_TCIF0) {
		// Clear flag by writing to control register
		SET_BIT(DMA2->LIFCR, DMA_LIFCR_CTCIF0);
		transfer_complete_callback();
	}
}
