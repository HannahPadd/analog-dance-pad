#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "Config/DancePadConfig.h"

typedef struct
{
    uint8_t red, green, blue;
} __attribute__((packed)) rgb_color;

enum LedMappingFlags
{
    LMF_ENABLED = 0x1,
};

enum LightRuleFlags
{
    LRF_ENABLED  = 0x1,
    LRF_FADE_ON  = 0x2,
    LRF_FADE_OFF = 0x4,
};

typedef struct
{
    uint8_t flags;
    uint8_t lightRuleIndex;
    uint8_t sensorIndex;
    uint8_t ledIndexBegin;
    uint8_t ledIndexEnd;
} __attribute__((packed)) LedMapping;

typedef struct
{
    uint8_t flags;
	rgb_color onColor;
	rgb_color offColor;
	rgb_color onFadeColor;
	rgb_color offFadeColor;
} __attribute__((packed)) LightRule;

typedef struct
{
    LightRule lightRules[MAX_LIGHT_RULES];
    LedMapping ledMappings[MAX_LED_MAPPINGS];
	uint8_t selectedLightRuleIndex;
    uint8_t selectedLedMappingIndex;
} __attribute__((packed)) LightConfiguration;

void Lights_UpdateConfiguration(const LightConfiguration* lightConfiguration);
void Lights_Update(bool force);

extern LightConfiguration LIGHT_CONF;

#endif
