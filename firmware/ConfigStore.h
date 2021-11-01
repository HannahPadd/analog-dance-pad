#ifndef _CONFIGSTORE_H_
#define _CONFIGSTORE_H_
    #include "Pad.h"
	#include "Lights.h"
	#include "ADC.h"

    #define MAX_NAME_SIZE 50

    typedef struct {
        uint8_t size;
        char name[MAX_NAME_SIZE];
    } __attribute__((packed)) NameAndSize; 

    typedef struct {
        PadConfiguration padConfiguration;
        NameAndSize nameAndSize;
		LightConfiguration lightConfiguration;
		AdcConfiguration adcConfiguration;
    } __attribute__((packed)) Configuration;
	
    void ConfigStore_LoadConfiguration(Configuration* conf);
    void ConfigStore_StoreConfiguration(const Configuration* conf);
    void ConfigStore_FactoryDefaults(Configuration* conf);
#endif
