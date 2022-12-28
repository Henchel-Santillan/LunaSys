#include "b1user_control.h"

#include "stm32f4xx_hal_gpio.h"

/**
 * Assumes that __HAL_RCC_GPIOC_CLK_ENABLE() has been called
 */
void B1USER_GPIO_Init(void) {
	GPIO_InitTypeDef gpio_init_b1user;

	// PC13 - TAMP_1 / WKUP1, USER B1 Button
	gpio_init_b1user.Pin = GPIO_PIN_13;
	gpio_init_b1user.Mode = GPIO_MODE_INPUT;
	gpio_init_b1user.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &gpio_init_b1user);
}

