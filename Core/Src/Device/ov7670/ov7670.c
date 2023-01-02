#include "ov7670.h"

#include "ov7670_config.h"


/**
 * HAL Handles: DCMI, DMA, I2C (for SCCB)
 */
static DCMI_HandleTypeDef *p_hdcmi;
static I2C_HandleTypeDef *p_hi2c;

/**
 * States
 */
static uint32_t mcont_dest_addr;	// Destination address when in continuous mode


/**
 *	Internal SCCB API
 */

static SCCB_Error SCCB_Write(uint8_t reg, uint8_t data) {
	uint32_t busy_timeout = 0x7FFFFF;

	// Check for busy timeout
	while (__HAL_I2C_GET_FLAG(p_hi2c, I2C_FLAG_BUSY)) {
		if ((busy_timeout--) == 0) {
			return SCCBE_BUSY;
		}
	}

	uint32_t write_timeout = 100;

	// Attempt to write to OV7670 in blocking mode
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(p_hi2c, DEVADDR_WRITE, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, write_timeout);
	if (status != HAL_OK) {
		return SCCBE_ERROR;
	}

	return SCCBE_OK;
}


/**
 * OV7670 user-facing Device Library API
 */

/**
 * Initializes the OV7670 by performing a reset and then writing the configurations to the
 * module as defined in ov7670_config.h via SCCB.
 */
SCCB_Error Init_OV7670(DCMI_HandleTypeDef *hdcmi, I2C_HandleTypeDef *hi2c) {
	p_hdcmi = hdcmi;
	p_hi2c = hi2c;

	mcont_dest_addr = 0;

	// Full HW reset on the OV7670
	SCCB_Error sccb_err = SCCB_Write(0x12, 0x80);
	if (sccb_err == SCCBE_OK) {
		// Configure the OV7670 if reset successful
		for (int i = 0; dev_reg_config[i][0] != REG_RANGE; i++) {
			if (SCCB_Write(dev_reg_config[i][0], dev_reg_config[i][1]) != SCCBE_OK) {
				break;
			}
			HAL_Delay(1);
		}
	}

	return sccb_err;
}

/**
 * Start DCMI DMA request and DCMI capture. Caller is responsible for ensuring that
 * the capture_mode is valid and the dest_addr is valid and exists
 */
OV7670_Status Start_OV7670(uint32_t capture_mode, uint32_t dest_addr) {
	if (capture_mode == DCMI_MODE_CONTINUOUS) {
		mcont_dest_addr = dest_addr;
	}

	uint32_t length = (QVGA_PPL * QVGA_LINES) / 2;
	HAL_StatusTypeDef status = HAL_DCMI_Start_DMA(p_hdcmi, capture_mode, dest_addr, length);
	return (status == HAL_OK) ? OV7670S_START_OK : OV7670S_START_ERROR;
}

/**
 * Disable DCMI DMA request and DCMI capture
 */
OV7670_Status Stop_OV7670() {
	HAL_StatusTypeDef status = HAL_DCMI_Stop(p_hdcmi);
	return (status == HAL_OK) ? OV7670S_STOP_OK : OV7670S_STOP_ERROR;
}
