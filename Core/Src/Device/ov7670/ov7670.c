#include "ov7670.h"
#include "ov7670Config.h"


/**** STATIC VARIABLES ****/

static DCMI_HandleTypeDef *sp_hdcmi;
static DMA_HandleTypeDef  *sp_hdma;
static I2C_HandleTypeDef  *sp_hi2c;


/**** STATIC FUNCTIONS ****/

static HAL_StatusTypeDef Write_Once_OV7670(uint8_t imem_addr, uint8_t data) {
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(sp_hi2c, DEVADDR_WRITE, imem_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
	return status;
}

static HAL_StatusTypeDef Write_OV7670(uint8_t imem_addr, uint8_t data) {
	HAL_StatusTypeDef status;

	do {
		status = HAL_I2C_Mem_Write(sp_hi2c, DEVADDR_WRITE, imem_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
	} while (status != HAL_OK);

	return status;
}

static HAL_StatusTypeDef Read_Once_OV7670(uint8_t imem_addr, uint8_t data) {
	// HAL_I2C_Mem_Read() does not work with SCCB
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(sp_hi2c, DEVADDR_WRITE, &imem_addr, 1, 100);
	status |= HAL_I2C_Master_Receive(sp_hi2c, DEVADDR_WRITE, &data, 1, 100);
	return status;
}

static HAL_StatusTypeDef Read_OV7670(uint8_t imem_addr, uint8_t data) {
	HAL_StatusTypeDef status;

	do {
		// HAL_I2C_Mem_Read() does not work with SCCB
		// Transmit in master (controller) mode an amount of data in blocking (polling) mode
		status = HAL_I2C_Master_Transmit(sp_hi2c, DEVADDR_WRITE, &imem_addr, 1, 100);

		// Receive in master (controller) mode an amount of data in blocking (polling) mode
		status |= HAL_I2C_Master_Receive(sp_hi2c, DEVADDR_WRITE, &data, 1, 100);

	} while (status != HAL_OK);

	return status;
}


/**** FUNCTIONS DEFINITIONS ****/

HAL_StatusTypeDef Init_OV7670(DCMI_HandleTypeDef *p_handle_dcmi, DMA_HandleTypeDef *p_handle_dma, I2C_HandleTypeDef *p_handle_i2c) {

	// Set the DCMI, DMA, and I2C handles
	sp_hdcmi = p_handle_dcmi;
	sp_hdma  = p_handle_dma;
	sp_hi2c  = p_handle_i2c;



	// Reset all internal device registers to default values: 0x12 = COM7, 0x80 = Bit 7 HIGH
	HAL_StatusTypeDef status = Write_OV7670(0x12, 0x80);
	HAL_Delay(30);

	return status;
}

HAL_StatusTypeDef Configure_OV7670(uint32_t mode) {
	// Stop capture to disable DCMI DMA request and DCMI capture
	HAL_StatusTypeDef status = Stop_Capture_OV7670();

	// Reset internal OV7670 device registers
	status |= Write_OV7670(0x12, 0x80);

	// See ov7670Config.h dev_reg_config
	for (int i = 0; dev_reg_config[i][0] != REG_RANGE; ++i) {
		status |= Write_OV7670(dev_reg_config[i][0], dev_reg_config[i][1]);
	}

	return status;
}

HAL_StatusTypeDef StartCapture_OV7670(uint32_t mode, uint32_t dest_addr) {
	if (mode != DCMI_MODE_CONTINUOUS || mode != DCMI_MODE_SNAPSHOT) {
		return HAL_ERROR;
	}

	HAL_StatusTypeDef status = Stop_Capture_OV7670();
	status |= HAL_DCMI_Start_DMA(sp_hdcmi, mode, dest_addr, OV7670_VGA_PPL * OV7670_VGA_LINES);

	return status;
}

HAL_StatusTypeDef Stop_Capture_OV7670() {
	return HAL_DCMI_Stop(sp_hdcmi);
}
