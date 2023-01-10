#include "serial.h"


/**
 * Variables
 */

static UART_HandleTypeDef *p_huart;
static volatile uint8_t rxBuffer[RX_BUFFER_SIZE];
static volatile uint8_t rxRp = 0;

#define rxWp (( RX_BUFFER_SIZE - p_huart->hdmarx->Instance->NDTR ) & ( RX_BUFFER_SIZE - 1 ))

/**
 * Initializes UART communication parameters. Called in main.
 */
HAL_StatusTypeDef LunaSys_Serial_Init(UART_HandleTypeDef *huart) {
	HAL_UART_DeInit(huart);
	huart->Instance = USART2;
	huart->Init.BaudRate = 115200;
	huart->Init.WordLength = UART_WORDLENGTH_8B;
	huart->Init.StopBits = UART_STOPBITS_1;
	huart->Init.Parity = UART_PARITY_NONE;
	huart->Init.Mode = UART_MODE_TX_RX;		// Both transmitter and receiver
	huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_StatusTypeDef status = HAL_UART_Init(huart);	// Calls HAL_UART_MspInit()

	p_huart = huart;
	return status;
}

/**
 * Assumes __HAL_RCC_GPIOA_CLK_ENABLE() has been called. Note that the DMA_HandleTypeDef passed here is different from the one used for
 * the DCMI. Called in HAL_UART_MspInit()
 */
void LunaSys_Serial_MSP_Init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_rx, DMA_HandleTypeDef *hdma_tx) {
	if (huart->Instance != USART2)
		return;

	// Enable the APB1 USART2 Clock
	__HAL_RCC_USART2_CLK_ENABLE();

	// Configure UART GPIO with alternate function
	// PA2 - USART2_TX, PA3 - USART2_RX
	GPIO_InitTypeDef gpio_init_uart = {0};
	gpio_init_uart.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	gpio_init_uart.Mode = GPIO_MODE_AF_PP;
	gpio_init_uart.Pull = GPIO_NOPULL;
	gpio_init_uart.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	gpio_init_uart.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &gpio_init_uart);

	// Configure the DMA for USART2_RX. According to Table 28 in RM0390, this is DMA1 Stream 5.
	// Choose Channel 4 for this particular configuration.
    hdma_rx->Instance = DMA1_Stream5;
    hdma_rx->Init.Channel = DMA_CHANNEL_4;
    hdma_rx->Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_rx->Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_rx->Init.MemInc = DMA_MINC_ENABLE;
    hdma_rx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_rx->Init.Mode = DMA_CIRCULAR;
    hdma_rx->Init.Priority = DMA_PRIORITY_LOW;
    hdma_rx->Init.FIFOMode = DMA_FIFOMODE_DISABLE;

    // Configure the DMA for USART2_TX. According to Table 28 in RM0390, this is DMA1 Stream 6.
    // Choose Channel [x] for this particular configuration.
    // TODO FIXME


	// Link USART2_RX to the DMA
	__HAL_LINKDMA(huart, DMA_Handle_tx, *hdma_rx);
	__HAL_LINKDMA(huart, DMA_Handle_rx, *hdma_tx);

	// Enable USART2 interrupts via NVIC
	__HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);	// Receive data register not empty
	HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/**
 * Called in HAL_UART_MspDeInit()
 */
void LunaSys_Serial_MSP_DeInit(UART_HandleTypeDef *huart) {
	if (huart->Instance != USART2)
		return;

	// Disable the USART2 Clock
	__HAL_RCC_USART2_CLK_DISABLE();

	// Deinitialize the GPIO
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);

	// Disable the DMA and the USART2 interrupts
	HAL_DMA_DeInit(huart->hdmarx);
	HAL_NVIC_DisableIRQ(USART2_IRQn);
}

/**
 * Initializes the DMA peripheral clock and DMA1 IRQ. Called in main
 */
void LunaSys_Serial_DMA_Init(DMA_HandleTypeDef *hdma_rx, DMA_HandleTypeDef *hdma_tx) {
	// Enable DMA1 controller clock
	__HAL_RCC_DMA1_CLK_ENABLE();

	// Enable DMA interrupts: transfer complete and transfer error
	__HAL_DMA_ENABLE_IT(hdma_rx, DMA_IT_TC);
	__HAL_DMA_ENABLE_IT(hdma_rx, DMA_IT_TE);
	__HAL_DMA_ENABLE_IT(hdma_tx, DMA_IT_TC);
	__HAL_DMA_ENABLE_IT(hdma_tx, DMA_IT_TE);

	// Configure DMA interrupt priority
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

/**
 * Assumes Serial_Init() has been called with a valid UART_HandleTypeDef
 */
LunaSys_Error LunaSys_Serial_Transmit(uint8_t data) {
	HAL_StatusTypeDef status = HAL_UART_Transmit(p_huart, &data, 1, 100);	// blocking, polling method for transmission of data
	return (status != HAL_OK) ? LUNASYSE_ERROR : LUNASYSE_OK;
}

/**
 * Assumes Serial_Init() has been called with a valid UART_HandleTypeDef
 */
LunaSys_Error LunaSys_Serial_Receive() {
	uint8_t data = 0;
	while (rxRp == rxWp);
	data = rxBuffer[rxRp++];
	rxRp &= (RX_BUFFER_SIZE - 1);
	return LUNASYSE_OK;
}
