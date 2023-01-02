#include "b1user_control.h"

#include "stm32f4xx.h"

/**
 * Assumes that __HAL_RCC_GPIOC_CLK_ENABLE() has been called. Initializes both the onboard pushbutton on
 * the STM32F446RE and an external pushbutton used to toggle between the two available OV7670 capture modes.
 * Is called in main.
 */
void Pushbutton_GPIO_Init(void) {
	GPIO_InitTypeDef gpio_init_b1user = {0};

	// PC13 - TAMP_1 / WKUP1, USER B1 Button. PC5 - pushbutton for mode switch.
	// Configure both pushbuttons in EXTI mode, with the trigger being the rising edge (LOW to HIGH)
	gpio_init_b1user.Pin = GPIO_PIN_5 | GPIO_PIN_13;
	gpio_init_b1user.Mode = GPIO_MODE_IT_RISING;
	gpio_init_b1user.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &gpio_init_b1user);
}
