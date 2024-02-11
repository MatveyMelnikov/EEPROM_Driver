#include "unity_fixture.h"
#include "eeprom_driver.h"
#include "mock_eeprom_io.h"

TEST_GROUP(eeprom_driver);

TEST_SETUP(eeprom_driver)
{
    mock_eeprom_io_create(10);
    //eeprom_driver_create(void);
}

TEST_TEAR_DOWN(eeprom_driver)
{
    //eeprom_driver_destroy(void);
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

    TEST_ASSERT_EQUAL(status, EEPROM_OK);
}

TEST(eeprom_driver, random_write_byte_success)
{
    uint8_t output_data[] = { 0x55, 0xaa, 0x54 };
    uint8_t input_data = 0x54;

    mock_eeprom_io_expect_write(output_data, 3); // 0xaa, 0x55, 0xff
    mock_eeprom_io_expect_read_then_return(&input_data, 1);

    eeprom_status status = eeprom_byte_write(0xaa55, 0x54);

    TEST_ASSERT_EQUAL(status, EEPROM_OK);
}

TEST(eeprom_driver, random_read_byte_success)
{
    uint8_t output_data[] = { 0x55, 0xaa, 0xff };
    uint8_t input_data = 0x55;
    uint8_t result_data = 0x0;

    mock_eeprom_io_expect_write(output_data, 3); // 0xaa, 0x55, 0xff
    mock_eeprom_io_expect_read_then_return(&input_data, 1);

    eeprom_status status = eeprom_random_byte_read(0xaa55, &result_data);

    TEST_ASSERT_EQUAL(status, EEPROM_OK);
    TEST_ASSERT_EQUAL(input_data, result_data);
}
