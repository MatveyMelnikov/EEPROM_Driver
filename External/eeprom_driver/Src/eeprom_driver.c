#include "eeprom_driver.h"
#include "eeprom_defs.h"
#include "eeprom_io.h"
#include "eeprom_time.h"
#include <stdint.h>
#include <string.h>

// Static functions ----------------------------------------------------------

static uint16_t invert_address(uint16_t addr)
{
    return (addr << 8) | (addr >> 8);
}

// The data word address lower 6 bits are internally incremented
// following the receipt of each data word
// (ATMEL AT24C8C128 datasheet pg. 7).
static uint16_t get_num_of_start_remaining_bytes(
    const uint16_t addr,
    uint16_t size
)
{
    //uint16_t result = 0x3f - (addr & 0x3f);
    //uint16_t result = 0x40 - (addr & 0x3f);
    uint8_t a = addr & 0x3f;
    uint16_t result = 0x40 - a;
    if (result > size)
        result = size;

    return result;
}

static eeprom_status send_page(
    const uint16_t addr,
    const uint8_t *const data,
    uint16_t data_size,
    uint8_t *const buffer
)
{
    uint16_t inverted_addr = invert_address(addr);

    memcpy(buffer, (uint8_t*)&inverted_addr, sizeof(inverted_addr));
    memcpy(buffer + 2, data, data_size); // first 2 bytes - addr
    eeprom_status status = eeprom_io_write(buffer, data_size + 2);
    eeprom_delay(WRITE_CYCLE_TIMEOUT);

    return status;
}

// The data word address lower 6 bits are internally incremented
// following the receipt of each data word
// (ATMEL AT24C8C128 datasheet pg. 7).
// We send the data and add padding to the address in which the
// data is sent and to the pointer to the data
static eeprom_status send_start_bytes(
    uint16_t *current_addr,
    uint8_t **current_data_ptr,
    const uint16_t size,
    uint16_t *const remaining_size,
    uint8_t *const buffer
)
{
    uint16_t start_bytes_num = get_num_of_start_remaining_bytes(
        *current_addr,
        size
    );
    eeprom_status status = send_page(
        *current_addr,
        *current_data_ptr,
        start_bytes_num,
        buffer
    );

    *remaining_size -= start_bytes_num;
    *current_addr += start_bytes_num;
    *current_data_ptr += start_bytes_num;

    return status;
}

// Sending an incomplete page to align the data to 64 bytes.
// We send the data and add padding to the address in which the
// data is sent and to the pointer to the data
static eeprom_status send_offset_bytes(
    uint16_t *current_addr,
    uint8_t **current_data_ptr,
    uint16_t *const remaining_size,
    uint8_t *const buffer
)
{
    uint16_t offset = *remaining_size % 64;

    if (offset == 0)
        return EEPROM_OK;
    
    eeprom_status status = send_page(
        *current_addr,
        *current_data_ptr,
        offset,
        buffer
    );

    *remaining_size -= offset;
    *current_addr += offset;
    *current_data_ptr += offset;

    return status;
}

// Implementations -----------------------------------------------------------

eeprom_status eeprom_addr_write(
    uint16_t addr
)
{
    uint16_t inverted_address = invert_address(addr);
    return eeprom_io_write((uint8_t*)&inverted_address, sizeof(addr));
}

eeprom_status eeprom_byte_write(
    uint16_t addr,
    uint8_t data
)
{
    struct 
    {
        uint16_t addr;
        uint8_t data;
    } sent_data;

    sent_data.addr = invert_address(addr);
    sent_data.data = data;

    eeprom_status status = eeprom_io_write(
        (uint8_t*)&sent_data,
        3
    );
    eeprom_delay(WRITE_CYCLE_TIMEOUT);

    return status;
}

eeprom_status eeprom_page_write(
    const uint16_t addr,
    const uint8_t *const data,
    uint16_t size
)
{
    // uint16_t current_addr = addr;
    // uint8_t *current_data_ptr = data;
    // struct 
    // {
    //     uint16_t addr;
    //     uint8_t data[64];
    // } sent_page;

    // sent_page.addr = invert_address(addr);

    // // Divide the data into parts of 64 bytes 
    // // (more than that cannot be written)
    // // (ATMEL AT24C8C128 datasheet pg. 7)
    // eeprom_status status = EEPROM_OK;

    // // The data word address lower 6 bits are internally incremented
    // // following the receipt of each data word
    // // (ATMEL AT24C8C128 datasheet pg. 7)
    // // uint16_t num_of_remaining_bytes = 0x3f - (addr & 0x3f);
    // // memcpy((uint8_t*)&sent_page.data, data, num_of_remaining_bytes);
    // // status |= eeprom_io_write((uint8_t*)&sent_page, num_of_remaining_bytes + 2);
    // // eeprom_delay(WRITE_CYCLE_TIMEOUT);

    // uint16_t remaining_bytes = get_num_of_start_remaining_bytes(addr, size);
    // status |= send_page(
    //     current_addr, current_data_ptr, remaining_bytes, (uint8_t*)&sent_page
    // );

    // if (remaining_bytes == size)
    


    // uint16_t offset = size % 64;

    // // Send the partial page first and align the data
    // if (offset != 0)
    // {
    //     memcpy(
    //         (uint8_t*)&sent_page.data + num_of_remaining_bytes,
    //         data,
    //         offset
    //     );
    //     status |= eeprom_io_write((uint8_t*)&sent_page, offset + 2);
    //     eeprom_delay(WRITE_CYCLE_TIMEOUT);
    // }

    // for (uint16_t i = 0; i < (size / 64); i++)
    // {
    //     memcpy(
    //         (uint8_t*)&sent_page.data,
    //         data + offset + (i * 64),
    //         64
    //     );

    //     status |= eeprom_io_write((uint8_t*)&sent_page, 66);
    //     eeprom_delay(WRITE_CYCLE_TIMEOUT);
    // }

    uint16_t current_addr = addr;
    uint8_t *current_data_ptr = (uint8_t*)data;
    uint16_t remaining_size = size;

    // struct 
    // {
    //     uint16_t addr;
    //     uint8_t data[64];
    // } sent_page;
    uint8_t buffer[66];

    // Divide the data into parts of 64 bytes 
    // (more than that cannot be written)
    // (ATMEL AT24C8C128 datasheet pg. 7)
    eeprom_status status = EEPROM_OK;

    // The data word address lower 6 bits are internally incremented
    // following the receipt of each data word
    // (ATMEL AT24C8C128 datasheet pg. 7)
    // uint16_t num_of_remaining_bytes = 0x3f - (addr & 0x3f);
    // memcpy((uint8_t*)&sent_page.data, data, num_of_remaining_bytes);
    // status |= eeprom_io_write((uint8_t*)&sent_page, num_of_remaining_bytes + 2);
    // eeprom_delay(WRITE_CYCLE_TIMEOUT);

    status |= send_start_bytes(
        &current_addr,
        &current_data_ptr,
        size,
        &remaining_size,
        buffer
    );
    
    if (remaining_size == 0)
        return status;

    // uint16_t offset = size % 64;

    // // Send the partial page first and align the data
    // if (offset != 0)
    // {
    //     memcpy(
    //         (uint8_t*)&sent_page.data + num_of_remaining_bytes,
    //         data,
    //         offset
    //     );
    //     status |= eeprom_io_write((uint8_t*)&sent_page, offset + 2);
    //     eeprom_delay(WRITE_CYCLE_TIMEOUT);
    // }

    status |= send_offset_bytes(
        &current_addr,
        &current_data_ptr,
        &remaining_size,
        buffer
    );

    for (uint16_t i = 0; i < (remaining_size / 64); i++)
    {
        // memcpy(
        //     (uint8_t*)&sent_page.data,
        //     data + offset + (i * 64),
        //     64
        // );

        // status |= eeprom_io_write((uint8_t*)&sent_page, 66);
        // eeprom_delay(WRITE_CYCLE_TIMEOUT);
        send_page(
            current_addr,
            current_data_ptr,
            64,
            buffer
        );
        current_addr += 64;
        current_data_ptr += 64;
    }

    return status;
}

eeprom_status eeprom_current_address_read(
    uint8_t *const data
)
{
    return eeprom_io_read(data, 1);
}

eeprom_status eeprom_random_byte_read(
    const uint16_t addr,
    uint8_t *const data
)
{
    eeprom_status status = eeprom_addr_write(addr);
    status |= eeprom_io_read(data, 1);

    return status;
}

// Before this function you need to specify the address with another command
eeprom_status eeprom_sequential_read(
    uint8_t *const data,
    const uint16_t size
)
{
    return eeprom_io_read(data, size);
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
