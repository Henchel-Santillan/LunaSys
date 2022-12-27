#ifndef LUNASYS_OV7670_OV7670CONTROL_H
#define LUNASYS_OV7670_OV7670CONTROL_H

#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_dcmi.h"
#include "stm32f4xx_hal_i2c.h"

void OV7670_MCO1_Init(void);
void OV7670_DCMI_Init(DCMI_HandleTypeDef *hdcmi);
void OV7670_DMA_Init(DMA_HandleTypeDef *hdma);
void OV7670_SCCB_Init(I2C_HandleTypeDef *hi2c);

#endif // LUNASYS_OV7670_OV7670CONTROL_H
