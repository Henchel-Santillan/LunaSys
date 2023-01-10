#include "lunasys_device.h"

/**
 * Camera Module
 */

LunaSys_Error Camera_Init(DCMI_HandleTypeDef *hdcmi, DMA_HandleTypeDef *hdma, I2C_HandleTypeDef *hi2c) {
	// Initialize the peripherals
	OV7670_MCO1_Init();
	OV7670_DCMI_Init(hdcmi);
	OV7670_DMA_Init(hdma);
	OV7670_SCCB_Init(hi2c);

	// Initialize the camera module
	SCCB_Error error = Init_OV7670(hdcmi, hi2c);
	return (error != SCCBE_OK) ? LUNASYSE_ERROR : LUNASYSE_OK;
}


/**
 * LCD
 */

LunaSys_Error LCD_Init_4B2L(LCD_TypeDef *p_lcd, Pin_TypeDef *p_rs, Pin_TypeDef *p_rw, Pin_TypeDef *p_e, Data_Pins_TypeDef *p_data_bus) {
	if (p_lcd == NULL || p_rs == NULL || p_rw == NULL || p_e == NULL || p_data_bus == NULL) {
		return LUNASYSE_ERROR;
	}

	// Initialize the peripherals
	LCD1602A_GPIO_Init();

	// Initialize the LCD device
	// LunaSys will use, by default, a 4-bit data bus and a 2-line configutation
	Init_LCD1602A(0 /* Any argument != 0x10 will work here */, LCD_MAX_LINES, p_lcd, p_rs, p_rw, p_e, p_data_bus);
	Home_Return_LCD1602A(p_lcd);

	return LUNASYSE_OK;
}

LunaSys_Error LCD_Write_CaptureMode(LCD_TypeDef *p_lcd, uint32_t capture_mode) {
	if (p_lcd == NULL || (capture_mode != DCMI_MODE_SNAPSHOT || capture_mode != DCMI_MODE_CONTINUOUS)) {
		return LUNASYSE_ERROR;
	}

	Display_Clear_LCD1602A(p_lcd);
	Home_Return_LCD1602A(p_lcd);

	if (capture_mode == DCMI_MODE_SNAPSHOT) {
		uint8_t mode[9] = "SNAPSHOT";
		Write_String_LCD1602A(p_lcd, mode, 9);
	} else {
		// DCMI_MODE_CONTINUOUS
		uint8_t mode[11] = "CONTINUOUS";
		Write_String_LCD1602A(p_lcd, mode, 11);
	}

	return LUNASYSE_OK;
}

/**
 * UART
 */

LunaSys_Error Serial_Init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_rx, DMA_HandleTypeDef *hdma_tx) {
	if (huart == NULL) {
		return LUNASYSE_ERROR;
	}

	HAL_StatusTypeDef status = LunaSys_Serial_Init(huart);
	if (status == HAL_OK) {
		LunaSys_Serial_DMA_Init(hdma_rx, hdma_tx);
	}

	return (status != HAL_OK) ? LUNASYSE_ERROR : LUNASYSE_OK;
}
