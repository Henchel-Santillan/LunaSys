#ifndef LUNASYS_LCD1602A_LCD1602A_H
#define LUNASYS_LCD1602A_LCD1602A_H

#include "lcd1602aConfig.h"
#include "stm32f4xx_hal.h"

#include <stdint.h>

// Uncomment to use eight-bit mode
// #define MODE_EIGHT_BIT


/*** Common Device Library structures ***/

typedef struct _Pin_TypeDef {

	GPIO_TypeDef *GPIO_PERIPHERAL;	// GPIOx, x = { A, ..., K }
	uint32_t GPIO_PIN;				// GPIO_Pin_x, x = { 0, ..., 15 }

} Pin_TypeDef;


typedef struct _LCD_TypeDef {

	Pin_TypeDef *RS;	// Register Select
	Pin_TypeDef *RW;	// Read-Write
	Pin_TypeDef *E;		// Enable

#ifdef MODE_EIGHT_BIT
	Pin_TypeDef *D0, *D1, *D2, D3*;		// MSB - Data Pins
#endif

	Pin_TypeDef *D4, *D5, *D6, *D7;		// LSB - Data Pins


} LCD_TypeDef;


/*** FUNCTIONS ***/

void Init_LCD1602A(LCD_TypeDef *p_lcd, Pin_TypeDef *rs, Pin_TypeDef *rw, Pin_TypeDef *e,

#ifdef MODE_EIGHT_BIT
		Pin_TypeDef *D0, Pin_TypeDef *D1, Pin_TypeDef *D2, Pin_TypeDef *D3,
#endif
		Pin_TypeDef *D4, Pin_TypeDef *D5, Pin_TypeDef *D6, Pin_TypeDef *D7);

void Clear_Display_LCD1602A(LCD_TypeDef *p_lcd);

void Return_Home_LCD1602A(LCD_TypeDef *p_lcd);

void Shift_Cursor_LCD1602A(LCD_TypeDef *p_lcd);

void Write_Char_LCD1602A(LCD_TypeDef *p_lcd, uint8_t *ch);

void Write_String_LCD1602A(LCD_TypeDef *p_lcd, uint8_t *str);

void Display_Off_LCD1602A(LCD_TypeDef *p_lcd);

void Display_On_LCD1602A(LCD_TypeDef *p_lcd);

void Blink_Off_LCD1602A(LCD_TypeDef *p_lcd);

void Blink_On_LCD1602A(LCD_TypeDef *p_lcd);

void Scroll_Left_LCD1602A(LCD_TypeDef *p_lcd);

void Scroll_Right_LCD1602A(LCD_TypeDef *p_lcd);



/* Users of this Device Library should generally not have to call these functions, and it is advised to use
 * the Device Library functions suffixed with "LCD1602A" instead. Users that do choose to use these functions should
 * include "lcd1602aConfig.h" and use the predefined command and flag macros.
 *  */

void send_command(LCD_TypeDef *p_lcd, uint8_t command);
void send_data(LCD_TypeDef *p_lcd, uint8_t data);


#endif // LUNASYS_LCD1602A_LCD1602A_H
