#include <stdint.h>
#include <avr/io.h>

#include "Config/DancePadConfig.h"

// see page 308 of https://cdn.sparkfun.com/datasheets/Dev/Arduino/Boards/ATMega32U4.pdf for these
static const uint8_t sensorToAnalogPin[SENSOR_COUNT] = {
    0b111111,
    0b111111,
    
	0b000100,
    0b000101,
    0b000110,
    0b000111,
    
	0b111111,
    0b111111,
    0b111111,
    0b111111,
    0b111111,
    0b111111
};

void ADC_Init(void) {
    // different prescalers change conversion speed. tinker! 111 is slowest, and not fast enough for many sensors.
    const uint8_t prescaler = (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);

    ADCSRA = (1 << ADEN) | prescaler;
    ADMUX = (1 << REFS0); // analog reference = 5V VCC
    ADCSRB = (1 << ADHSM); // enable high speed mode
}

uint16_t ADC_Read(uint8_t sensor) {
    uint8_t pin = sensorToAnalogPin[sensor];
	if(pin == 0b111111) {
		return 0;
	}

    // see: https://www.avrfreaks.net/comment/885267#comment-885267
    ADMUX = (ADMUX & 0xE0) | (pin & 0x1F); // select channel (MUX0-4 bits)
    ADCSRB = (ADCSRB & 0xDF) | (pin & 0x20); // select channel (MUX5 bit) 

    ADCSRA |= (1 << ADSC); // start conversion
    while (ADCSRA & (1 << ADSC)) {}; // wait until done

    return ADC;
}
