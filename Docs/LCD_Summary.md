# LCD1602A Device Overview

## Preface

The LCD1602A is based on Hitachi's LCD HD44780 controller chip. It is a 16 x 2 character LCD, meaning that it can display a total of 32 ASCII characters over 2 rows, 16 characters each. 

Table 1 summarizes some basic characteristics of the LCD. 

<br />

__Table 1: Important LCD Characteristics__

| Characteristic | Description or Value |
|---------------|----------------------|
| Display Font  | 5 x 8 pixels or dots |

<br />

## Pinout

Table 2 lists the pins of the LCD1602A and their functions.

<br />

__Table 2: LCD1602A Pin Descriptions__

| Pin | Function |
|-----|----------|
| GND | Ground   |
| VCC | 5V       |
| Vo (LCD Contrast) | Controls contrast and brigntess of the LCD |
| RS (Register Select) | Separates the sending of instructions vs. data to the LCD. When the pin is set to LOW, this means an instruction is being sent from the controller. When HIGH, data is being sent instead. |
| RW (Read/Write) | Specifies whether the operation is a read (HIGH) or a write (LOW) |
| E (Enable) | Enables the display. When set to LOW, all changes on the RS, R/W, and data bus are ignored. When set to HIGH, incoming data and instructions are processed. |
| D0 -> D7 (Data Bus) | Carries 8-bit data, which forms characters based on the ASCII table. D0 is the LSB, and D7 is the MSB. 
| A, K (Anode, Cathode) | Control LED backlight

<br /> 

## Commands




