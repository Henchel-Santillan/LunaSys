# LunaSys

LunaSys is the firmware/systems component of the Luna project. LunaSys uses the STM32F4 HAL and LL drivers to connect a VGA camera module (OV7670) and a 16x2 LCD display to the STM32F446RE NUCLEO-64 MCU. LunaSys interoperates with [LunaUI](https://github.com/Henchel-Santillan/LunaUI), a custom platform-agnostic user interface solution built using Qt6 to display live feed from the camera module, and send data and commands to the hardware via USART (using RS232). Image processing at the application level may be in a future release.

## Repository Navigation
All relevant user code can be found in the [Core](https://github.com/Henchel-Santillan/LunaSys/tree/master/Core) directory. See the [Docs](https://github.com/Henchel-Santillan/LunaSys/tree/master/Docs) directory for LunaSys-relevant peripheral summaries, image resources, and architecture diagrams.

## Building and Flashing

See [UM2609](https://www.st.com/resource/en/user_manual/um2609-stm32cubeide-user-guide-stmicroelectronics.pdf) user guide for the STM32CubeIDE.

Refer to Section 36 (_STM32F446xx devices bootloader_) and __Table 2__ (_Bootloader activation patterns_) under Section 4 (_General bootloader description_) of the [AN2606](https://www.st.com/resource/en/application_note/cd00167594-stm32-microcontroller-system-memory-boot-mode-stmicroelectronics.pdf) application note for more information on the board-specific bootloader.

The instructions that follow are for flashing to a __DFU connected board__ using the STM32CubeIDE.

1. Use a MicroUSB Type-B to USB Type-A connector to connect the F446RE MCU to a PC.
2. In the STM32CubeIDE, click once on the project in the _Project Explorer_ pane and click `Project` in the toolbar and then `Build Project`. 
3. Click `Run`. 
4. Edit the `Run Configurations` as follows:

* In the `Main` tab,
* In the `Debugger` tab,
* In the `Startup` tab,
* In the `Source` tab,
* In the `Common` tab,

## High-Level Architecture

<p align="center">
  <img src="https://github.com/Henchel-Santillan/LunaSys/blob/master/Docs/Res/hw_arch.png">
</p>

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
| PA1                             |  |
| PA2                             |  USART2_TX          |
| PA3                             |  USART2_RX          |
| VSS
| VDD
| PA4                             |  DCMI_HSYNC  |
| PA5
| PA6                             |  DCMI_PIXCLK  |
| PA7
| PC4
| PC5                             | None (External Pushbutton) |
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
