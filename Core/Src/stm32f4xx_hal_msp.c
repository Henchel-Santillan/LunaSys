/**
  ******************************************************************************
  * @file         stm32f4xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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
#include "ov7670_control.h"
#include "serial.h"

extern DMA_HandleTypeDef hdma;
extern DMA_HandleTypeDef hdma_uart_rx;
extern DMA_HandleTypeDef hdma_uart_tx;

/**
  * Initializes the Global MSP
  */
void HAL_MspInit(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();

	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

	// Set interrupt priorities for Cortex-M4 Processor -related components
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
	HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
	HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
	HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/**
 * Peripheral MSPs
 */

void HAL_DCMI_MspInit(DCMI_HandleTypeDef *hdcmi) {
	OV7670_DCMI_MSP_Init(hdcmi, &hdma);
}

void HAL_DCMI_MspDeInit(DCMI_HandleTypeDef *hdcmi) {
	OV7670_DCMI_MSP_DeInit(hdcmi);
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c) {
	OV7670_SCCB_MSP_Init(hi2c);
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c) {
	OV7670_SCCB_MSP_DeInit(hi2c);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
	LunaSys_Serial_MSP_Init(huart, &hdma_uart_rx, &hdma_uart_tx);
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart) {
	LunaSys_Serial_MSP_DeInit(huart);
}
