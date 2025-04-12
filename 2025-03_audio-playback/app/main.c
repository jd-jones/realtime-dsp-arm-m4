/**
  ******************************************************************************
  * @file    main.c 
  * @author  jdjones
  * @brief   Main program body.
  ******************************************************************************
  * TODO
  ******************************************************************************
  */

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_cortex.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_rcc.h>
#include <stm32f4xx_ll_utils.h>


#define GREEN_LED_PIN       LL_GPIO_PIN_12
#define ORANGE_LED_PIN      LL_GPIO_PIN_13
#define RED_LED_PIN         LL_GPIO_PIN_14
#define BLUE_LED_PIN        LL_GPIO_PIN_15
#define LED_PORT    GPIOD


int main(int argc, char const* argv[]) {
    (void)argc;
    (void)argv;

    // init systick at 4 MHz
    LL_Init1msTick(4000000);
    LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

    // init led
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

    while (1) {
        LL_mDelay(500);
        LL_GPIO_TogglePin(
            LED_PORT,
            GREEN_LED_PIN | ORANGE_LED_PIN | RED_LED_PIN | BLUE_LED_PIN
        );
    }

    return 0;
}
