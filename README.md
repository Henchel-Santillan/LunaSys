# LunaSys

LunaSys is the firmware/systems component of the Luna project. LunaSys uses the STM32F4 HAL and LL drivers to connect a VGA camera module (OV7670) and a 16x2 LCD display to the STM32F446RE NUCLEO-64 MCU. LunaSys interoperates with LunaUI, a custom platform-agnostic user interface solution built using Qt6 to display live feed from camera module, send data and commands to the hardware via USART and/or RS232, and (in a future release) process images at the application level.

## IO Register Map

Table 1 shows the project's I/O register map.

__Table 1: Luna Project Register Map, STM32F446RE NUCLEO-64__

| Pin Name (Function After Reset) | GPIO Alternate Function Assignment            |
|:-------------------------------:|:---------------------------------------------:|
| VBAT                            |
| PC13                            |  None (B1-USER) |
| PC14-OSC32_IN                   |
| PC15-OSC32_OUT                  | 
| PH0-OSC_IN                      | 
| PH1-OSC_OUT                     |
| NRST
| PC0                             |  None (LCD D4)  |
| PC1                             |  None (LCD D5)  |
| PC2                             |  None (LCD D6)  |
| PC3                             |  None (LCD D7)  |
| VSSA
| VDDA
| PA0-WKUP
| PA1                             |  None (ADC123_IN1)  |
| PA2
| PA3
| VSS
| VDD
| PA4                             |  DCMI_HSYNC  |
| PA5
| PA6                             |  DCMI_PIXCLK  |
| PA7
| PC4
| PC5
| PB0
| PB1
| PB2-BOOT 1
| PB10                            |  I2C2_SCL  |
| VCAP_1
| VSS
| VDD
| PB12                            |  None (LCD RS)  |
| PB13                            |  None (LCD RW)  |
| PB14                            |  None (LCD E)  |
| PB15
| PC6                             |  DCMI_D0  |
| PC7                             |  DCMI_D1  |
| PC8                             |  DCMI_D2  |
| PC9                             |  DCMI_D3  |
| PA8                             |  MCO1  |
| PA9
| PA10
| PA11
| PA12
| PA13(JTMS-SWDIO)
| VSS
| VDD
| PA14(JTCK-SWCLK)
| PA15(JTDI)
| PC10
| PC11                            |  DCMI_D4  |
| PC12                            |  I2C2_SDA  |
| PD2
| PB3(JTDO/TRACESWO)
| PB4(NJTRST)
| PB5
| PB6                             |  DCMI_D5  |
| PB7                             |  DCMI_VSYNC  |
| BOOT0
| PB8                             |  DCMI_D6  |
| PB9                             |  DCMI_D7  |
| VSS
| VDD
