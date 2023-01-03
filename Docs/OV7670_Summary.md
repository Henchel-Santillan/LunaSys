# OV7670 Device Overview

## Preface 
The OV7670 is a simple VGA camera module and image processor. It promises a maximum of 30 frames per second (fps) and VGA resolution (640 x 480 px). The OV7670 module used in the LunaSys project comes without a FIFO buffer.

<br />

## Pinout

Table 1 provides details about each of the pins on the OV7670. 

<br />

__Table 1: Pinout of OV7670 Camera Module__

| Pin | Description |
|---------|-------------------|
| VDD     | 3.3V power supply |
| GND     | Ground            |
| SCL     | SCCB (~I2C) Serial Clock Line |
| SDA     | SCCB (~I2C) Serial Data Line |
| VS      | VSYNC |
| HS      | HSYNC |
| PLK     | Pixel Clock Output |
| XLK     | System Clock |
| D0 - D7 | Pixel data output |
| RET     | Reset |
| PWDN    | Power down |

<br />

## Power Constraints
According to the datasheet, 3.3V can be safely supplied to VDD. The I/O pins may also receive 3.3V safely, given that internal protection diodes will clamp the voltage down to around 2.8V. 

<br />

## Configuration Signals
The OV7670 is parametrized through an I2C communication bus. On the F446-RE, pin `PB10` will be assigned to `I2C2_SCL`, which will connect to the `SCL` (serial clock line) pin of the OV7670. Pin `PC12` will be assigned to `I2C2_SDA`, which in turn will connect to the `SDA` (serial data line) pin of the OV7670.

<br />

## Clock and Timing Configuration
The OV7670 sends image data in a __parallel synchronous format__. When setting up the `DCMI`, it is therefore important that the "parallel" interface type is selected. 

An input clock signal must be provided by the MCU to the module via the `XLK` (system clock input) pin. The frequency of this signal, according to the data sheet, is between __10 and 48 MHz__, with __24 MHz__ being the typical (TYP). The STM32F446-RE is capable of providing up to __54 MHz__ (maximum DCMI pixel clock output). LunaSys will use `PA8` and assign it to `MCO1` to achieve a 24 MHz frequency. The camera module's `PLK` pin will connect to a GPIO pin (`PA6`) assigned to `DCMI_PIXCLK` via GPIO alternate functions on the F446-RE. These pin assignments are related to the pixel clock.

It may be worthwhile to note parenthetically that by default, the frequency of `XLK` is equivalent to that of `PLK`. Recall that `XLK` is fed a 24 MHz clock signal from the MCU.

The `VS` pin of the OV7670 is for `VSYNC`, and the `HS` pin is for `HSYNC`. For a valid capture, VSYNC must be configured __active high__ on the STM32 (since a valid capture occurs when VSYNC is low) and HSYNC must be configured __active low__ on the STM32 (since a valid capture occurs when HSYNC is high). During `HSYNC` high state, _x_ pixels must be captured. This is equivalent to _one line_. During `VSYNC` low state, _y_ lines must be captured. This gives the image resolution of _x_ by _y_ pixels.

On the F446-RE, pins `PA4` and `PB7` have been assigned to `DCMI_HSYNC` and `DCMI_VSYNC`, respectively. 

<br />

## Image Data 
The OV7670 has a total of 8 pixel data output pins, labelled `D0` to `D7`. Note that image data _must_ be sampled at the __rising edge__ of the `XCLK` signal. Table 2 shows the GPIO pins on the F446-RE assigned to DCMI for the pixel data output pins (`DCMI_Dx` on the F446-RE, and `Dx` on the OV7670, where x = {0, 1, 2, 3, 4 , 5, 6, 7}).

<br />

__Table 2: DCMI Pixel Data Output Pin Assignment__

| Pin Name | Alternate Function Assignment    | To Pin on OV7670 |
|:--------:|:--------------------------------:|:----------------:|
| PC6      |  DCMI_D0                         | D0               |
| PC7      |  DCMI_D1                         | D1               |
| PC8      |  DCMI_D2                         | D2               |
| PC9      |  DCMI_D3                         | D3               |
| PC11     |  DCMI_D4                         | D4               |
| PB6      |  DCMI_D5                         | D5               |
| PB8      |  DCMI_D6                         | D6               |
| PB9      |  DCMI_D7                         | D7               |

<br />

## Supported Output Formats
The OV7670 supports the following output data formats:
1. YUV/YCbCr
2. RGB565/555/444
3. GRB 4:2:2
4. Raw RGB Data

The OV7670 supports the following frame formats:
1. VGA (640 x 480)
2. QVGA (320 x 240)
3. CIF (352 x 240)
4. QCIF (176 x 144)
5. Manual Scaling

<br />

## SCCB

The SCCB (Serial Camera Control Bus) on the OV7670 is compatible with I2C. This bus will be used to configure the camera via software. See the OmniVision SCCB specification here 

>https://www.waveshare.com/w/upload/1/14/OmniVision_Technologies_Seril_Camera_Control_Bus%28SCCB%29_Specification.pdf 

for more information.
