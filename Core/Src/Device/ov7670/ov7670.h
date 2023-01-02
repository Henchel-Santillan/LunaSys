#ifndef LUNASYS_OV7670_OV7670_H
#define LUNASYS_OV7670_OV7670_H

#include "stm32f4xx.h"

/**
 * SCCB flag status and check event timeouts
 */
typedef enum {

	SCCBE_OK = 0,
	SCCBE_BUSY = 0x1,
	SCCBE_ERROR = 0x2

} SCCB_Error;

/**
 * OV7670 Device Library status
 */

typedef enum {

	OV7670S_START_OK = 0,
	OV7670S_START_ERROR = 0x1,
	OV7670S_STOP_OK = 0x2,
	OV7670S_STOP_ERROR = 0x4

} OV7670_Status;

SCCB_Error Init_OV7670(DCMI_HandleTypeDef *hdcmi, I2C_HandleTypeDef *hi2c);
OV7670_Status Start_OV7670(uint32_t capture_mode, uint32_t dest_addr);
OV7670_Status Stop_OV7670();

#endif // LUNASYS_OV7670_OV7670_H
