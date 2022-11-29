#include "lcd1602a.h"


/*** STATIC HELPER FUNCTIONS ***/

static void write_4bits(LCD_TypeDef *p_lcd, uint8_t value) {
	// Concerned with the LSB; therefore, write to the lower nibble ("half-byte")
	HAL_GPIO_WritePin(p_lcd->D4->GPIO_PERIPHERAL, p_lcd->D4->GPIO_PIN, (value & 0x08));
	HAL_GPIO_WritePin(p_lcd->D5->GPIO_PERIPHERAL, p_lcd->D5->GPIO_PIN, (value & 0x04));
	HAL_GPIO_WritePin(p_lcd->D6->GPIO_PERIPHERAL, p_lcd->D6->GPIO_PIN, (value & 0x02));
	HAL_GPIO_WritePin(p_lcd->D7->GPIO_PERIPHERAL, p_lcd->D7->GPIO_PIN, (value & 0x01));
}

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

	// Set the LCD to be 8-Bit Mode
	send_command(p_lcd, CMD_FUNCTIONSET | FL_FUNCTIONSET_8BITMODE | FL_FUNCTIONSET_TWOLINE | FL_FUNCTIONSET_5X8MODE);

#else
	// Set the LCD to be 4-Bit Mode
	send_command(p_lcd, CMD_FUNCTIONSET | FL_FUNCTIONSET_4BITMODE | FL_FUNCTIONSET_TWOLINE | FL_FUNCTIONSET_5X8MODE);

#endif

	// Set the Display, Blink, and Cursor to be all ON
	send_command(p_lcd, CMD_DISPLAYCONTROL | FL_DISPLAYCONTROL_DISPLAYON | FL_DISPLAYCONTROL_CURSORON | FL_DISPLAYCONTROL_BLINKON);

	send_command(p_lcd, CMD_ENTRYMODESET | FL_ENTRYMODESET_SHIFTRIGHT | FL_ENTRYMODESET_DDRAMDECR);
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
	write_4bits(p_lcd, data);
#else
	write_8bits(p_lcd, data);
#endif
}
