#include "lcd1602a.h"

#include "lcd1602aConfig.h"

/*** STATIC VARIABLES ***/
static uint8_t lcd_line;
static uint8_t lcd_column;
static uint8_t flags_entry_mode;
static uint8_t flags_display_control;
static uint8_t flags_function_set;


/*** STATIC HELPER FUNCTIONS ***/

static void write_4bits(LCD_TypeDef *p_lcd, uint8_t value) {
	// Concerned with the LSB; therefore, write to the lower nibble ("half-byte")
	HAL_GPIO_WritePin(p_lcd->D4->GPIO_PERIPHERAL, p_lcd->D4->GPIO_PIN, (value & 0x08));
	HAL_GPIO_WritePin(p_lcd->D5->GPIO_PERIPHERAL, p_lcd->D5->GPIO_PIN, (value & 0x04));
	HAL_GPIO_WritePin(p_lcd->D6->GPIO_PERIPHERAL, p_lcd->D6->GPIO_PIN, (value & 0x02));
	HAL_GPIO_WritePin(p_lcd->D7->GPIO_PERIPHERAL, p_lcd->D7->GPIO_PIN, (value & 0x01));
}

#ifdef MODE_EIGHT_BIT
static void write_8bits(LCD_TypeDef *p_lcd, uint8_t value) {
	HAL_GPIO_WritePin(p_lcd->D0->GPIO_PERIPHERAL, p_lcd->D0->GPIO_PIN, (value & 0x80));
	HAL_GPIO_WritePin(p_lcd->D1->GPIO_PERIPHERAL, p_lcd->D1->GPIO_PIN, (value & 0x40));
	HAL_GPIO_WritePin(p_lcd->D2->GPIO_PERIPHERAL, p_lcd->D2->GPIO_PIN, (value & 0x20));
	HAL_GPIO_WritePin(p_lcd->D3->GPIO_PERIPHERAL, p_lcd->D3->GPIO_PIN, (value & 0x10));
	HAL_GPIO_WritePin(p_lcd->D4->GPIO_PERIPHERAL, p_lcd->D4->GPIO_PIN, (value & 0x08));
	HAL_GPIO_WritePin(p_lcd->D5->GPIO_PERIPHERAL, p_lcd->D5->GPIO_PIN, (value & 0x04));
	HAL_GPIO_WritePin(p_lcd->D6->GPIO_PERIPHERAL, p_lcd->D6->GPIO_PIN, (value & 0x02));
	HAL_GPIO_WritePin(p_lcd->D7->GPIO_PERIPHERAL, p_lcd->D7->GPIO_PIN, (value & 0x01));
}
#endif


/*** FUNCTION DEFINITIONS ***/

void Init_LCD1602A(LCD_TypeDef *p_lcd, Pin_TypeDef *rs, Pin_TypeDef *rw, Pin_TypeDef *e,

#ifdef MODE_EIGHT_BIT
		Pin_TypeDef *D0, Pin_TypeDef *D1, Pin_TypeDef *D2, Pin_TypeDef *D3,
#endif
		Pin_TypeDef *D4, Pin_TypeDef *D5, Pin_TypeDef *D6, Pin_TypeDef *D7) {

	p_lcd->RS = rs;
	p_lcd->RW = rw;
	p_lcd->E = e;

	p_lcd->D4 = D4;
	p_lcd->D5 = D5;
	p_lcd->D6 = D6;
	p_lcd->D7 = D7;

#ifdef MODE_EIGHT_BIT
	p_lcd->D0 = D0;
	p_lcd->D1 = D1;
	p_lcd->D2 = D2;
	p_lcd->D3 = D3;

	// The LCD requires at least 40 ms after voltage rises above 2.7 V before sending commands. Add an extra 5 ms time buffer.
	// See p 45/60 of the Hitachi HD44780 Datasheet https://www.sparkfun.com/datasheets/LCD/HD44780.pdf.
	HAL_Delay(45000);

	// Set the LCD to be 8-Bit Mode
	flags_function_set = FL_FUNCTIONSET_8BITMODE | FL_FUNCTIONSET_TWOLINE | FL_FUNCTIONSET_5x8MODE;
#else
	// Set the LCD to be 4-Bit Mode
	flags_function_set = FL_FUNCTIONSET_4BITMODE | FL_FUNCTIONSET_TWOLINE | FL_FUNCTIONSET_5x8MODE;
#endif

	send_command(p_lcd, CMD_FUNCTIONSET | flags_function_set);

	// Set the Display, Blink, and Cursor to be all ON
	flags_display_control = FL_DISPLAYCONTROL_DISPLAYON | FL_DISPLAYCONTROL_CURSORON | FL_DISPLAYCONTROL_BLINKON;
	send_command(p_lcd, CMD_DISPLAYCONTROL | flags_display_control);

	flags_entry_mode = FL_ENTRYMODESET_SHIFTRIGHT | FL_ENTRYMODESET_DDRAMDECR;
	send_command(p_lcd, CMD_ENTRYMODESET | flags_entry_mode);

	// Clear the display and set the cursor to point at line 1 (index 0), column 1 (index 0)
	Display_Clear_LCD1602A(p_lcd);
	Home_Return_LCD1602A(p_lcd);
}

void Display_Clear_LCD1602A(LCD_TypeDef *p_lcd) {
	send_command(p_lcd, CMD_CLEARDISPLAY);
}

void Display_Off_LCD1602A(LCD_TypeDef *p_lcd) {
	flags_display_control &= ~(FL_DISPLAYCONTROL_DISPLAYON);
	send_command(p_lcd, CMD_DISPLAYCONTROL | flags_display_control);
}

void Display_On_LCD1602A(LCD_TypeDef *p_lcd) {
	flags_display_control |= FL_DISPLAYCONTROL_DISPLAYON;
	send_command(p_lcd, CMD_DISPLAYCONTROL | FL_DISPLAYCONTROL_DISPLAYON);
}

void Display_ShiftLeft_LCD1602A(LCD_TypeDef *p_lcd) {
	send_command(p_lcd, CMD_SHIFT | FL_SHIFT_DISPLAY | FL_SHIFT_LEFT);
}

void Display_ShiftRight_LCD1602A(LCD_TypeDef *p_lcd) {
	send_command(p_lcd, CMD_SHIFT | FL_SHIFT_DISPLAY | FL_SHIFT_RIGHT);
}

void Home_Return_LCD1602A(LCD_TypeDef *p_lcd) {
	send_command(p_lcd, CMD_RETURNHOME);
	lcd_line = 0;
	lcd_column = 0;
}

void Cursor_Off_LCD1602A(LCD_TypeDef *p_lcd) {
	flags_display_control &= ~(FL_DISPLAYCONTROL_CURSORON);
	send_command(p_lcd, CMD_DISPLAYCONTROL | flags_display_control);
}

void Cursor_On_LCD1602A(LCD_TypeDef *p_lcd) {
	flags_display_control |= FL_DISPLAYCONTROL_CURSORON;
	send_command(p_lcd, CMD_DISPLAYCOMNTROL | flags_display_control);
}

void Cursor_Set_LCD1602A(LCD_TypeDef *p_lcd, uint8_t line, uint8_t column) {
	lcd_line = line;
	lcd_column = column;
}

void Cursor_ShiftLeft_LCD1602A(LCD_TypeDef *p_lcd) {
	send_command(p_lcd, CMD_SHIFT | FL_SHIFT_CURSOR | FL_SHIFT_LEFT);
}

void Cursor_ShiftRight_LCD1602A(LCD_TypeDef *p_lcd) {
	send_command(p_lcd, CMD_SHIFT | FL_SHIFT_CURSOR | FL_SHIFT_RIGHT);
}

void Cursor_JustifyLeft_LCD1602A(LCD_TypeDef *p_lcd) {

}

void Blink_Off_LCD1602A(LCD_TypeDef *p_lcd) {
	flags_display_control &= ~(FL_DISPLAYCONTROL_BLINKON);
	send_command(p_lcd, CMD_DISPLAYCONTROL | FL_DISPLAYCONTROL_BLINKOFF);
}

void Blink_On_LCD1602A(LCD_TypeDef *p_lcd) {
	flags_display_control |= FL_DISPLAYCONTROL_BLINKON;
	send_command(p_lcd, CMD_DISPLAYCONTROL | FL_DISPLAYCONTROL_BLINKON);
}

void Write_Char_LCD1602A(LCD_TypeDef *p_lcd) {

}

void Write_String_LCD1602A(LCD_TypeDef *p_lcd) {

}

void send_command(LCD_TypeDef *p_lcd, uint8_t command) {
	// To send commands, must pull RS and RW both to LOW
	HAL_GPIO_WritePin(p_lcd->RS->GPIO_PERIPHERAL, p_lcd->RS->GPIO_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(p_lcd->RW->GPIO_PERIPHERAL, p_lcd->RW->GPIO_PIN, GPIO_PIN_RESET);

#ifdef MODE_EIGHT_BITS
	write_8bits(p_lcd, command);
#else
	write_4bits(p_lcd, command);
#endif
}

void send_data(LCD_TypeDef *p_lcd, uint8_t data) {
	// To send data, the RS pin must be pulled HIGH, and the RW pin must be pulled LOW
	HAL_GPIO_WritePin(p_lcd->RS->GPIO_PERIPHERAL, p_lcd->RS->GPIO_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(p_lcd->RW->GPIO_PERIPHERAL, p_lcd->RW->GPIO_PIN, GPIO_PIN_RESET);

#ifdef MODE_EIGHT_BITS
	write_8bits(p_lcd, data);
#else
	write_4bits(p_lcd, data);
#endif
}
