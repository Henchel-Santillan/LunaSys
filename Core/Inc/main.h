/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* GPIO Ports */

// TODO FIXME: Remove this section and just specify the GPIOx value. Only define pins.

// DCMI and OV7670
#define DCMI_HSYNC_PIXCLK_PORT GPIOA
#define DCMI_MCO_PORT GPIOA

#define DCMI_D01234_PORT GPIOC
#define DCMI_D567_PORT GPIOB
#define DCMI_VSYNC_PORT GPIOB


#define I2C2_SCL_PORT GPIOB
#define I2C2_SDA_PORT GPIOC

// LCD1602A
#define LCD_D_PORT GPIOC	// D4, D5, D6, D7
#define LCD_CTRL_PORT GPIOB	// RS, RW, E

// USER Onboard Pushbutton and Potentiometer
#define PUSHBUTTON_PORT GPIOC
#define POTENTIOMETER_PORT GPIOA


/* GPIO Pins */

// DCMI and OV7670
#define DCMI_HSYNC_PIN GPIO_PIN_4
#define DCMI_PIXCLK_PIN GPIO_PIN_6
#define DCMI_MCO_PIN GPIO_PIN_8

#define DCMI_D0_PIN GPIO_PIN_6
#define DCMI_D1_PIN GPIO_PIN_7
#define DCMI_D2_PIN GPIO_PIN_8
#define DCMI_D3_PIN GPIO_PIN_9
#define DCMI_D4_PIN GPIO_PIN_11
#define DCMI_D5_PIN GPIO_PIN_5
#define DCMI_D6_PIN GPIO_PIN_6
#define DCMI_D7_PIN GPIO_PIN_7
#define DCMI_VSYNC_PIN GPIO_PIN_7

#define I2C2_SCL_PIN GPIO_PIN_10
#define I2C2_SDA_PIN GPIO_PIN_12

// LCD1602A
#define LCD_D4_PIN GPIO_PIN_0
#define LCD_D5_PIN GPIO_PIN_1
#define LCD_D6_PIN GPIO_PIN_2
#define LCD_D7_PIN GPIO_PIN_3
#define LCD_RS_PIN GPIO_PIN_12
#define LCD_RW_PIN GPIO_PIN_13
#define LCD_E_PIN GPIO_PIN_14

// USER Onboard Pushbutton and Potentiometer
#define PUSHBUTTON_PIN GPIO_PIN_13
#define POTENTIOMETER_PIN GPIO_PIN_1

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
