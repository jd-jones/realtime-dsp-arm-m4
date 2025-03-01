/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


#include "stm32f4xx.h"
#include "gpio.h"

#define TOGGLE_BIT(REG, BIT)        ((REG) ^= (BIT))
#define MCU_CLOCK_FREQUENCY         16000000  // 16 MHz


void setup(void) {
    setup_leds();
    setup_user_pushbutton();
    setup_user_pushbutton_interrupt();
}


int main(void)
{
    setup();

    while (1) {
        // (No loop code required)
    }
}


void toggle_leds(void) {
    TOGGLE_BIT(GPIOD->ODR, GPIO_ODR_OD12);
    TOGGLE_BIT(GPIOD->ODR, GPIO_ODR_OD13);
    TOGGLE_BIT(GPIOD->ODR, GPIO_ODR_OD14);
    TOGGLE_BIT(GPIOD->ODR, GPIO_ODR_OD15);
}


void EXTI0_IRQHandler(void) {
    /* --[NOTES]---------------------------------------------------------------
     * STM32 Reference Manual 0090 Section 12.3.6:
     * ===========================================
     * PRx: Pending bit
     *   0: No trigger request occurred
     *   1: selected trigger request occurred
     *   This bit is set when the selected edge event arrives on the external
     *   interrupt line. This bit is cleared by programming it to ‘1’.
     *------------------------------------------------------------------------*/
    if (READ_BIT(EXTI->PR, EXTI_PR_PR0) != 0) {
        SET_BIT(EXTI->PR, EXTI_PR_PR0);
        toggle_leds();
    }
}


/*==[ NOTES ]==============================================
 *
 *--[ GPIO: LED ]-----------------------------------------------
 * LD1 COM: LD1 default status is red. LD1 turns to green to indicate that
 *   communications are in progress between the PC and the ST-LINK/V2-A.
 * LD2 PWR: red LED indicates that the board is powered.  User LD3: orange LED
 *   is a user LED connected to the I/O PD13 of the STM32F407VGT6.
 * User LD4: green LED is a user LED connected to the I/O PD12 of the STM32F407VGT6.
 * User LD5: red LED is a user LED connected to the I/O PD14 of the STM32F407VGT6.
 * User LD6: blue LED is a user LED connected to the I/O PD15 of the STM32F407VGT6.
 * USB LD7: green LED indicates when VBUS is present on CN5 and is connected to
 *   PA9 of the STM32F407VGT6.
 * USB LD8: red LED indicates an over-current from VBUS of CN5 and is connected
 *   to the I/O PD5 of the STM32F407VGT6.
 *
 *--[ GPIO: Pushbuttons ]-----------------------------------------------
 * B1 USER: User and Wake-Up buttons are connected to the I/O PA0 of the
 *   STM32F407VG.
 * B2 RESET: Push button connected to NRST is used to RESET the STM32F407VG.
 *
 *--[ Timer and clock frequencies ]------------------------
 * On reset the 16 MHz internal RC oscillator is selected as the default CPU clock
 *   --> The default clock frequency is 16 MHz.
 *
 *=========================================================
 */
