#include "stm32f4xx.h"
#include "gpio.h"


void setup_leds(void) {
    /*--[NOTES]-----------------------------------------------------------------
     * PD12: Green LED
     * PD13: Orange LED
     * PD14: Red LED
     * PD15: Blue LED
     *------------------------------------------------------------------------*/

    // Enable GPIO D
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);

    /*==============================================================
     * Set PD12, PD13, PD14, PD15 to output mode.
     *   00: Input (reset state)
     *   01: General purpose output mode
     *   10: Alternate function mode
     *   11: Analog mode                                          */
    //--[Pin 12 (Green LED)]------------------------------------------
    CLEAR_BIT(GPIOD->MODER, GPIO_MODER_MODER12_1);
    SET_BIT(GPIOD->MODER, GPIO_MODER_MODER12_0);
    //--[Pin 13 (Orange LED)]------------------------------------------
    CLEAR_BIT(GPIOD->MODER, GPIO_MODER_MODER13_1);
    SET_BIT(GPIOD->MODER, GPIO_MODER_MODER13_0);
    //--[Pin 14 (Red LED)]---------------------------------------------
    CLEAR_BIT(GPIOD->MODER, GPIO_MODER_MODER14_1);
    SET_BIT(GPIOD->MODER, GPIO_MODER_MODER14_0);
    //--[Pin 15 (Blue LED)]--------------------------------------------
    CLEAR_BIT(GPIOD->MODER, GPIO_MODER_MODER15_1);
    SET_BIT(GPIOD->MODER, GPIO_MODER_MODER15_0);
    //==================================================================
}


void setup_user_pushbutton(void) {
    /*--[NOTES]-----------------------------------------------------------------
     * PA00: User and wake-up button(s)
     *------------------------------------------------------------------------*/

     // Enable GPIO A
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);

    /* Set PA0 to input mode: No change necessary, this is the reset state.
     *   00: Input (reset state)
     *   01: General purpose output mode
     *   10: Alternate function mode
     *   11: Analog mode
     */
}


void setup_user_pushbutton_interrupt(void) {
    /*--[NOTES]-----------------------------------------------------------------
     * PA00: User and wake-up button(s)
     *
     * STM32 reference manual 0090 Section 12.2.4:
     * ===========================================
     * To configure the 23 lines as interrupt sources, use the following procedure:
     *   - Configure the mask bits of the 23 interrupt lines (EXTI_IMR)
     *   - Configure the Trigger selection bits of the interrupt lines
     *     (EXTI_RTSR and EXTI_FTSR)
     *   - Configure the enable and mask bits that control the NVIC IRQ channel
     *     mapped to the external interrupt controller (EXTI) so that an
     *     interrupt coming from one of the 23 lines can be correctly acknowledged.
     *------------------------------------------------------------------------*/

     __disable_irq();

     //--[Set port A (PA0) as input to EXTI0]----------------------------------
     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
     SET_BIT(SYSCFG->EXTICR[1], SYSCFG_EXTICR1_EXTI0_PA);
     // (Does SYSCFG need clock access after this point?)
     //------------------------------------------------------------------------

     // Select falling edge trigger for EXTI0
     // (We don't want to interrupt on rising edges. But EXTI->RTSR does not
     // need to be configured because its reset status is disabled)
     SET_BIT(EXTI->FTSR, EXTI_FTSR_TR0);

     //--[Enable EXTI0]--------------------------------------------------------
     SET_BIT(EXTI->IMR, EXTI_IMR_MR0);
     NVIC_EnableIRQ(EXTI0_IRQn);
     //------------------------------------------------------------------------

    __enable_irq();
}
