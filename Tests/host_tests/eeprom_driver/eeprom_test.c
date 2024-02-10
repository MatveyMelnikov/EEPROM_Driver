#include "unity_fixture.h"
#include "eeprom_driver.h"
#include "mock_eeprom_io.h"

TEST_GROUP(eeprom_driver);

TEST_SETUP(eeprom_driver)
{
    mock_eeprom_io_create(10);
    eeprom_create(void);
}

TEST_TEAR_DOWN(eeprom_driver)
{
    eeprom_destroy(void);
    mock_eeprom_io_destroy();
}

TEST(eeprom_driver, check_link_success)
{
    eeprom_status status = eeprom_check_link();

    TEST_ASSERT_EQUAL(status, EEPROM_OK);
}