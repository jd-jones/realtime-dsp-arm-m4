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
#define FIVE_HUNDRED_MS     500


void initialize_system_clock(uint32_t);
void initialize_leds(void);
void blink_leds(uint32_t);


int main(int argc, char const* argv[]) {
    (void)argc;
    (void)argv;

    initialize_system_clock(FOUR_MHz);
    initialize_leds();

    while (1) {
        blink_leds(FIVE_HUNDRED_MS);
    }

    return 0;
}


void initialize_system_clock(uint32_t clock_speed_mhz) {
    LL_Init1msTick(FOUR_MHz);
    LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
}


void initialize_leds(void) {
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
}


void blink_leds(uint32_t delay_ms) {
    LL_mDelay(delay_ms);
    LL_GPIO_TogglePin(
        LED_PORT,
        GREEN_LED_PIN | ORANGE_LED_PIN | RED_LED_PIN | BLUE_LED_PIN
    );
}
