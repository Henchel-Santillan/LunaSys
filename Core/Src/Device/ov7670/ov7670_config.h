#ifndef LUNA_SYS_OV7670_OV7670CONFIG_H
#define LUNA_SYS_OV7670_OV7670CONFIG_H

/*
 * Configuration file for the OV7670 device
 * Defines device-specific settings
 *
 * */


//*** PPL, LINES ***//

#define QVGA_PPL 320
#define QVGA_LINES 480


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

	// Matrix coefficients.
	{ 0x4F, 0x80 }, // MTX1 +
	{ 0x50, 0x80 }, // MTX2 -
	{ 0x51, 0x00 }, // MTX3 -
	{ 0x52, 0x22 }, // MTX4 -
	{ 0x53, 0x5E }, // MTX5 -
	{ 0x54, 0x80 }, // MTX6 +

	// MTXS: Enable auto contrast center (CONTRAS-CENTER)
	// Set matrix coefficient signs (+-)
	{ 0x58, 0x9E },

	// Gamma curve values
	{ 0x7A, 0x20 }, // SLOP: Gamma curve highest segment slope
	{ 0x7B, 0x10 }, // GAM1: Gamma curve 1st segment input end point 0x04 output value
	{ 0x7C, 0x1E }, // GAM2: Gamma curve 2nd segment input end point 0x08 output value
	{ 0x7D, 0x35 }, // GAM3: Gamma curve 3rd segment input end point 0x10 output value
	{ 0x7E, 0x5A }, // GAM4: Gamma curve 4th segment input end point 0x20 output value
	{ 0x7F, 0x69 }, // GAM5: GC 5th SIEP 0x28 OV
	{ 0x80, 0x76 }, // GAM6: GC 6th SIEP 0x30 OV
	{ 0x81, 0x80 }, // GAM7: GC 7th SIEP 0x38 OV
	{ 0x82, 0x88 }, // GAM8: GC 8th SIEP 0x40 OV
	{ 0x83, 0x8F }, // GAM9: GC 9th SIEP 0x48 OV
	{ 0x84, 0x96 }, // GAM10: GC 10th SIEP 0x50 OV
	{ 0x85, 0xA3 }, // GAM11: GC 11th SIEP 0x60 OV
	{ 0x86, 0xAF }, // GAM12: GC 12th SIEP 0x70 OV
	{ 0x87, 0xC4 }, // GAM13: GC 13th SIEP 0x90 OV
	{ 0x88, 0xD7 }, // GAM14: GC 14th SIEP 0xB0 OV
	{ 0x89, 0xE8 }, // GAM15: GC 15th SIEP 0xD0 OV

	// Mark end of configuration
	{ REG_RANGE, REG_RANGE }
};

#endif // LUNA_SYS_OV7670_OV7670CONFIG_H
