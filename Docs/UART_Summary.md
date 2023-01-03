# UART

## Preface

UART (Universal Asynchronous Receiver-Transmitter) is an asynchronous serial communication hardware device with configurable transmission speeds and data formats. UART supports 3 communication modes.

1. Simplex: Unidirectional, Tx sends to Rx, Rx does not need to send back information to Tx
2. Half-Duplex: Bidirectional, devices take turns transmitting and receiving
3. Full-Duplex: Bidirectional, devices send and receive at the same time

UART transmits individual bits of data sequentially, in most cases starting with the least significant and ending with the most significant. These data bits are "framed" with start and stop bits. At the destination, a second UART reassembles the bits.

See Figure 1 for the anatomy of a typical UART frame.

<p align="center">
  <img src="https://github.com/Henchel-Santillan/LunaSys/blob/master/Docs/Res/uart_frame.png"">
</p>

<p align="center"><b>Figure 1: UART frame showing stop bits=1 and start bit</b></p>

The __start bit__ signals to Rx that a new data frame (data bits) is arriving. The UART transmission line is typically held at high voltage (logic high, or '1') when idle. Tx pulls the line from HIGH to LOW for one clock cycle (meaning 1 bit, the start bit, is sampled during this time at the rising or falling edge of the clock signal) to begin transmission.

The __data frame__ or _character_ is represented by 5 to 9 bits, depending on the code set utilized. If a parity bit is not set, the data frame contains a number of bits in the range of 5 to 9 bits; otherwise, it will be comprised of 5 to 8 bits.
 
The __parity bit__, if set, describes the evenness or oddness of the data frame. After Rx receives the data frame, the number of bits with a value of 1 is counted. If the parity bit is __0__ (even parity), the total counted "1" bits should be an even number. If the parity bit is __1__ (odd parity), the total should be odd. If there is a mismatch (caused by, for example, incongruent baud rates), the data frame has been changed and should be discarded.

Tx drives the tranmission line from LOW to HIGH (in the "mark") for 1 or 2 clock cycles. What is sampled during this time are the __stop bits__.

When configuring UART, various key parameters must be considered. See Table 1.

<br />

__Table 1: Key UART Communication Parameters__
|  Parameter   |  Description |
|:------------:|--------------|
| Baud Rate    | Speed of communication over the data channel |
| Word Length  | The number of data bits transmitted or received in a single frame |
| Stop Bits    | The number of bits to use to signal the end of the data packet | 
| Parity       | Defines the evenness or oddness of the received data. Set by Tx, Rx uses parity to identify if the data bits have an error and if the frame should be rejected. |
| Mode         | Receive (Rx) or Transmit (Tx) or both |
| Flow Control | Allows Rx to signal when it is ready to receive data allowing synchronization with Tx. This enables slow and fast devices to communicate. Flow control is implemented in hardware via __RTS__ and __CTS__ lines. |

The parameters with an asterisk (*) next to them _must_ be the same on both Tx and Rx. The most common configuration is called "8N1", or a word length of 8 bits, no parity, and 1 stop bit.

<br />

## I/O Operation using the HAL

I/O operations (transmit, receive, write, read) are offered in three different modes:
1. Polling;
2. Interrupt; and
3. DMA.

In __polling__ mode, data processing is handled internally via a loop. The operation blocks until it returns, after which a return status can be used to determine whether the operation succeeded or exited with an error. To prevent the process from hanging, a timeout is employed.

In __interrupt__ mode, user-implemented callbacks (provided as HAL functions prepended with the `__weak` descriptor, specifying weak linkage) indicate end of operation. Callbacks are called _after_ data processing has started and the appropriate interruption is enabled.

In the context of the HAL, `USARTx_IRQHandler(void)` is defined in `stm32f4xx_it.c` (see `startup_stm32f446retx.s` for available IRQ Handlers), and calls `HAL_UART_IRQHandler(UART_HandleTypeDef*)`. The `HAL_UART_IRQHandler(UART_HandleTypeDef*)` function then calls the callbacks (weak or registered).

In __DMA__ mode, data processing is started via the DMA. The process returns after the appopriate DMA interruption is enabled. A DMA handle must be associated with a UART handle via `__HAL_LINKDMA`, which is done as part of the peripheral's MSP (MCU Support Package) initialization procedure, defined in `HAL_UART_MspInit(UART_HandleTypeDef*)` in the file `stm32f4xx_hal_msp.c`. 

In the context of the HAL, `DMAx_IRQHandler(void)` is defined in `stm32f4xx_it.c` (in this case, DMAx = DMA1 for both USART2_TX and USART2_RX), which calls `HAL_DMA_IRQHandler(DMA_HandleTypeDef*)`. This in turn calls process complete and/or error callbacks (weak or registered), which are assigned to the function pointer members of the DMA handle belonging to the UART handle in question, accomplished via `HAL_UART_Process_DMA(...)`. 
