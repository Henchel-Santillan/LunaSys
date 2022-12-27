#include "lcd1602a_control.h"

#include "stm32f4xx_hal_gpio.h"

/**
 * Assumes that __HAL_RCC_GPIOB_CLK_ENABLE() and __HAL_RCC_GPIOC_CLK_ENABLE()
 * have been called.
 */
void LCD1602A_GPIO_Init(void) {
	GPIO_InitTypeDef gpio_init_lcd;

	// LCD Data Pins: 4-bit data bus (PC0 - D4, PC1 - D5, PC2 - D6, PC3 - D7)
	gpio_init_lcd.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	gpio_init_lcd.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_lcd.Pull = GPIO_PULLDOWN;
	gpio_init_lcd.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(LCD_DATA_PORT, &gpio_init_lcd);

	// LCD Control Pins: PB12 - RS, PB13 - RW, and PB14 - E
	gpio_init_lcd.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	gpio_init_lcd.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_lcd.Pull = GPIO_PULLDOWN;
	gpio_init_lcd.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(LCD_CTRL_PORT, &gpio_init_lcd);
}
