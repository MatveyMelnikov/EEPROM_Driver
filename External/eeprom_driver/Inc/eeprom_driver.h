#ifndef EEPROM_DRIVER_H
#define EEPROM_DRIVER_H

// #include "stm32f1xx.h"
// #include "stm32f1xx_hal_i2c.h"
#include <stdint.h>
#include "eeprom_defs.h"

eeprom_status eeprom_byte_write(
    uint16_t addr,
    uint8_t data
);

eeprom_status eeprom_page_write(
    uint16_t addr,
    const uint8_t *const data,
    uint16_t size
);

eeprom_status eeprom_current_address_read(
    uint8_t *const data
);

eeprom_status EEPROM_random_read(
    uint16_t addr,
    uint8_t *const data
);

eeprom_status eeprom_sequential_read(
    uint8_t data
);

eeprom_status eeprom_check_link(void);

#endif
