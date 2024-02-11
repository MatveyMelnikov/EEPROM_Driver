#include "eeprom_driver.h"
#include "eeprom_defs.h"
#include "eeprom_io.h"
#include "eeprom_time.h"
#include <stdint.h>
#include <string.h>

// HAL_StatusTypeDef eeprom_byte_write(
//     const I2C_HandleTypeDef *const i2c,
//     uint16_t addr,
//     uint8_t data
// )
// {
//     const uint8_t sent_data[3] = { 
//         *((uint8_t*)&addr),
//         *((uint8_t*)&addr + 1),
//         data
//     };
//     // memcpy(&sent_data, (uint8_t*)&addr, 2);
//     // memcpy(&sent_data + 2, &data, 1);

//     HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(
//         i2c, DEV_ADDR, &sent_data, 3, EEPROM_TIMEOUT
//     );
//     HAL_Delay(WRITE_CYCLE_TIMEOUT);

//     return status;
// }

// HAL_StatusTypeDef eeprom_page_write(
//     const I2C_HandleTypeDef *const i2c,
//     uint16_t addr,
//     const uint8_t *const data,
//     uint16_t size
// )
// {
//     struct 
//     {
//         uint16_t addr;
//         uint8_t part[64];
//     } sent_part;

//     sent_part.addr = addr;

//     // Divide the data into parts of 64 bytes 
//     // (more than that cannot be written*)
//     // *AT24C128 datasheet 7 page (page write)
//     HAL_StatusTypeDef status = HAL_OK;
//     for (uint16_t i = 0; i < size / 64; i++)
//     {
//         memcpy((uint8_t*)&sent_part.part, data, 64);

//         status |= HAL_I2C_Master_Transmit(
//             i2c, DEV_ADDR, (uint8_t*)&sent_part, 66, EEPROM_TIMEOUT
//         );        
//     }

//     HAL_Delay(WRITE_CYCLE_TIMEOUT);
// }

// HAL_StatusTypeDef eeprom_current_address_read(
//     const I2C_HandleTypeDef *const i2c,
//     uint8_t *const data
// )
// {

// }

// HAL_StatusTypeDef eeprom_random_read(
//     const I2C_HandleTypeDef *const i2c,
//     uint16_t addr,
//     uint8_t *const data
// )
// {

// }

// HAL_StatusTypeDef EEPROM_sequential_read(
//     const I2C_HandleTypeDef *const i2c,
//     uint8_t data
// )
// {

// }

// HAL_StatusTypeDef eeprom_check_link(const I2C_HandleTypeDef *const i2c)
// {
//     static uint8_t data[] = { 0x0, 0x0, 0x55 };
//     static uint8_t result = 0x0;

//     HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(
//         i2c, DEV_ADDR, &data, 3, EEPROM_TIMEOUT
//     );
//     HAL_Delay(WRITE_CYCLE_TIMEOUT);
    
//     if (status)
//         return status;

//     status = HAL_I2C_Master_Transmit(
//         i2c, DEV_ADDR, &data, 2, EEPROM_TIMEOUT
//     );
//     status |= HAL_I2C_Master_Receive(
//         i2c, DEV_ADDR, &result, 1, EEPROM_TIMEOUT
//     );

//     if (result != 0x55)
//         status |= HAL_ERROR;
//     return status;
// }

// Implementations -----------------------------------------------------------

eeprom_status eeprom_byte_write(
    uint16_t addr,
    uint8_t data
)
{
    const uint8_t sent_data[] = { 
        *((uint8_t*)&addr),
        *((uint8_t*)&addr + 1),
        data
    };

    eeprom_status status = eeprom_io_write(
        (uint8_t*)&sent_data,
        sizeof(sent_data)
    );
    eeprom_delay(WRITE_CYCLE_TIMEOUT);

    return status;
}

eeprom_status eeprom_page_write(
    uint16_t addr,
    const uint8_t *const data,
    uint16_t size
)
{
    struct 
    {
        uint16_t addr;
        uint8_t part[64];
    } sent_part;

    sent_part.addr = addr;

    // Divide the data into parts of 64 bytes 
    // (more than that cannot be written)
    // (ATMEL AT24C8C128 datasheet pg. 7)
    eeprom_status status = EEPROM_OK;
    for (uint16_t i = 0; i < size / 64; i++)
    {
        memcpy((uint8_t*)&sent_part.part, data, 64);

        status |= eeprom_io_write((uint8_t*)&sent_part, 66);
    }
    eeprom_delay(WRITE_CYCLE_TIMEOUT);

    return status;
}

eeprom_status eeprom_current_address_read(
    uint8_t *const data
)
{
    return EEPROM_OK;
}

eeprom_status eeprom_random_byte_read(
    const uint16_t addr,
    uint8_t *const data
)
{
    struct 
    {
        uint16_t addr;
        uint8_t dummy;
    } set_addr_cmd = { .addr = addr, .dummy = 0xff };

    eeprom_status status = eeprom_io_write(
        (uint8_t*)&set_addr_cmd,
        sizeof(set_addr_cmd)
    );
    status |= eeprom_io_read(data, 1);

    return status;
}

eeprom_status eeprom_sequential_read(
    uint8_t data
)
{
    return EEPROM_OK;
}

eeprom_status eeprom_check_link(void)
{
    static uint8_t data[] = { 0x0, 0x0, 0x55 };
    static uint8_t result = 0x0;

    eeprom_status status = eeprom_io_write(&data, sizeof(data));
    eeprom_delay(WRITE_CYCLE_TIMEOUT);
    
    if (status)
        return status;

    status = eeprom_io_write(&data, 2);
    status |= eeprom_io_read(&result, 1);

    if (result != 0x55)
        status |= EEPROM_ERROR;
    return status;
}
