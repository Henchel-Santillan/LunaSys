#ifndef LUNASYS_DEVICE_LUNASYSDEVICE_H
#define LUNASYS_DEVICE_LUNASYSDEVICE_H

#include "b1user_control.h"
#include "lcd1602a_control.h"
#include "lcd1602a.h"
#include "ov7670_control.h"
#include "ov7670.h"

/**
 * Error status for Lunasys functions
 */
typedef enum {

	LUNASYSE_OK = 0,
	LUNASYSE_ERROR = 0x1

} LunaSys_Error;

/**
 * Camera Module
 */
LunaSys_Error Camera_Init(DCMI_HandleTypeDef *hdcmi, DMA_HandleTypeDef *hdma, I2C_HandleTypeDef *hi2c);

/**
 * LCD
 */
LunaSys_Error LCD_Init_4B2L(LCD_TypeDef *p_lcd, Pin_TypeDef *p_rs, Pin_TypeDef *p_rw, Pin_TypeDef *p_e, Data_Pins_TypeDef *p_data_bus);
LunaSys_Error LCD_Write_CaptureMode(LCD_TypeDef *p_lcd, uint32_t capture_mode);


#endif // LUNASYS_DEVICE_LUNASYSDEVICE_H
