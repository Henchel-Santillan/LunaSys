#include "lcd1602a.h"

/**
 * States
 */

static uint8_t curr_line;	// Current line
static uint8_t curr_col;	// Current column

static uint8_t fl_display_control;		// ON/OFF for display, cursor, and blink
static uint8_t fl_display_entry_mode;	// DDRAM address increment/decrement, and display shift
static uint8_t fl_display_function;		// IDLC (bit mode), line number, and font type control


/**
 * Internal EN and R/W API
 */

static void enable_pulse(LCD_TypeDef *p_lcd) {
	HAL_GPIO_WritePin(p_lcd->E->GPIO_PERIPHERAL, p_lcd->E->GPIO_PIN, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(p_lcd->E->GPIO_PERIPHERAL, p_lcd->E->GPIO_PIN, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(p_lcd->E->GPIO_PERIPHERAL, p_lcd->E->GPIO_PIN, GPIO_PIN_RESET);
}

static void write_4bits(LCD_TypeDef *p_lcd, uint8_t value) {
	// Concerned with the LSB; therefore, write to the lower nibble ("half-byte")
	HAL_GPIO_WritePin(p_lcd->DATABUS->D4->GPIO_PERIPHERAL, p_lcd->DATABUS->D4->GPIO_PIN, (value & 0x08));
	HAL_GPIO_WritePin(p_lcd->DATABUS->D5->GPIO_PERIPHERAL, p_lcd->DATABUS->D5->GPIO_PIN, (value & 0x04));
	HAL_GPIO_WritePin(p_lcd->DATABUS->D6->GPIO_PERIPHERAL, p_lcd->DATABUS->D6->GPIO_PIN, (value & 0x02));
	HAL_GPIO_WritePin(p_lcd->DATABUS->D7->GPIO_PERIPHERAL, p_lcd->DATABUS->D7->GPIO_PIN, (value & 0x01));

	enable_pulse(p_lcd);
}

static void write_8bits(LCD_TypeDef *p_lcd, uint8_t value) {
	HAL_GPIO_WritePin(p_lcd->DATABUS->D0->GPIO_PERIPHERAL, p_lcd->DATABUS->D0->GPIO_PIN, (value & 0x80));
	HAL_GPIO_WritePin(p_lcd->DATABUS->D1->GPIO_PERIPHERAL, p_lcd->DATABUS->D1->GPIO_PIN, (value & 0x40));
	HAL_GPIO_WritePin(p_lcd->DATABUS->D2->GPIO_PERIPHERAL, p_lcd->DATABUS->D2->GPIO_PIN, (value & 0x20));
	HAL_GPIO_WritePin(p_lcd->DATABUS->D3->GPIO_PERIPHERAL, p_lcd->DATABUS->D3->GPIO_PIN, (value & 0x10));
	HAL_GPIO_WritePin(p_lcd->DATABUS->D4->GPIO_PERIPHERAL, p_lcd->DATABUS->D4->GPIO_PIN, (value & 0x08));
	HAL_GPIO_WritePin(p_lcd->DATABUS->D5->GPIO_PERIPHERAL, p_lcd->DATABUS->D5->GPIO_PIN, (value & 0x04));
	HAL_GPIO_WritePin(p_lcd->DATABUS->D6->GPIO_PERIPHERAL, p_lcd->DATABUS->D6->GPIO_PIN, (value & 0x02));
	HAL_GPIO_WritePin(p_lcd->DATABUS->D7->GPIO_PERIPHERAL, p_lcd->DATABUS->D7->GPIO_PIN, (value & 0x01));

	enable_pulse(p_lcd);
}

static void send_command(LCD_TypeDef *p_lcd, uint8_t command) {
	// To send commands, must pull RS and RW both to LOW
	HAL_GPIO_WritePin(p_lcd->RS->GPIO_PERIPHERAL, p_lcd->RS->GPIO_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(p_lcd->RW->GPIO_PERIPHERAL, p_lcd->RW->GPIO_PIN, GPIO_PIN_RESET);

	if (fl_display_function & FL_FUNCTIONSET_8BITMODE) {
		write_8bits(p_lcd, command);
	} else {
		write_4bits(p_lcd, command);
	}
}

static void send_data(LCD_TypeDef *p_lcd, uint8_t data) {
	// To send data, the RS pin must be pulled HIGH, and the RW pin must be pulled LOW
	HAL_GPIO_WritePin(p_lcd->RS->GPIO_PERIPHERAL, p_lcd->RS->GPIO_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(p_lcd->RW->GPIO_PERIPHERAL, p_lcd->RW->GPIO_PIN, GPIO_PIN_RESET);

	if (fl_display_function & FL_FUNCTIONSET_8BITMODE) {
		write_8bits(p_lcd, data);
	} else {
		write_4bits(p_lcd, data);
	}
}


/**
 * LCD1602A user-facing Device Library API
 */

void Init_LCD1602A(uint8_t bit_mode, uint8_t lines, LCD_TypeDef *p_lcd, Pin_TypeDef *p_rs, Pin_TypeDef *p_rw, Pin_TypeDef *p_e, Data_Pins_TypeDef *p_data_bus) {
	p_lcd->RS = p_rs;
	p_lcd->RW = p_rw;
	p_lcd->E = p_e;

	p_lcd->DATABUS->D4 = p_data_bus->D4;
	p_lcd->DATABUS->D5 = p_data_bus->D5;
	p_lcd->DATABUS->D6 = p_data_bus->D6;
	p_lcd->DATABUS->D7 = p_data_bus->D7;

	if (bit_mode == 0x10) {
		fl_display_function = FL_FUNCTIONSET_8BITMODE | FL_FUNCTIONSET_ONELINE | FL_FUNCTIONSET_5x8MODE;

		p_lcd->DATABUS->D0 = p_data_bus->D0;
		p_lcd->DATABUS->D1 = p_data_bus->D1;
		p_lcd->DATABUS->D2 = p_data_bus->D2;
		p_lcd->DATABUS->D3 = p_data_bus->D3;

	} else {
		// 4-Bit mode is the default
		fl_display_function = FL_FUNCTIONSET_4BITMODE | FL_FUNCTIONSET_ONELINE | FL_FUNCTIONSET_5x8MODE;

		p_lcd->DATABUS->D0 = NULL;
		p_lcd->DATABUS->D1 = NULL;
		p_lcd->DATABUS->D2 = NULL;
		p_lcd->DATABUS->D3 = NULL;
	}

	curr_line = lines;

	// One line is the default
	if (curr_line == LCD_MAX_LINES) {
		fl_display_function |= FL_FUNCTIONSET_TWOLINE;
	} else {
		fl_display_function |= FL_FUNCTIONSET_ONELINE;
	}

	// Follow the "initialization by instruction" method specified in the HD44780U LCD datasheet
	// Assume the power supply conditions for correctly operating the internal reset circuit have not been met

	// After VCC rises to 2.7V, wait for more than 40 ms
	HAL_Delay(50);

	// Pull-down both RS and RW to write commands
	HAL_GPIO_WritePin(p_lcd->RS->GPIO_PERIPHERAL, p_lcd->RS->GPIO_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(p_lcd->RW->GPIO_PERIPHERAL, p_lcd->RW->GPIO_PIN, GPIO_PIN_RESET);

	if (fl_display_function & FL_FUNCTIONSET_4BITMODE) {
		write_4bits(p_lcd, 0x03);
		HAL_Delay(5);				// Wait for more than 4.1 ms
		write_4bits(p_lcd, 0x03);
		HAL_Delay(1);				// Wait for more than 1 us
		write_4bits(p_lcd, 0x03);

		// Initialize to 4-bit interface
		write_4bits(p_lcd, 0x02);

	} else {
		write_8bits(p_lcd, 0x30);
		HAL_Delay(5);				// Wait for more than 4.1 ms
		write_8bits(p_lcd, 0x30);
		HAL_Delay(1);				// Wait for more than 1 us
		write_8bits(p_lcd, 0x30);
	}

	// Function set: specify number of display lines, character font. Override mode bit mode setting
	send_command(p_lcd, CMD_FUNCTIONSET | fl_display_function);

	// Display Control: set display ON, blinking and cursor OFF by default
	fl_display_control = FL_DISPLAYCONTROL_DISPLAYON | FL_DISPLAYCONTROL_CURSOROFF | FL_DISPLAYCONTROL_BLINKOFF;
	send_command(p_lcd, CMD_DISPLAYCONTROL | fl_display_control);

	// Clear the display
	Display_Clear_LCD1602A(p_lcd);

	// Entry Mode: set default text direction as left-to-right
	fl_display_entry_mode = FL_ENTRYMODESET_SHIFTLEFT | FL_ENTRYMODESET_DDRAMDECR;
	send_command(p_lcd, CMD_ENTRYMODESET | fl_display_entry_mode);
}

void Home_Return_LCD1602A(LCD_TypeDef *p_lcd) {
	send_command(p_lcd, CMD_RETURNHOME);
	curr_line = 0;
	curr_col = 0;
	HAL_Delay(2);
}

void Display_Clear_LCD1602A(LCD_TypeDef *p_lcd) {
	send_command(p_lcd, CMD_CLEARDISPLAY);
	HAL_Delay(2);
}

void Display_Off_LCD1602A(LCD_TypeDef *p_lcd) {
	fl_display_control &= ~(FL_DISPLAYCONTROL_DISPLAYON);
	send_command(p_lcd, CMD_DISPLAYCONTROL | fl_display_control);
}

void Display_On_LCD1602A(LCD_TypeDef *p_lcd) {
	fl_display_control |= FL_DISPLAYCONTROL_DISPLAYON;
	send_command(p_lcd, CMD_DISPLAYCONTROL | fl_display_control);
}

void Display_ShiftLeft_LCD1602A(LCD_TypeDef *p_lcd) {
	send_command(p_lcd, CMD_SHIFT | FL_SHIFT_DISPLAY | FL_SHIFT_LEFT);
}

void Display_ShiftRight_LCD1602A(LCD_TypeDef *p_lcd) {
	send_command(p_lcd, CMD_SHIFT | FL_SHIFT_DISPLAY | FL_SHIFT_RIGHT);
}

void Cursor_Off_LCD1602A(LCD_TypeDef *p_lcd) {
	fl_display_control &= ~(FL_DISPLAYCONTROL_CURSORON);
	send_command(p_lcd, CMD_DISPLAYCONTROL | fl_display_control);
}

void Cursor_On_LCD1602A(LCD_TypeDef *p_lcd) {
	fl_display_control |= FL_DISPLAYCONTROL_CURSORON;
	send_command(p_lcd, CMD_DISPLAYCONTROL | fl_display_control);
}

void Cursor_Set_LCD1602A(LCD_TypeDef *p_lcd, uint8_t line, uint8_t column) {
	uint8_t tmp = 0;
	column = column % 16;
	tmp = column;

	if (line == 1) {
		tmp += 0x40;
	}

	send_command(p_lcd, CMD_DDRAMADDRSET | tmp);
	HAL_Delay(1);

	curr_line = line;
	curr_col = column;
}

void Cursor_ShiftLeft_LCD1602A(LCD_TypeDef *p_lcd) {
	send_command(p_lcd, CMD_SHIFT | FL_SHIFT_CURSOR | FL_SHIFT_LEFT);
}

void Cursor_ShiftRight_LCD1602A(LCD_TypeDef *p_lcd) {
	send_command(p_lcd, CMD_SHIFT | FL_SHIFT_CURSOR | FL_SHIFT_RIGHT);
}

void Cursor_JustifyLeft_LCD1602A(LCD_TypeDef *p_lcd) {
	fl_display_entry_mode |= FL_ENTRYMODESET_DDRAMINCR;
	send_command(p_lcd, CMD_ENTRYMODESET | fl_display_entry_mode);
}

void Cursor_JustifyRight_LCD1602A(LCD_TypeDef *p_lcd) {
	fl_display_entry_mode |= FL_ENTRYMODESET_DDRAMDECR;
	send_command(p_lcd, CMD_ENTRYMODESET | fl_display_entry_mode);
}

void Blink_Off_LCD1602A(LCD_TypeDef *p_lcd) {
	fl_display_control &= ~(FL_DISPLAYCONTROL_BLINKON);
	send_command(p_lcd, CMD_DISPLAYCONTROL | fl_display_control);
}

void Blink_On_LCD1602A(LCD_TypeDef *p_lcd) {
	fl_display_control |= FL_DISPLAYCONTROL_BLINKON;
	send_command(p_lcd, CMD_DISPLAYCONTROL | fl_display_control);
}

void Write_Char_LCD1602A(LCD_TypeDef *p_lcd, uint8_t ch) {
	send_data(p_lcd, ch);
	HAL_Delay(1);

	// Move cursor forward
	if (curr_line == 0 && curr_col == LCD_MAX_COLUMNS) {
		// Move to the next line (row)
		Cursor_Set_LCD1602A(p_lcd, 1, 0);
	} else if (curr_line == LCD_MAX_LINES - 1 && curr_col == LCD_MAX_COLUMNS - 1) {
		// Wrap back and go to Home
		Cursor_Set_LCD1602A(p_lcd, 0, 0);
	} else {
		// Advance to the next column on the same line
		Cursor_Set_LCD1602A(p_lcd, curr_line, curr_col + 1);
	}
}

void Write_String_LCD1602A(LCD_TypeDef *p_lcd, uint8_t *str, uint8_t str_size) {
	for (uint8_t i = 0; i < str_size; ++i) {
		Write_Char_LCD1602A(p_lcd, str[i]);
	}
}
