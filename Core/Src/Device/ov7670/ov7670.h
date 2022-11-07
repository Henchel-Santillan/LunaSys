#ifndef LUNASYS_OV7670_OV7670_H
#define LUNASYS_OV7670_OV7670_H


Ops_StatusTypeDef Write_OV7670(uint8_t addr, uint8_t payload);
Ops_StatusTypeDef Read_OV7670(uint8_t addr, uint8_t payload);

Ops_StatusTypeDef Init_OV7670(DCMI_HandleTypeDef *p_handle_dcmi, DMA_HandleTypeDef *p_handle_dma, I2C_HandleTypeDef *p_handle_i2c);
Ops_StatusTypeDef ModeConfig_OV7670(const uint32_t mode);
Ops_StatusTypeDef StartCapture_OV7670(const uint32_t mode, uint32_t dest_addr);
Ops_StatusTypeDef StopCapture_OV7670();

// TODO: Implement a callback function once you learn more about the HW arch



#endif // LUNASYS_OV7670_OV7670_H
