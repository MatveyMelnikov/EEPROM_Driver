#include "unity_fixture.h"
#include "eeprom_driver.h"
#include "mock_eeprom_io.h"
#include <string.h>

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
    mock_eeprom_io_create(10);
    //eeprom_driver_create(void);
}

TEST_TEAR_DOWN(eeprom_driver)
{
    //eeprom_driver_destroy(void);
    mock_eeprom_io_verify_complete();
    mock_eeprom_io_destroy();
}

TEST(eeprom_driver, check_link_success)
{
    uint8_t output_data[] = { 0x0, 0x0, 0x55 };
    uint8_t input_data = 0x55;
    mock_eeprom_io_expect_write(output_data, 3); // 0x0, 0x0, 0x55
    mock_eeprom_io_expect_write(output_data, 2); // 0x0, 0x0
    mock_eeprom_io_expect_read_then_return(&input_data, 1);

    eeprom_status status = eeprom_check_link();

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
}

TEST(eeprom_driver, random_write_byte_success)
{
    uint8_t output_data[] = { 0xaa, 0x55, 0x54 };
    uint8_t input_data = 0x54;

    mock_eeprom_io_expect_write(output_data, 3); // 0xaa, 0x55, 0x54

    eeprom_status status = eeprom_byte_write(0xaa55, 0x54);

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
}

TEST(eeprom_driver, random_read_byte_success)
{
    uint8_t output_data[] = { 0xaa, 0x55 };
    uint8_t input_data = 0x55;
    uint8_t result_data = 0x0;

    mock_eeprom_io_expect_write(output_data, 2); // 0xaa, 0x55 (address)
    mock_eeprom_io_expect_read_then_return(&input_data, 1);

    eeprom_status status = eeprom_random_byte_read(0xaa55, &result_data);

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    TEST_ASSERT_EQUAL(input_data, result_data);
}

TEST(eeprom_driver, current_address_read_byte_success)
{
    uint8_t input_data = 0x55;
    uint8_t result_data = 0x0;

    mock_eeprom_io_expect_read_then_return(&input_data, 1);

    eeprom_status status = eeprom_current_address_read(&result_data);

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    TEST_ASSERT_EQUAL(input_data, result_data);
}

TEST(eeprom_driver, page_8_write_success)
{
    uint8_t data[8] = {
        0xff, 0xaa, 0xff, 0x0,
        0x0, 0xff, 0xaa, 0x55
    };
    uint8_t output_data[10] = { 0x55, 0xaa };
    memcpy(output_data + 2, data, sizeof(data));
    
    mock_eeprom_io_expect_write(output_data, sizeof(output_data));

    eeprom_status status = eeprom_page_write(0x55aa, data, sizeof(data));

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
}

TEST(eeprom_driver, page_68_write_success)
{
    // uint8_t data[68] = {
    //     0xff, 0xaa, 0xff, 0x0,
    //     0x0, 0xff, 0xaa, 0x55
    // };
    // memset(data + 8, 0xaa, 60);
    // uint8_t output_data_0[6] = { 0x55, 0xaa };
    // memcpy(output_data_0 + 2, data, 4);
    // uint8_t output_data_1[66] = { 0x55, 0xaa };
    // memcpy(output_data_1 + 2, data + 4, sizeof(data) - 4);
    
    // mock_eeprom_io_expect_write(output_data_0, sizeof(output_data_0));
    // mock_eeprom_io_expect_write(output_data_1, sizeof(output_data_1));
    
    // eeprom_status status = eeprom_page_write(0x55aa, data, sizeof(data));

    // TEST_ASSERT_EQUAL(EEPROM_OK, status);

    uint8_t data[68];
    for (uint8_t i = 0; i < 68; i++)
        data[i] = i;

    uint8_t output_data_0[2 + 42] = { 0x55, 0xaa };
    memcpy(output_data_0 + 2, data, 42);
    uint8_t output_data_1[2 + 26] = { 0x55, 0xaa + 42 };
    memcpy(output_data_1 + 2, data + 42, 26);
    
    mock_eeprom_io_expect_write(output_data_0, sizeof(output_data_0));
    mock_eeprom_io_expect_write(output_data_1, sizeof(output_data_1));
    
    eeprom_status status = eeprom_page_write(0x55aa, data, sizeof(data));

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
}

TEST(eeprom_driver, page_128_write_success)
{
    uint8_t data[128] = {
        0xff, 0xaa, 0xff, 0x0,
        0x0, 0xff, 0xaa, 0x55
    };
    memset(data + 8, 0xaa, 120);
    data[100] = 0x55;
    uint8_t output_data_0[66] = { 0x55, 0xaa };
    memcpy(output_data_0 + 2, data, 64);
    uint8_t output_data_1[66] = { 0x55, 0xaa };
    memcpy(output_data_1 + 2, data + 64, 64);
    
    mock_eeprom_io_expect_write(output_data_0, sizeof(output_data_0));
    mock_eeprom_io_expect_write(output_data_1, sizeof(output_data_1));
    
    eeprom_status status = eeprom_page_write(0x55aa, data, sizeof(data));

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
}

TEST(eeprom_driver, page_8_sequential_read_success)
{
    // uint8_t output_data[] = { 0x55, 0xaa, 0xff };
    uint8_t input_data[8] = {
        0xff, 0xaa, 0xff, 0x0,
        0x0, 0xff, 0xaa, 0x55
    };
    uint8_t result[8];
    
    mock_eeprom_io_expect_read_then_return(input_data, sizeof(input_data));

    eeprom_status status = eeprom_sequential_read(result, 8);

    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    check_data(input_data, result, sizeof(input_data));
}
