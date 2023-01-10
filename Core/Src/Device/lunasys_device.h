#ifndef LUNASYS_DEVICE_LUNASYSDEVICE_H
#define LUNASYS_DEVICE_LUNASYSDEVICE_H

#include "b1user_control.h"
#include "lcd1602a_control.h"
#include "lcd1602a.h"
#include "ov7670_control.h"
#include "ov7670.h"
#include "serial.h"

/**
 * Camera Module
 */
LunaSys_Error Camera_Init(DCMI_HandleTypeDef *hdcmi, DMA_HandleTypeDef *hdma, I2C_HandleTypeDef *hi2c);

/**
 * LCD
 */
LunaSys_Error LCD_Init_4B2L(LCD_TypeDef *p_lcd, Pin_TypeDef *p_rs, Pin_TypeDef *p_rw, Pin_TypeDef *p_e, Data_Pins_TypeDef *p_data_bus);
LunaSys_Error LCD_Write_CaptureMode(LCD_TypeDef *p_lcd, uint32_t capture_mode);
LunaSys_Error Serial_Init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_rx, DMA_HandleTypeDef *hdma_tx);


#endif // LUNASYS_DEVICE_LUNASYSDEVICE_H
