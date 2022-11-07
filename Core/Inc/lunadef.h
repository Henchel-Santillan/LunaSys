#ifndef LUNASYS_INC_LUNADEF_H
#define LUNASYS_INC_LUNADEF_H

#include <stdint.h>


/**
 * Ops_StatusTypeDef: Human readable return values for device operations
 *
 *
 */

typedef enum {

	OPS_OK = 0x00000000,
	OPS_FAIL = 0x00000001,

} Ops_StatusTypeDef;


/**
 * Camera_StatusTypeDef: Camera status feedback
 *
 * CAMERA_OK      	   ==> Camera is both supported and detected. No setup issues encountered.
 * CAMERA_ERR     	   ==> An error was encountered while setting up the camera.
 * CAMERA_TIMEOUT 	   ==> Camera timed out while [...]
 * CAMERA_NOT_DETECTED ==> Camera module cannot be found at the given address
 * CAMERA_UNSUPPORTED  ==> Camera module at the given address is not supported by the MCU architecture
 *
 */

typedef enum {

	CAMERA_OK           = 0x00,
	CAMERA_ERR          = 0x01,
	CAMERA_TIMEOUT      = 0x02,
	CAMERA_NOT_DETECTED = 0x03,
	CAMERA_UNSUPPORTED  = 0x04

} Camera_StatusTypeDef;


#endif // LUNASYS_INC_LUNADEF_H
