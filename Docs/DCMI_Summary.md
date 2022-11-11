# Digital Camera Interface (DCMI) and OV7670

## Preface

A camera module requires 4 main signals to properly transmit image data:

1. Control
2. Image Data
3. Power Supply
4. Camera Configuration

Table 1 explains each signal type in more detail. 

__Table 1: Camera Module Signals Overview__

| Signal Type | Description |
| :----------:|:------------|
| Control     | Used for clock generation and data transfer synchonization. Camera modules provide two data synchronization signals: (1) HSYNC (horizontal or line synchronization); and (2) VSYNC (vertical or frame synchronization). |
| Image Data  | The width of these signals determines the number of bits to be transferred at each pixel clock. |
| Power Supply | Fairly obvious use case. The OV7670 in particular requires 3.3V+ in.|
| Camera Configuration | Related to several image features, including resolution, format, frame rate, contrast, and brightness. These signals are also used for interface type selection (parallel versus serial). |      

<br />

## DCMI Signals and Architecture

Figure 1 shows 

![](INSERT dcmi_signals.png HERE LATER)

The OV7670 has 8 pixel data output pins that interface with GPIO pins on the STM32F446-RE assigned to DCMI (`DCMI_Dx`) through GPIO alternate functions. In particular, x (in `DCMI_Dx`) is in the set { 0, 1, 2, 3, 4, 5, 6, 7 }; an 8-bit data width means that 1 byte of image data is transferred each pixel clock (DCMI_PIXCLK) cycle into a __data extractor__, which - unsurprisingly - extracts the data received by the DCMI. 

The `DCMI_PLK`, `DCMI_VSYNC`, and `DCMI_HSYNC` control signals are fed into a __synchronizer__, which ascertains that the flow of data through the DCMI is controlled and ordered. It is responsible for controlling the data extractor, and two components yet to be introduced: the __4-word FIFO__ and the __32-bit data register__.

The 4-word FIFO is where data from the data extractor is packed. It adapts the data rate transfers to the AMBA High-Performance (AHB) bus. No overrun protection is established; that is, in the event of overrun (overfilling of a buffer causing memory corruption) or sync signal errors, data in the FIFO may be overwritten if the AHB does not sustain the data transfer rate. What occurs instead is a FIFO reset; the DCMI also waits for the start of a new frame. 

Figure 2 shows the main components of the DCMI, taken from the AN5020 STM reference. 

![](INSERT dcmi_arch.png HERE LATER)

The 32-bit register is where the data from the 4-word FIFO is ordered. The little-endian format is supported and used, wherein the LSB (least significant byte) is stored in the smallest address. As may be observed for the 32-bit word in Figure 3, the first captured data byte is placed in the LSB position and the fourth captured byte is placed in the MSB position. For the OV7670, we are interested in an 8-bit data width; a 32-bit word would thus be made up every four pixel clock cycles (32 / 8 = 4).

![](INSERT dcmi_32bitword.png HERE LATER)

Once the 32-bit word is made, a DMA (direct memory access) request is generated. The DCMI is connected to the AHB matrix via AHB2 peripherals, and is accessed by the DMA controller to transfer data to the appropriate memory destination (either to internal SRAMs or to external memory via flexible memory controller, or FMC).

<br />

## Connecting the DCMI with the OV7670

The data synchronization method implemented in Luna is the __hardware__ or __external__ synchronization method. The `DCMI_PIXCLK` captured edge is configured to be the rising edge (by convention) and both `DCMI_VSYNC` and `DCMI_HSYNC` are programmed active level. In this project, the active level is active high for both LINE VALID (HSYNC) and FRAME VALID (VSYNC).  

Both the VSYNC and HSYNC signals act as . This means that since HSYNC and VSYNC are active high, data at the sampling points (rising edges of the clock signal) are considered invalid ("blank" frames, either vertical or horizontal blanking).

See Figure 4 for reference.

<br />

## A Note on Capture Modes
