#ifndef EEPROM_DEFS_H
#define EEPROM_DEFS_H

// #define EEPROM_TIMEOUT 0x64U

// typedef enum
// {
//     WRITE_TIMEOUT_1_8_V = 0x14U,
//     WRITE_TIMEOUT_2_7_V = 0xaU,
//     WRITE_TIMEOUT_5_V = 0xaU,
// } EEPROM_write_cycle_time;

enum
{
    EEPROM_TIMEOUT = 0x64U
};

#define DEV_ADDR 0xA0U
#define VOLTAGE_1_8_V

#ifdef VOLTAGE_1_8_V
#define WRITE_CYCLE_TIMEOUT 0x14U
#else
#define WRITE_CYCLE_TIMEOUT 0xaU
#endif

typedef enum
{
    EEPROM_OK = 0x00U,
    EEPROM_ERROR = 0x01U,
    EEPROM_BUSY = 0x02U,
    EEPROM_TIMEOUT_ERROR = 0x03U
} eeprom_status;

#endif
