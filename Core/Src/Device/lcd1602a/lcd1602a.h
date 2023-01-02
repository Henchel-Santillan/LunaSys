#ifndef LUNASYS_LCD1602A_LCD1602A_H
#define LUNASYS_LCD1602A_LCD1602A_H

#include "stm32f4xx.h"
#include "lcd1602a_config.h"


/*** Common Device Library structures ***/

typedef struct _Pin_TypeDef {

	GPIO_TypeDef *GPIO_PERIPHERAL;	// GPIOx
	uint32_t GPIO_PIN;				// GPIO_Pin_x

} Pin_TypeDef;

typedef struct _Data_Pins_TypeDef {

	Pin_TypeDef *D0, *D1, *D2, *D3, 	// MSB - Data Pins
				*D4, *D5, *D6, *D7;		// LSB - Data Pins

} Data_Pins_TypeDef;

typedef struct _LCD_TypeDef {

	Pin_TypeDef *RS;	// Register Select
	Pin_TypeDef *RW;	// Read-Write
	Pin_TypeDef *E;		// Enable

	Data_Pins_TypeDef *DATABUS;

} LCD_TypeDef;


/*** FUNCTIONS ***/

void Init_LCD1602A(uint8_t bit_mode, uint8_t lines, LCD_TypeDef *p_lcd, Pin_TypeDef *p_rs, Pin_TypeDef *p_rw, Pin_TypeDef *p_e, Data_Pins_TypeDef *p_data_bus);

void Home_Return_LCD1602A(LCD_TypeDef *p_lcd);

void Display_Clear_LCD1602A(LCD_TypeDef *p_lcd);
void Display_Off_LCD1602A(LCD_TypeDef *p_lcd);
void Display_On_LCD1602A(LCD_TypeDef *p_lcd);
void Display_ShiftLeft_LCD1602A(LCD_TypeDef *p_lcd);
void Display_ShiftRight_LCD1602A(LCD_TypeDef *p_lcd);
void Display_LeftToRight_LCD1602A(LCD_TypeDef *p_lcd);
void Display_RightToLeft_LCD1602A(LCD_TypeDef *p_lcd);

void Cursor_Off_LCD1602A(LCD_TypeDef *p_lcd);
void Cursor_On_LCD1602A(LCD_TypeDef *p_lcd);
void Cursor_Set_LCD1602A(LCD_TypeDef *p_lcd, uint8_t line, uint8_t column);
void Cursor_ShiftLeft_LCD1602A(LCD_TypeDef *p_lcd);
void Cursor_ShiftRight_LCD1602A(LCD_TypeDef *p_lcd);
void Cursor_JustifyLeft_LCD1602A(LCD_TypeDef *p_lcd);
void Cursor_JustifyRight_LCD1602A(LCD_TypeDef *p_lcd);

void Blink_Off_LCD1602A(LCD_TypeDef *p_lcd);
void Blink_On_LCD1602A(LCD_TypeDef *p_lcd);

void Write_Char_LCD1602A(LCD_TypeDef *p_lcd, uint8_t ch);
void Write_String_LCD1602A(LCD_TypeDef *p_lcd, uint8_t *str, uint8_t str_size);

#endif // LUNASYS_LCD1602A_LCD1602A_H
