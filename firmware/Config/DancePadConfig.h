#ifndef _DANCE_PAD_CONFIG_H_
#define _DANCE_PAD_CONFIG_H_
    //Version 2 since Kauhsa's initial version will be considered version 0
    #define FIRMWARE_VERSION_MAJOR 1
    #define FIRMWARE_VERSION_MINOR 3

	#define FEATURE_DEBUG 1 << 0
	#define FEATURE_DIGIPOT 1 << 1
	#define FEATURE_LIGHTS 1 << 2
	
	//#define FEATURE_DEBUG_ENABLED
	//#define FEATURE_DIGIPOT_ENABLED
	//#define FEATURE_LIGHTS_ENABLED
	
	// Set the board type if not provided to the make command
    // #define BOARD_TYPE_

    #define MAX_SENSOR_VALUE 1024

    // this value doesn't mean we're actively using all these buttons.
    // it's just what we report and is technically possible to use.
    // for now, should be divisible by 8.
    #define BUTTON_COUNT 16

    // this value doesn't mean we're reading all these sensors.
    // teensy 2.0 has 12 analog sensors, so that's what we use.
    #define SENSOR_COUNT 12

    #define MAX_LIGHT_RULES 16
    #define MAX_LED_MAPPINGS 16
	
	#if defined(BOARD_TYPE_FSRMINIPAD_2)
		#define BOARD_TYPE "fsrminipad2";
        #define BOOTLOADER_ADDRESS "0x7000"
		
		#define FEATURE_LIGHTS_ENABLED
		#define FEATURE_DIGIPOT_ENABLED
		
		#define LED_PANELS 4
		#define PANEL_LEDS 8
		
		// Trigger existing exceptions
		#define BOARD_TYPE_FSRMINIPAD
	
    #elif defined(BOARD_TYPE_FSRMINIPAD)
        #define BOARD_TYPE "fsrminipad";
        #define BOOTLOADER_ADDRESS "0x7000"
		
		#define FEATURE_LIGHTS_ENABLED
		
		#define LED_PANELS 4
		#define PANEL_LEDS 8
		
	#elif defined(BOARD_TYPE_FSRIO_1)
        #define BOARD_TYPE "fsrio1";
        #define BOOTLOADER_ADDRESS "0x7000"
		
		#define FEATURE_LIGHTS_ENABLED
		#define FEATURE_DIGIPOT_ENABLED
		
		#define LED_PANELS 8
		#define PANEL_LEDS 8

    #elif defined(BOARD_TYPE_TEENSY2)
    	#define BOARD_TYPE "teensy2";
    	#define BOOTLOADER_ADDRESS "0x7E00"

    #else
    	// Assuming generic atmega32u4 board like the arduino leonardo or pro micro
    	#define BOARD_TYPE "leonardo";
    	#define BOOTLOADER_ADDRESS "0x7000"
		
		#warning "No board type defined, using generic leonardo"
		#pragma message ( "No board type defined, using generic leonardo" )

    #endif
	
	#if defined(FEATURE_LIGHTS_ENABLED)
		#define LED_COUNT (LED_PANELS * PANEL_LEDS)
	#else
		#define LED_COUNT 0
	#endif
#endif
