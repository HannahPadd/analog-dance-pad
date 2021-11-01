#pragma once

#include "stdint.h"
#include <string>
#include <map>

#include "Model/Firmware.h"

namespace adp {

enum DeviceChangeFlags
{
	DCF_DEVICE         = 1 << 0,
	DCF_BUTTON_MAPPING = 1 << 1,
	DCF_NAME           = 1 << 2,
	DCF_LIGHTS         = 1 << 3,
	DCF_ADC			   = 1 << 4
};

typedef int32_t DeviceChanges;

struct RgbColor
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

struct SensorState
{
	double threshold = 0.0;
	double value = 0.0;
	int button = 0; // zero means unmapped.
	bool pressed = false;
};

struct PadState
{
	std::wstring name;
	int maxNameLength = 0;
	int numButtons = 0;
	int numSensors = 0;
	double releaseThreshold = 1.0;
	BoardType boardType = BOARD_UNKNOWN;
	bool featureDebug;
	bool featureDigipot;
};

struct LedMapping
{
	int lightRuleIndex;
	int sensorIndex;
	int ledIndexBegin;
	int ledIndexEnd;
};

struct LightRule
{
	bool fadeOn;
	bool fadeOff;
	RgbColor onColor;
	RgbColor offColor;
	RgbColor onFadeColor;
	RgbColor offFadeColor;
};

struct LightsState
{
	std::map<int, LightRule> lightRules;
	std::map<int, LedMapping> ledMappings;
};

struct AdcState
{
	bool disabled;
	bool setResistor;
	bool aref5;
	bool aref3;
	int resistorValue;
};

class Device
{
public:
	static void Init();

	static void Shutdown();

	static DeviceChanges Update();

	static int PollingRate();

	static const PadState* Pad();

	static const LightsState* Lights();

	static const SensorState* Sensor(int sensorIndex);

	static const AdcState* Adc(int sensorIndex);

	static string ReadDebug();

	static bool SetThreshold(int sensorIndex, double threshold);

	static bool SetReleaseThreshold(double threshold);

	static bool SetButtonMapping(int sensorIndex, int button);

	static bool SetDeviceName(const wchar_t* name);

	static bool SendLedMapping(int ledMappingIndex, LedMapping mapping);

	static bool DisableLedMapping(int ledMappingIndex);

	static bool SendLightRule(int lightRuleIndex, LightRule rule);

	static bool DisableLightRule(int lightRuleIndex);

	static bool SendAdcConfig(int index, AdcState adc);

	static void SendDeviceReset();

	static void SendFactoryReset();
};

}; // namespace adp.