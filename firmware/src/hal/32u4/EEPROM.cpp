#include "hal/hal_EEPROM.hpp"
#include <util/atomic.h>
#include <avr/eeprom.h>

void HAL_EEPROM_Init()
{
}

void HAL_EEPROM_Write(uint16_t address, uint8_t* buffer, uint16_t length)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        eeprom_update_block((void*)buffer, (void*)address, (size_t)length);
    }
}

void HAL_EEPROM_Read(uint16_t address, uint8_t* buffer, uint16_t length)
{
    eeprom_read_block((void*)buffer, (void*)address, (size_t)length);
}