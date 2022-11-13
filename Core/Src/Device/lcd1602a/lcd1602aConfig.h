#ifndef LUNASYS_LCD1602A_LCD1602ACONFIG_H
#define LUNASYS_LCD1602A_LCD1602ACONFIG_H


/*
 * Configuration file for the LCD1602A device
 * Defines pin addresses and device-specific settings
 *
 * */


//**** BASE COMMAND DEFINITIONS ****//

#define CMD_CLEARDISPLAY   0x01
#define CMD_RETURNHOME     0x02
#define CMD_ENTRYMODESET   0x04
#define CMD_DISPLAYCONTROL 0x08
#define CMD_SHIFT          0x10
#define CMD_FUNCTIONSET    0x20
#define CMD_CGRAMADDRSET   0x40
#define CMD_DDRAMADDRSET   0x40


//**** ENTRY MODE SET FLAGS ****//

// DDRAM address increment or decrement (I/D)
#define FL_ENTRYMODESET_DDRAMINCR  0x02
#define FL_ENTRYMODESET_DDRAMDECR  0x00

// Shift of entire display (S)
#define FL_ENTRYMODESET_SHIFTLEFT  0x01
#define FL_ENTRYMODESET_SHIFTRIGHT 0x00


//**** DISPLAY CONTROL FLAGS ****//

// Display ON/OFF control (D)
#define FL_DISPLAYCONTROL_DISPLAYON  0x04
#define FL_DISPLAYCONTROL_DISPLAYOFF 0x00

// Cursor ON/OFF control (C)
#define FL_DISPLAYCONTROL_CURSORON   0x02
#define FL_DISPLAYCONTROL_CURSOROFF  0x00

// Blink ON/OFF control (B)
#define FL_DISPLAYCONTROL_BLINKON    0x01
#define FL_DISPLAYCONTROL_BLINKOFF   0x00


//**** CURSOR AND DISPLAY SHIFT FLAGS ****//

#define FL_SHIFT_CURSOR  0x00
#define FL_SHIFT_DISPLAY 0x08
#define FL_SHIFT_LEFT    0x00
#define FL_SHIFT_RIGHT   0x04


//**** FUNCTION SET FLAGS ****//

// Interface Data Length Control (DL)
#define FL_FUNCTIONSET_8BITMODE 0x10
#define FL_FUNCTIONSET_4BITMODE 0x00

// Display Line Number Control (N)
#define FL_FUNCTIONSET_TWOLINE 0x08
#define FL_FUNCTIONSET_ONELINE 0x00

// Display Font Type Control (F)
#define FL_FUNCTIONSET_5X11MODE 0x04
#define FL_FUNCTIONSET_5x8MODE  0x00


#endif // LUNASYS_LCD1602A_LCD1602ACONFIG_H
