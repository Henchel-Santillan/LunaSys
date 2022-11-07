#include "ov7670.h"
#include "ov7670Config.h"
#include "Inc/lunadef.h"

#include "stm32f4xx_hal.h"


static DCMI_HandleTypeDef *sp_hddcmi;
static DMA_HandleTypeDef  *sp_hddma;
static I2C_HandleTypeDef  *sp_hdi2c;

static uint32_t dest_addr_continuous;



Ops_StatusTypeDef Write_OV7670(uint8_t addr, uint8_t payload) {
	HAL_StatusTypeDef status;

	do {
		status = HAL_I2C_Mem_Write(sp_hdi2c, 0x42 /* TODO: Replace this slave address*/, addr, I2C_MEMADD_SIZE_8BIT, &payload, 1, 100);
	} while (status != HAL_OK);

	return OPS_OK;
}

Ops_StatusTypeDef Write_OV7670(uint8_t addr, uint8_t payload) {
	HAL_StatusTypeDef status;

	do {
		status =  HAL_I2C_Master_Transmit(sp_hdi2c, 0x42 /*TODO: Replace this slave address*/, &addr, 1, 100);
		status |= HAL_I2C_Master_Receive(sp_hdi2c, 0x42 /*TODO: Replace this slave address*/, payload, 1, 100);
	} while (status != HAL_OK);

	return OPS_OK;
}

Ops_StatusTypeDef Init_OV7670(DCMI_HandleTypeDef *p_handle_dcmi, DMA_HandleTypeDef *p_handle_dma, I2C_HandleTypeDef *p_handle_i2c) {

	// Set the DCMI, DMA, and I2C handles
	sp_hddcmi = p_handle_dcmi;
	sp_hddma  = p_handle_dma;
	sp_hdi2c  = p_handle_i2c;

	dest_addr_continuous = 0;


}


Ops_StatusTypeDef ModeConfig_OV7670(const uint32_t mode) {

}

Ops_StatusTypeDef StartCapture_OV7670(const uint32_t mode, uint32_t dest_addr) {
	if (mode != MODE_CONTINUOUS_GRAB || mode != MODE_SNAPSHOT) {
		return OPS_FAIL;
	}

	dest_addr_continuous = (mode == MODE_CONTINUOUS_GRAB) ? dest_addr : 0;
	HAL_StatusTypeDef start_status = HAL_DCMI_Start_DMA(sp_hddcmi, mode, dest_addr, (OV7670_QVGA_WIDTH * OV7670_QVGA_HEIGHT) / 2);

	return (start_status == HAL_OK) ? OPS_OK : OPS_FAIL;
}


Ops_StatusTypeDef StopCapture_OV7670() {
	HAL_StatusTypeDef stop_status = HAL_DCMI_Stop();
	return (stop_status == HAL_OK) ? OPS_OK : OPS_FAIL;
}



