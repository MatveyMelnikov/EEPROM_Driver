#include "unity_fixture.h"
#include "eeprom_driver.h"
#include "eeprom_io.h"

// Static functions ----------------------------------------------------------

static void check_data(
    const uint8_t *const expected,
    const uint8_t *const actual,
    const uint16_t data_size
)
{
    for (uint16_t i = 0; i < data_size; i++)
        TEST_ASSERT_BYTES_EQUAL(expected[i], actual[i]);
}

// Tests ---------------------------------------------------------------------

TEST_GROUP(eeprom_driver);

TEST_SETUP(eeprom_driver)
{
}

TEST_TEAR_DOWN(eeprom_driver)
{
}

TEST(eeprom_driver, check_link_success)
{
    eeprom_status status = eeprom_check_link();

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
}

TEST(eeprom_driver, random_write_and_read_byte_success)
{
    uint8_t received_data = 0x0U;
    uint16_t addr = 0x0a55;

    eeprom_status status = eeprom_byte_write(addr, 0x54);
    status |= eeprom_random_byte_read(addr, &received_data);

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    TEST_ASSERT_BYTES_EQUAL(0x54, received_data);
}

TEST(eeprom_driver, current_address_read_byte_success)
{
    uint8_t result_data = 0x0;
    uint16_t addr = 0x45;

    eeprom_status status = eeprom_byte_write(addr, 0x54);
    status |= eeprom_addr_write(addr);
    status |= eeprom_current_address_read(&result_data);

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    TEST_ASSERT_EQUAL(0x54, result_data);
}

TEST(eeprom_driver, multiple_current_address_read_byte_success)
{
    uint8_t data[4] = { 0x54, 0xaa, 0x01, 0x02 };
    uint8_t result_data[4];
    uint16_t addr = 0x55;

    eeprom_status status = eeprom_byte_write(addr, data[0]);
    status |= eeprom_byte_write(addr + 1, data[1]);
    status |= eeprom_byte_write(addr + 2, data[2]);
    status |= eeprom_byte_write(addr + 3, data[3]);

    status |= eeprom_addr_write(addr);
    status |= eeprom_current_address_read(result_data);
    status |= eeprom_current_address_read(result_data + 1);
    status |= eeprom_current_address_read(result_data + 2);
    status |= eeprom_current_address_read(result_data + 3);

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    check_data(data, result_data, 4);
}

TEST(eeprom_driver, page_8_sequential_read_success)
{
    uint16_t addr = 0x44;
    uint8_t test_data[] = { 0x55, 0x01, 0xaa, 0x02 };
    uint8_t data[4];

    eeprom_status status = EEPROM_OK;
    for (uint8_t i = 0; i < 4; i++)
        status |= eeprom_byte_write(addr + i, test_data[i]);

    status |= eeprom_addr_write(addr);
    status |= eeprom_sequential_read(data, 4);

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    check_data(test_data, data, 4);
}

TEST(eeprom_driver, page_68_sequential_read_success)
{
    uint16_t addr = 0x68;
    uint8_t test_data[] = { 0x55, 0x01, 0xaa, 0x02 };
    uint8_t data[68];

    eeprom_status status = EEPROM_OK;
    for (uint8_t i = 0; i < 4; i++)
        status |= eeprom_byte_write(addr + i, test_data[i]);

    status |= eeprom_byte_write(addr + 60, 0x47);

    status |= eeprom_addr_write(addr);
    status |= eeprom_sequential_read(data, 68);

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    check_data(test_data, data, 4);
    TEST_ASSERT_BYTES_EQUAL(0x47, data[60]);
}

TEST(eeprom_driver, page_128_sequential_read_success)
{
    uint16_t addr = 0x75;
    uint8_t test_data[] = { 0x34, 0x02, 0x55, 0xaa };
    uint8_t data[128];

    eeprom_status status = EEPROM_OK;
    for (uint8_t i = 0; i < 4; i++)
        status |= eeprom_byte_write(addr + i, test_data[i]);

    status |= eeprom_byte_write(addr + 60, 0x26);
    status |= eeprom_byte_write(addr + 111, 0x45);

    status |= eeprom_addr_write(addr);
    status |= eeprom_sequential_read(data, 128);

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    check_data(test_data, data, 4);
    TEST_ASSERT_BYTES_EQUAL(0x26, data[60]);
    TEST_ASSERT_BYTES_EQUAL(0x45, data[111]);
}

TEST(eeprom_driver, page_8_write_success)
{
    uint16_t addr = 0x0644;
    uint8_t data[8] = {
        0xff, 0xaa, 0xff, 0x0,
        0x0, 0xff, 0xaa, 0x55
    };
    uint8_t result[8];

    eeprom_status status = eeprom_page_write(addr, data, sizeof(data));

    status |= eeprom_random_byte_read(addr, result);

    status |= eeprom_addr_write(addr);
    status |= eeprom_sequential_read(result, 8);

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    check_data(data, result, 8);
}

TEST(eeprom_driver, page_68_write_success)
{
    uint16_t addr = 0x0755;
    uint8_t data[68] = {
        0xff, 0xaa, 0xff, 0x0,
        0x0, 0xff, 0xaa, 0x55
    };
    memcpy(data + 60, data, 8);
    memcpy(data + 30, data, 8);
    uint8_t result[68];
    
    eeprom_status status = eeprom_page_write(addr, data, 68);

    status |= eeprom_addr_write(addr);
    status |= eeprom_sequential_read(result, 68);

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    check_data(data, result, 68);
}

TEST(eeprom_driver, page_128_write_success)
{
    uint16_t addr = 0x0155;
    uint8_t data[128] = {
        0x33, 0x44, 0x66, 0x77,
        0x0, 0xaa, 0xaa, 0x55
    };
    memcpy(data + 120, data, 8);
    memcpy(data + 90, data, 8);
    memcpy(data + 60, data, 8);
    uint8_t result[128];
    
    eeprom_status status = eeprom_page_write(addr, data, sizeof(data));
    
    status |= eeprom_addr_write(addr);
    status |= eeprom_sequential_read(result, 128);

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    check_data(data, result, 128);
}

TEST(eeprom_driver, page_133_write_success)
{
    uint16_t addr = 0x0178;
    uint8_t data[133] = {
        0x22, 0x11, 0x66, 0x55
    };
    memcpy(data + 129, data, 4);
    memcpy(data + 100, data, 4);
    memcpy(data + 65, data, 4);
    uint8_t result[133];
    
    eeprom_status status = eeprom_page_write(addr, data, sizeof(data));
    
    status |= eeprom_addr_write(addr);
    status |= eeprom_sequential_read(result, 133);

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    check_data(data, result, 133);
}
