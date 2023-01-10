/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f4xx_it.h"

extern DCMI_HandleTypeDef hdcmi;
extern DMA_HandleTypeDef hdma;
extern DMA_HandleTypeDef hdma_uart_rx;
extern DMA_HandleTypeDef hdma_uart_tx;
extern UART_HandleTypeDef huart;

/**
  * Handle system tick timer
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

void DCMI_IRQHandler(void) {
	HAL_DCMI_IRQHandler(&hdcmi);
}

void DMA2_Stream1_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma);	// DCMI
}

void DMA1_Stream5_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_uart_rx);
}

void USART2_IRQHandler(void) {
	HAL_UART_IRQHandler(&huart);
}

void EXTI9_5_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);	// B1_USER, capture start/stop button
}

void EXTI15_10_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);	// Mode switch button
}
