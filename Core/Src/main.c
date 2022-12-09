/* USER CODE BEGIN Header */
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

DCMI_HandleTypeDef hdcmi;
DMA_HandleTypeDef hdma;
I2C_HandleTypeDef hi2c;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

static void ADC_Init();
static void DMA_Init();
static void DCMI_Init();
static void GPIO_Init();
static void I2C_Init();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */

  ADC_Init();
  DMA_Init();
  DCMI_Init();
  GPIO_Init();
  I2C_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

static void ADC_Init() {

}

static void DMA_Init() {

	// Enable DMA controller clocks
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();


	// Initialize DMA interrupts using the NVIC

}

static void DCMI_Init() {
	hdcmi.Instance = DCMI;
	hdcmi.Init.SynchroMode = DCMI_SYNCHRO_HARDWARE;			// Hardware (external) synchronization
	hdcmi.Init.PCKPolarity = DCMI_PCKPOLARITY_RISING;  		// Pixel clock polarity - sample on the rising edge
	hdcmi.Init.VSPolarity = DCMI_VSPOLARITY_HIGH;			// Configure DCMI VSYNC active level to be high, since valid frames on the OV7670 are VSYNC active low
	hdcmi.Init.HSPolarity = DCMI_HSPOLARITY_LOW;			// DCMI HSYNC is active level low, since lines are valid on HSYNC high on the OV7670
	hdcmi.Init.CaptureRate = DCMI_CR_ALL_FRAME;	    		// Frequency of frame capture. Here, all frames will be captured.
	hdcmi.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;		// 8-bit data width - 1 byte per PIXCLK
	hdcmi.Init.JPEGMode = DCMI_JPEG_DISABLE;				// Disable JPEG mode, since image compression is not needed for Luna

	if (HAL_DCMI_Init(&hdcmi) != HAL_OK) {
		Error_Handler();
	}
}

static void GPIO_Init() {
	// Enable the GPIO clocks: GPIO ports are P{X}{Y}, where X = { A, ..., H } and Y = { 1, ..., 15 }
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef gpio_init;

	// Pushbutton
	gpio_init.Pin = PUSHBUTTON_PIN;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(PUSHBUTTON_PORT, &gpio_init);

	// Potentiometer
	gpio_init.Pin = POTENTIOMETER_PIN;
	gpio_init.Mode = GPIO_MODE_ANALOG;
	gpio_init.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(POTENTIOMETER_PORT, &gpio_init);

	// LCD Data Pins
	gpio_init.Pin = LCD_D4_PIN | LCD_D5_PIN | LCD_D6_PIN | LCD_D7_PIN;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(LCD_D_PORT, &gpio_init);

	// LCD Control Pins
	gpio_init.Pin = LCD_RS_PIN | LCD_RW_PIN | LCD_E_PIN;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(LCD_CTRL_PORT, &gpio_init);

	// Set up the DCMI Data Pins

	// Port C
	gpio_init.Pin = DCMI_D0_PIN | DCMI_D1_PIN | DCMI_D2_PIN | DCMI_D3_PIN | DCMI_D4_PIN;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init.Alternate = GPIO_AF13_DCMI_D0 | GPIO_AF13_DCMI_D1 | GPIO_AF13_DCMI_D2 | GPIO_AF13_DCMI_D3 | GPIO_AF13_DCMI_D4;
	HAL_GPIO_Init(DCMI_D01234_PORT, &gpio_init);

	// Port B
	gpio_init.Pin = DCMI_D5_PIN | DCMI_D6_PIN | DCMI_D7_PIN;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init.Alternate = GPIO_AF13_D5 | GPIO_AF13_D6 | GPIO_AF13_D7;
	HAL_GPIO_Init(DCM_567_PORT, &gpio_init);

	// Set up DCMI HSYNC, PIXCLK, and VSYNC
	gpio_init.Pin = DCMI_HSYNC_PIN | DCMI_PIXCLK_PIN;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init.Alternate = GPIO_AF13_DCMI_HSYNC | GPIO_AF13_DCMI_PIXCLK;
	HAL_GPIO_Init(DCMI_HSYNC_PIXCLK_PORT, &gpio_init);

	gpio_init.Pin = DCMI_VSYNC_PIN;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init.Alternate = GPIO_AF13_DCMI_VSYNC;
	HAL_GPIO_Init(DCMI_VSYNC_PORT, &gpio_init);

	// Set up the I2C2 Pins
	gpio_init.Pin = I2C2_SCL_PIN;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init.Alternate = GPIO_AF4_I2C2_SCL;

	gpio_init.Pin = I2C2_SDA_PIN;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init.Alternate = GPIO_AF4_I2C2_SDA;
}

static void I2C_Init() {
	hi2c.Instance = I2C2;
	hi2c.Init.ClockSpeed = 100000;						// I2C clock frequency < 400kHz. Common frequencies: 100, 400, and 1000 kHz. Choose 100 kHz.
	hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;	// 7-bit mode is standard I2C mode, where 7-bit peripheral address is transferred in 1st byte after start condition
	hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;	    //
	hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; 	//
	hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;		//

	if (HAL_I2C_Init(&hi2c) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
