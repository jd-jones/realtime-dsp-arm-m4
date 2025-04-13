/**
  ******************************************************************************
  * @file    main.c 
  * @author  jdjones
  * @brief   Main program body.
  ******************************************************************************
  * TODO
  ******************************************************************************
  */

#include <stm32f4xx_ll_bus.h>       // LL_AHB1_GRP1_EnableClock
#include <stm32f4xx_ll_cortex.h>    // LL_SYSTICK_SetClkSource
#include <stm32f4xx_ll_gpio.h>      // GPIO functions & definitions
#include <stm32f4xx_ll_utils.h>     // LL_Init1msTick, LL_mDelay


#define GREEN_LED_PIN       LL_GPIO_PIN_12
#define ORANGE_LED_PIN      LL_GPIO_PIN_13
#define RED_LED_PIN         LL_GPIO_PIN_14
#define BLUE_LED_PIN        LL_GPIO_PIN_15
#define LED_PORT            GPIOD
#define FOUR_MHz            4000000


int main(int argc, char const* argv[]) {
    (void)argc;
    (void)argv;

    // == INITIALIZE SYSTEM CLOCK TO 4 MHZ ====================================
    LL_Init1msTick(FOUR_MHz);
    LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
    // ========================================================================

    // == INITIALIZE LED PINS =================================================
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
    LL_GPIO_SetPinMode(LED_PORT, GREEN_LED_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LED_PORT, ORANGE_LED_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LED_PORT, RED_LED_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LED_PORT, BLUE_LED_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(
        LED_PORT,
        GREEN_LED_PIN | ORANGE_LED_PIN | RED_LED_PIN | BLUE_LED_PIN,
        LL_GPIO_OUTPUT_PUSHPULL
    );
    // ========================================================================

    while (1) {
        // == BLINK LEDS BY TOGGLING EVERY 500 MS =============================
        LL_mDelay(500);
        LL_GPIO_TogglePin(
            LED_PORT,
            GREEN_LED_PIN | ORANGE_LED_PIN | RED_LED_PIN | BLUE_LED_PIN
        );
        // ====================================================================
    }

    return 0;
}
