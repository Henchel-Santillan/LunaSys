# LCD1602A Device Overview

## Preface

The LCD1602A is based on Hitachi's LCD HD44780 controller chip. It is a 16 x 2 character LCD, meaning that it can display a total of 32 ASCII characters over 2 rows, 16 characters each. 

<br />

## Pinout

Table 1 lists the pins of the LCD1602A and their functions.

<br />

__Table 1: LCD1602A Pin Descriptions__

| Pin | Function |
|:---:|----------|
| GND | Ground   |
| VCC | 5V       |
| Vo (LCD Contrast) | Controls contrast and brightness of the LCD |
| RS (Register Select) | Separates the sending of instructions vs. data to the LCD. When the pin is set to LOW, this means an instruction is being sent from the controller. When HIGH, data is being sent instead. |
| RW (Read/Write) | Specifies whether the operation is a read (HIGH) or a write (LOW) |
| E (Enable) | Enables the display. When set to LOW, all changes on the RS, R/W, and data bus are ignored. When set to HIGH, incoming data and instructions are processed. |
| D0 -> D7 (Data Bus) | Carries 8-bit data, which forms characters based on the ASCII table. D0 is the MSB, and D7 is the LSB. 
| A, K (Anode, Cathode) | Control LED backlight

<br /> 

The LED backlight "A" pin allows 5 V, with 1.1 mA current. The "K" pin connects to GND.

## Commands and Flags

Table 2 lists the commands - and their associated flags - supported by the LCD1602A.

<br />

__Table 2: LCD1602A Commands and Flags__

| Command (Hex)            | Flags | Hex Value, W |
|:------------------------:|-----------------|:------------:|
| Clear Display (0x01)     | N/A | N/A |
| Return Home (0x02)       | N/A | N/A |
| Entry Mode Set (0x04)    | Increment <br /> Decrement <br /> Shift Left <br /> Shift Right | 0x02 <br /> 0x00 <br /> 0x01 <br /> 0x00 |
| Display Control (0x08)   | Display On <br /> Display Off <br /> Cursor On <br /> Cursor Off <br /> Blink On <br /> Blink Off <br /> | 0x04 <br /> 0x00 <br /> 0x02 <br /> 0x00 <br /> 0x01 <br /> 0x00 |
| Shift (0x10)             | Cursor <br /> Display <br /> Left <br /> Right | 0x00 <br /> 0x08 <br /> 0x00 <br /> 0x04 |
| Function Set (0x20)      | 8 Bit Mode <br /> 4 Bit Mode </br> Two Lines <br /> One Line <br /> 5 x 11 Mode <br /> 5 x 8 Mode | 0x10 <br /> 0x00 <br /> 0x08 <br /> 0x00 <br /> 0x04 <br /> 0x00 | 
| CGRAM Address Set (0x40) | N/A | N/A | 
| DDRAM Address Set (0x80) | N/A | N/A |

<br />

Writing commands to the LCD requires pulling the `RS` and `RW` pins low. Using the STM32F4 HAL, this can be done by writing to GPIO and using `GPIO_PIN_RESET`. With the exception of the first two commands listed in Table 2, all commands have associated flags to further specify them. In the LunaSys Device Library for the LCD1602A, commands and flags are chained or set using bitwise `OR`, and are removed or "unset" using a combination of bitwise `AND` assignment and `NOT`. 

On the other hand, writing data to the LCD using the __Data Bus__ requires pulling `RS` high, and `RW` low. The `GPIO_PIN_SET` macro is useful here when writing to GPIO. The documentation at

>https://www.openhacks.com/uploadsproductos/eone-1602a1.pdf

provides a table mapping binary values to certain ASCII characters. For example, the uppercase letter "A" can be written to the LCD by pulling `RS` high, pulling `RW` low, and then sending `uint8_t` with value `00000100` to the databus. Note that for a 4-bit configuration, the value `00000100` must be split into two nibbles (half-bytes, or 4 bits), and two write operations need to be performed. The LunaSys project will use 4-bit mode to save GPIO pins; the drawback is that writing to the LCD will be slower. 

LunaSys will use the LCD1602A to display setting information. The current relevant use case is the camera mode (SNAPSHOT vs CONTINUOUS).

Table 3 gives the GPIO ports and pins used for the LCD.

<br />

__Table 3: LCD1602A GPIO Port and Pin Assignments__

| Port and Pin | LCD1602A Pin |
|:------------:|:------------:|
|  PC0         |  D4          |
|  PC1         |  D5          |
|  PC2         |  D6          |
|  PC3         |  D7          |
|  PB12        |  RS          |
|  PB13        |  RW          |
|  PB14        |  E           |


