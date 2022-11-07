#ifndef LUNA_SYS_OV7670_OV7670CONFIG_H
#define LUNA_SYS_OV7670_OV7670CONFIG_H


/*
 * Configuration file for the OV7670 device
 * Defines pin addresses and device-specific settings
 *
 * */


//*** OV7670 WIDTH AND HEIGHT CONFIGS ***//
// Note: VGA is 640 x 480. QVGA is a quarter of the area of VGA.

#define OV7670_QVGA_WIDTH  320
#define OV7670_QVGA_HEIGHT 240

//*** CAMERA MODE ***///

#define MODE_CONTINUOUS_GRAB 0
#define MODE_SNAPSHOT 		 1


//*** PINS AND REGISTERS ***//



//***  ***//


#endif // LUNA_SYS_OV7670_OV7670CONFIG_H
