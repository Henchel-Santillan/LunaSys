#ifndef LUNASYS_INV_SERIAL_H
#define LUNASYS_INV_SERIAL_H

#include "stm32f4xx.h"
#include "lunasys_common.h"

#define RX_BUFFER_SIZE 16

HAL_StatusTypeDef LunaSys_Serial_Init(UART_HandleTypeDef *huart);
void LunaSys_Serial_MSP_Init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_rx, DMA_HandleTypeDef *hdma_tx);
void LunaSys_Serial_MSP_DeInit(UART_HandleTypeDef *huart);
void LunaSys_Serial_DMA_Init(DMA_HandleTypeDef *hdma_rx, DMA_HandleTypeDef *hdma_tx);

LunaSys_Error LunaSys_Serial_Transmit(uint8_t data);
LunaSys_Error LunaSys_Serial_Receive();

#endif // LUNASYS_INV_SERIAL_H
