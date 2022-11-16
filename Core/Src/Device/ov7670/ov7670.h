#ifndef LUNASYS_OV7670_OV7670_H
#define LUNASYS_OV7670_OV7670_H


#include "stm32f4xx_hal.h"


HAL_StatusTypeDef Write_Once_OV7670(uint8_t imem_addr, uint8_t data);
HAL_StatusTypeDef Write_OV7670(uint8_t imem_addr, uint8_t data);
HAL_StatusTypeDef Read_Once_OV7670(uint8_t imem_addr, uint8_t data);
HAL_StatusTypeDef Read_OV7670(uint8_t imem_addr, uint8_t data);

HAL_StatusTypeDef Init_OV7670(DCMI_HandleTypeDef *p_handle_dcmi, DMA_HandleTypeDef *p_handle_dma, I2C_HandleTypeDef *p_handle_i2c);
HAL_StatusTypeDef Configure_OV7670(uint32_t mode);
HAL_StatusTypeDef Start_Capture_OV7670(uint32_t mode, uint32_t dest_addr);
HAL_StatusTypeDef Stop_Capture_OV7670();


#endif // LUNASYS_OV7670_OV7670_H
