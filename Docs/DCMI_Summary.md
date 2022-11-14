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
| Power Supply | Fairly obvious use case. See the Power Constraints section of the OV7670_Summary.md for more information. |
| Camera Configuration | Related to several image features, including resolution, format, frame rate, contrast, and brightness. These signals are also used for interface type selection (parallel versus serial). |      

<br />

## DCMI Signals and Architecture

Figure 1 shows a block diagram with the input and output signals of the DCMI.

<p align="center">
  <img src="https://github.com/Henchel-Santillan/LunaSys/blob/master/Docs/Res/dcmi_signals.png">
</p>

<p align="center"><b>Figure 1: DCMI signals</b></p>

The OV7670 has 8 pixel data output pins that interface with GPIO pins on the STM32F446-RE assigned to DCMI (`DCMI_Dx`) through GPIO alternate functions. In particular, x (in `DCMI_Dx`) is in the set { 0, 1, 2, 3, 4, 5, 6, 7 }; an 8-bit data width means that 1 byte of image data is transferred each pixel clock (DCMI_PIXCLK) cycle into a __data extractor__, which - unsurprisingly - extracts the data received by the DCMI. 

The `DCMI_PLK`, `DCMI_VSYNC`, and `DCMI_HSYNC` control signals are fed into a __synchronizer__, which ascertains that the flow of data through the DCMI is controlled and ordered. It is responsible for controlling the data extractor, as well as two components yet to be introduced: the __4-word FIFO__ and the __32-bit data register__.

The 4-word FIFO is where data from the data extractor is packed. It adapts the data rate transfers to the AMBA High-Performance (AHB) bus. No overrun protection is established; that is, in the event of overrun (overfilling of a buffer causing memory corruption) or sync signal errors, data in the FIFO may be overwritten if the AHB does not sustain the data transfer rate. When this happens, the FIFO is reset, and the DCMI waits for the start of a new frame. 

Figure 2 shows the main components of the DCMI, taken from the AN5020 STM reference. 

<p align="center">
  <img src="https://github.com/Henchel-Santillan/LunaSys/blob/master/Docs/Res/dcmi_arch.png">
</p>

<p align="center"><b>Figure 2: DCMI architecture [1]</b></p>

The 32-bit register is where the data from the 4-word FIFO is ordered. The little-endian format is supported and used, wherein the LSB (least significant byte) is stored in the smallest address. As may be observed for the 32-bit word in Figure 3, the first captured data byte is placed in the LSB position and the fourth captured byte is placed in the MSB position. For the OV7670, we are interested in an 8-bit data width; a 32-bit word would thus be made up every four pixel clock cycles (32 / 8 = 4). See Figure 3.

<p align="center">
  <img src="https://github.com/Henchel-Santillan/LunaSys/blob/master/Docs/Res/dcmi_32bitword.png">
</p>

<p align="center"><b>Figure 3: Structure of 32-bit word [1]</b></p>

Once the 32-bit word is made, a DMA (direct memory access) request is generated. The DCMI is connected to the AHB matrix via AHB2 peripherals, and is accessed by the DMA controller to transfer data to the appropriate memory destination (either to internal SRAMs or to external memory via flexible memory controller, or FMC).

<br />

## DCMI Data Synchronization

The data synchronization method implemented in Luna is the __hardware__ or __external__ synchronization method. The `DCMI_PIXCLK` captured edge is configured to be the rising edge (by convention) and both `DCMI_VSYNC` and `DCMI_HSYNC` are programmed active level. 

In this project, the active level is active high for both LINE VALID (HSYNC) and FRAME VALID (VSYNC). This means that data at the sampling points (rising edges of the clock signal) are considered invalid ("blank" frames, either vertical or horizontal blanking) when HSYNC or VSYNC are high.

See Figure 4 for reference.

<p align="center">
  <img src="https://github.com/Henchel-Santillan/LunaSys/blob/master/Docs/Res/dcmi_hwframestruct.png">
</p>

<p align="center"><b>Figure 4: Hardware Frame Structure</b> [1]</p>

<br />

## A Note on Capture Modes

Two capture modes are supported by the DCMI: 

1. Snapshot
2. Continuous Grab

### Snapshot Mode

In this mode, only a single frame is captured. Data sampling only begins once the interface detects a start of frame, which is represented by a `DCMI_VSYNC` signal for the hardware (external) synchronization mode. 

Once the first completed frame is received, the DCMI is automatically disabled, resulting in subsequent frames being ignored. In the event overrun takes place, the frame is considered to be "lost" and the camera interface is disabled. 

The capture is enabled by setting the `CAPTURE` bit in `DCMI_CR`, and is cleared when the first frame is completely sampled.

### Continuous Grab Mode

In continuous grab mode, data sampling also begins once the interface detects a `DCMI_VSYNC` signal. The difference between this mode and snapshot mode is that the DCMI is not automatically disabled; instead, the user must disable it by setting `CAPTURE=0`. It should be noted that the DCMI will continue to grab data until the end of the current frame. 

<br />

## Configuration

To correctly implement the application, first reset the DCMI and the camera module. Then, the following configurations should be made:

1. GPIOs;
2. Timings and clocks;
3. DCMI peripheral;
4. DMA; and
5. Camera Module.

Some practices and recommendations are given in the ensuing sections.

### GPIO Configuration 

GPIOs can be assigned to DCMI using GPIO alternate functions. Depending on the configuration of the `EDM` (extended data mode) bits in the `DCMI_CR` register, the DCMI should receive an 8-bpp clock (`DCMI_PIXCLK`). 

Given an 8-bit data width as well as hardware (external) synchronization, only 11 GPIOs need to be assigned to DCMI. These are given below.

1. Eight (8) `DCMI_Dx` data pins
2. One (1) `DCMI_PIXCLK` pin
3. One (1) `DCMI_VSYNC` pin
4. One (1) `DCMI_HSYNC` pin

### Clock and Timing Configuration

The highest system clock will be used for best performance. The `DCMI_PIXCLK` configuration will be the same as the pixel clock configuration of the camera module.

The capture edge (rising or falling) is configured through the `PCKPOL` bit in the `DCMI_CR` register. The synchronization mode (hardware versus embedded) is configured through the `ESS` bit in the `DCMI_CR`. 

For the hardware synchronization mode, the `DCMI_HSYNC` and `DCMI_VSYNC` polarities are programmed according to the camera module configuration, and are set using the `HSPOL` and `VSPOL` bits in `DCMI_CR`, respectively. 

### DCMI and DMA Configuration

[TODO] Finish this section!

Configuring the DCMI means selecting the capture mode, the data format, image size, and resolution. See section 6.3 of AN5020 for more details.

Configuring the DMA means setting up the proper DCMI-to-memory transfers (transfer direction, source address, destination address, peripheral data width, the number of 32-bit data words for transfer, and the DMA operation mode), the configuration relative to image size and capture mode, channel and stream, and the FIFO. See section 6.4 of AN5020 for more information. 

### Camera Module

The configuration signals are sent from the camera module to the DCMI via I2C protocol. Thus, I/O functionalities for camera configuration pins should be configured to enable the modification of its registers. 

As noted above, it is advised to perform a full hardware reset on the camera module before initialization.

In the initialization step, several key features can be configured:

1. Image resolution
2. Contrast and brightness
3. White balance
4. Synchronization mode
5. Clock signal frequencies
6. Output data format

__________

# References

[1] ST Microelectronics, "AN5020 Revision 2: Digital camera interface (DCMI) for STM32 MCUs - Application note," 2022. [Online]. Available:        https://www.st.com/resource/en/application_note/an5020-digital-camera-interface-dcmi-on-stm32-mcus-stmicroelectronics.pdf.
