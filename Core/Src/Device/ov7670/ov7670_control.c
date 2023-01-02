#include "ov7670_control.h"

/**
 * Assumes that HAL_RCC_MCOConfig(...) macro has been called. Called in main.
 */
void OV7670_MCO1_Init(void) {
	//PA8 - MCO1 (Alternate function)
	GPIO_InitTypeDef gpio_init_mco1 = {0};
	gpio_init_mco1.Pin = GPIO_PIN_8;
	gpio_init_mco1.Mode = GPIO_MODE_AF_PP;
	gpio_init_mco1.Pull = GPIO_NOPULL;
	gpio_init_mco1.Speed = GPIO_SPEED_FREQ_VERY_HIGH;	// 50 MHz to 200 MHz
	gpio_init_mco1.Alternate = GPIO_AF0_MCO;
	HAL_GPIO_Init(GPIOA, &gpio_init_mco1);
}

/**
 * Assumes that __HAL_RCC_GPIOA_CLK_ENABLE(), __HAL_RCC_GPIOB_CLK_ENABLE() and __HAL_RCC_GPIOC_CLK_ENABLE()
 * have been called. Called in main.
 */
void OV7670_DCMI_Init(DCMI_HandleTypeDef *hdcmi) {
	// Initialize DCMI
	HAL_DCMI_DeInit(hdcmi);
	hdcmi->Instance = DCMI;
	hdcmi->Init.SynchroMode = DCMI_SYNCHRO_HARDWARE;	// Hardware (external) synchronization
	hdcmi->Init.PCKPolarity = DCMI_PCKPOLARITY_RISING;	// Sample pixel clock on the rising edge
	hdcmi->Init.VSPolarity = DCMI_VSPOLARITY_HIGH;		// Valid frames on the OV7670 are VSYNC active low
	hdcmi->Init.HSPolarity = DCMI_HSPOLARITY_LOW;		// Lines are valid on HSYNC high
	hdcmi->Init.CaptureRate = DCMI_CR_ALL_FRAME;	    // Capture all frames
	hdcmi->Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;	// 8-bit data width = 1 byte per PIXCLK
	HAL_DCMI_Init(hdcmi);
}

/**
 * Called in HAL_DCMI_MspInit()
 */
void OV7670_DCMI_MSP_Init(DCMI_HandleTypeDef *hdcmi, DMA_HandleTypeDef *hdma) {
	if (hdcmi->Instance != DCMI)
		return;

	// Enable AHB2 DCMI Clock
	__HAL_RCC_DCMI_CLK_ENABLE();

	// Configure GPIOs with AF == DCMI
	GPIO_InitTypeDef gpio_init_dcmi = {0};

	// PA4 - DCMI_HSYNC, PA6 - DCMI_PIXCLK
	gpio_init_dcmi.Pin = GPIO_PIN_4 | GPIO_PIN_6;
	gpio_init_dcmi.Mode = GPIO_MODE_AF_PP;
	gpio_init_dcmi.Pull = GPIO_PULLUP;
	gpio_init_dcmi.Speed = GPIO_SPEED_FREQ_HIGH;	// 25 MHz to 100 MHz
	gpio_init_dcmi.Alternate = GPIO_AF13_DCMI;
	HAL_GPIO_Init(GPIOA, &gpio_init_dcmi);

	// PB5 - DCMI_D5, PB7 - DCMI_VSYNC, PB8 - DCMI_D6, PB9 - DCMI_D7
	gpio_init_dcmi.Pin = GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
	gpio_init_dcmi.Mode = GPIO_MODE_AF_PP;
	gpio_init_dcmi.Pull = GPIO_PULLUP;
	gpio_init_dcmi.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_dcmi.Alternate = GPIO_AF13_DCMI;
	HAL_GPIO_Init(GPIOB, &gpio_init_dcmi);

	// PC6 - DCMI_D0, PC7 - DCMI_D1, PC8 - DCMI_D2, PC9 - DCMI_D3, PC11 - DCMI_D4
	gpio_init_dcmi.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_11;
	gpio_init_dcmi.Mode = GPIO_MODE_AF_PP;
	gpio_init_dcmi.Pull = GPIO_PULLUP;
	gpio_init_dcmi.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_dcmi.Alternate = GPIO_AF13_DCMI;
	HAL_GPIO_Init(GPIOC, &gpio_init_dcmi);

	// Configure DMA2 Stream1 Channel1 to transfer data from DCMI DR register to internal SRAM
	HAL_DMA_DeInit(hdma);
	hdma->Instance = DMA2_Stream1;							// Set the instance to DMA2 Stream 1
	hdma->Init.Channel = DMA_CHANNEL_1;						// Set the channel to Channel 1
	hdma->Init.Direction = DMA_PERIPH_TO_MEMORY;			// Data transfer is from peripheral (DCMI) to memory
	hdma->Init.PeriphInc = DMA_PINC_DISABLE;				// Disable peripheral address register increment. PBURST mode is not possible, omit.
	hdma->Init.MemInc = DMA_MINC_ENABLE;					// Enable memory address register increment
	hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;	// Set the peripheral data width to be one word (32 bits)
	hdma->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;		// Set the memory data width to be one word (32 bits)
	hdma->Init.Mode = DMA_NORMAL;							// Set the operation mode of DMA2 Stream1 to normal (vs. circular)
	hdma->Init.Priority = DMA_PRIORITY_HIGH;				// High SW priority for DMA2 Stream1
	hdma->Init.FIFOMode = DMA_FIFOMODE_ENABLE;				// Enable FIFO mode
	hdma->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;		// Full threshold for DMA FIFO mode
	hdma->Init.MemBurst = DMA_MBURST_SINGLE;				// Single burst data transfer
	HAL_DMA_Init(hdma);

	// Link the DCMI to DMA2
	__HAL_LINKDMA(hdcmi, DMA_Handle, *hdma);

	// Enable DCMI interrupts via NVIC
	__HAL_DCMI_ENABLE_IT(hdcmi, DCMI_IT_FRAME);	// Frame capture complete interrupt mask

	/*__HAL_DCMI_ENABLE_IT(hdcmi, DCMI_IT_OVR);	// Overrun interrupt mask
	__HAL_DCMI_ENABLE_IT(hdcmi, DCMI_IT_ERR);	// Synchronization error interrupt mask*/

	HAL_NVIC_SetPriority(DCMI_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DCMI_IRQn);
}

/**
 * Called in HAL_DCMI_MspDeInit()
 */
void OV7670_DCMI_MSP_DeInit(DCMI_HandleTypeDef *hdcmi) {
	if (hdcmi->Instance != DCMI)
		return;

	// Disable DCMI clock
	__HAL_RCC_DCMI_CLK_DISABLE();


	// Deinitialize GPIOs
	// PA4, PA6
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4 | GPIO_PIN_6);

	// PB5, PB7, PB8, PB9
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9);

	// PC6, PC7, PC8, PC9, PC11
	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_11);

	// Deinitialize DMA
	HAL_DMA_DeInit(hdcmi->DMA_Handle);

	// Disable IRQ
	HAL_NVIC_DisableIRQ(DCMI_IRQn);
}

/**
 * Initializes the DMA peripheral clock and the DMA2 IRQs. Called in main.
 */
void OV7670_DMA_Init(DMA_HandleTypeDef *hdma) {
	// Enable DMA2 controller clock
	__HAL_RCC_DMA2_CLK_ENABLE();

	// Enable DMA interrupts
	__HAL_DMA_ENABLE_IT(hdma, DMA_IT_TC);
	__HAL_DMA_ENABLE_IT(hdma, DMA_IT_TE);

	// Configure DMA interrupt priority and enable the IRQ handler using NVIC
	HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 5 /* preempt priority */, 0 /* sub-priority */);
	HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
}

/**
 * Called in main
 */
void OV7670_SCCB_Init(I2C_HandleTypeDef *hi2c) {
	HAL_I2C_DeInit(hi2c);
	hi2c->Instance = I2C2;
	hi2c->Init.ClockSpeed = 100000;							// I2C clock frequency < 400 kHz. Common: 100, 400, 1000. Choose 100 kHz.
	hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;	// 7-bit peripheral address is transferred in first byte after start
	hi2c->Init.DutyCycle = I2C_DUTYCYCLE_2;					// Fast mode duty-cycle
	hi2c->Init.OwnAddress1 = 0;
	hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;	// No need to specify OwnAddress2
	hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(hi2c);
}

/**
 * Called in HAL_I2C_MspInit()
 */
void OV7670_SCCB_MSP_Init(I2C_HandleTypeDef *hi2c) {
	if (hi2c->Instance != I2C2)
		return;

	// Enable APB1 I2C2 Clock
	__HAL_RCC_I2C2_CLK_ENABLE();

	// Configure GPIOs with AF == I2C2
	GPIO_InitTypeDef gpio_init_i2c2 = {0};

	// PB10 - I2C2_SCL
	gpio_init_i2c2.Pin = GPIO_PIN_10;
	gpio_init_i2c2.Mode = GPIO_MODE_AF_OD;	// Use open-drain, since I2C is two-way, single-line
	gpio_init_i2c2.Pull = GPIO_PULLUP;
	gpio_init_i2c2.Speed = GPIO_SPEED_FREQ_LOW;	// 2 MHz
	gpio_init_i2c2.Alternate = GPIO_AF4_I2C2;
	HAL_GPIO_Init(GPIOB, &gpio_init_i2c2);

	// PC12 - I2C2_SDA
	gpio_init_i2c2.Pin = GPIO_PIN_12;
	gpio_init_i2c2.Mode = GPIO_MODE_AF_OD;
	gpio_init_i2c2.Pull = GPIO_PULLUP;
	gpio_init_i2c2.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_init_i2c2.Alternate = GPIO_AF4_I2C2;
	HAL_GPIO_Init(GPIOC, &gpio_init_i2c2);
}

/**
 * Called in HAL_I2C_MspDeInit()
 */
void OV7670_SCCB_MSP_DeInit(I2C_HandleTypeDef *hi2c) {
	if (hi2c->Instance != I2C2)
		return;

	// Disable I2C2 peripheral clock
	__HAL_RCC_I2C2_CLK_DISABLE();

	// Deinitialize GPIOs
	// PB10
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

	// PC12
	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);
}
