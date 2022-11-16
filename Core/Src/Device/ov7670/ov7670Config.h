#ifndef LUNA_SYS_OV7670_OV7670CONFIG_H
#define LUNA_SYS_OV7670_OV7670CONFIG_H

/*
 * Configuration file for the OV7670 device
 * Defines pin addresses and device-specific settings
 *
 * */


//*** FRAMES, LINES ***//

#define OV7670_VGA_PPL 640		// Pixels per line
#define OV7670_VGA_LINES 480	// Number of lines


//*** CAMERA MODE ***///

#define MODE_CONTINUOUS_GRAB 0
#define MODE_SNAPSHOT 		 1


//*** DEVICE REGISTER LIST ***//

#define REG_RANGE 0xFF
#define DEVADDR_WRITE 0x42	// device address for writing
#define DEVADDR_READ  0x43	// device address for reading

const uint8_t dev_reg_config[][2] = {

	// Disable RGB444, and enable RBG565 with full output range [00] to [FF]
	{ 0x8C, 0x00 },
	{ 0x40, 0xD0 },

	// VGA Output, RGB
	{ 0x12, 0x04 },

	// Configure output sequence as UYVY: closest representation to original available signal, thus good for capturing
	// Gamma enable, UV auto adjust, and UYVY
	{ 0x3A, 0xC },
	{ 0x3D, 0x80 },

	// MVFP: Enable horizontal flip only
	{ 0x1E, 0x20 },

	// CLKRC: Set F(internal clock) = F(input clock), i.e. pre-scaler = 1:1. Clock source is not divided.
	{ 0x11, 0x00 },

	// Mark end of configuration
	{ REG_RANGE, REG_RANGE }
};

#endif // LUNA_SYS_OV7670_OV7670CONFIG_H
