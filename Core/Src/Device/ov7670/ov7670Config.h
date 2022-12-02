#ifndef LUNA_SYS_OV7670_OV7670CONFIG_H
#define LUNA_SYS_OV7670_OV7670CONFIG_H

/*
 * Configuration file for the OV7670 device
 * Defines device-specific settings
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

	// COM16: Edge-enhancement, de-noise for YUV output, and AWB (Auto White Balance) gain enable
	{ 0x41, 0x38 },

	// MVFP: Enable horizontal flip only
	{ 0x1E, 0x20 },

	// Clock-Related and Scaling
	{ 0x0C, 0x04 },	// Enable DCW
	{ 0x70, 0x3A },	// X scaling
	{ 0x71, 0x35 },	// Y scaling
	{ 0x72, 0x11 },	// DCW scaling
	{ 0x73, 0xF0 }, // PCLK_DIV

	// Windowing
	{ 0x11, 0x00 },	// CLKRC: Set F(internal clock) = F(input clock), i.e. pre-scaler = 1:1. Clock source is not divided.
	{ 0x17, 0x17 },	// HSTART
	{ 0x18, 0x04 },	// HSTOP
	{ 0x19, 0x02 }, // VSTART
	{ 0x1A, 0x7B },	// VSTOP
	{ 0x32, 0x80 },	// HREF
	{ 0x03, 0x0A },	// VREF

	// Matrix coefficients TODO FIXME
	{ 0x4f, 0x80 }, //
	{ 0x50, 0x80 }, //
	{ 0x51, 0x00 }, //
	{ 0x52, 0x22 }, //
	{ 0x53, 0x5e }, //
	{ 0x54, 0x80 }, //
	{ 0x58, 0x9e },

	// Gamma curve values TODO FIXME
	{ 0x7a, 0x20 }, //
	{ 0x7b, 0x10 }, //
	{ 0x7c, 0x1e }, //
	{ 0x7d, 0x35 }, //
	{ 0x7e, 0x5a }, //
	{ 0x7f, 0x69 }, //
	{ 0x80, 0x76 }, //
	{ 0x81, 0x80 }, //
	{ 0x82, 0x88 }, //
	{ 0x83, 0x8f }, //
	{ 0x84, 0x96 }, //
	{ 0x85, 0xa3 }, //
	{ 0x86, 0xaf }, //
	{ 0x87, 0xc4 }, //
	{ 0x88, 0xd7 }, //
	{ 0x89, 0xe8 },

	// Mark end of configuration
	{ REG_RANGE, REG_RANGE }
};

#endif // LUNA_SYS_OV7670_OV7670CONFIG_H
