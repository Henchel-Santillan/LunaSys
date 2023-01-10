/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
 */

#include "stm32f4xx.h"
#include "lunasys_device.h"

/**
 * HAL Peripheral Handles
 */
DCMI_HandleTypeDef hdcmi;
DMA_HandleTypeDef hdma;
DMA_HandleTypeDef hdma_uart_rx;
DMA_HandleTypeDef hdma_uart_tx;

UART_HandleTypeDef huart;

I2C_HandleTypeDef hi2c;

/**
 * LCD Pin_, Data_Pins_, LCD_TypeDef
 */

Pin_TypeDef pin_rs = { .GPIO_PERIPHERAL = GPIOB, .GPIO_PIN = GPIO_PIN_12 },	// PB12
			pin_rw = { .GPIO_PERIPHERAL = GPIOB, .GPIO_PIN = GPIO_PIN_13 },	// PB13
			pin_e  = { .GPIO_PERIPHERAL = GPIOB, .GPIO_PIN = GPIO_PIN_14 }, // PB14
			pin_d4 = { .GPIO_PERIPHERAL = GPIOC, .GPIO_PIN = GPIO_PIN_0 },	//PC0
			pin_d5 = { .GPIO_PERIPHERAL = GPIOC, .GPIO_PIN = GPIO_PIN_1 },	// PC1
			pin_d6 = { .GPIO_PERIPHERAL = GPIOC, .GPIO_PIN = GPIO_PIN_2 },	// PC2
			pin_d7 = { .GPIO_PERIPHERAL = GPIOC, .GPIO_PIN = GPIO_PIN_3 };	// PC3


void SystemClock_Config(void);


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  // Reset all peripherals, initialize the Flash interface and the Systick. Also calls HAL_MspInit() to init LL HW.
  HAL_Init();

  // Configure the system clock
  SystemClock_Config();

  // Enable GPIO{A, B, C} clocks
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  // Initialize the LCD Device Library pin structures. Note that this is separate from initializing GPIO.
  Data_Pins_TypeDef data_pins = { .D0 = NULL, .D1 = NULL, .D2 = NULL, .D3 = NULL,
  	  	  	  	  	  	  	  	  .D4 = &pin_d4, .D5 = &pin_d5, .D6 = &pin_d6, .D7 = &pin_d7};
  LCD_TypeDef lcd_pins = {0};

  // Initialize devices and peripherals using lunasys_device API
  Camera_Init(&hdcmi, &hdma, &hi2c);
  LCD_Init_4B2L(&lcd_pins, &pin_rs, &pin_rw, &pin_e, &data_pins);
  Pushbutton_GPIO_Init();
  Serial_Init(&huart, &hdma_uart_rx, &hdma_uart_tx);

  // Infinite loop
  while (1) {}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	*/
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;

	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

	/** Configure the MCO1 Clock. Specify the output direction as MCO1,
	* the clock source to output as the main PLL clock,
	* and the prescaler to be division by 4. Using RCC_MCO1SOURCE_PLLCLK and RCC_MCODIV_4 results in garbage output.
	*/

	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_4);
}


/**
 * INTERRUPT (IT) CALLBACKS
 */

/**
 * External Interrupt callback for DCMI
 */

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi) {

}

/**
 * External Interrupt callback for UART
 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {

}

/**
 * EXTI GPIO Callback
 */

/**
 * External Interrupt callback for pushbuttons
 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

}
