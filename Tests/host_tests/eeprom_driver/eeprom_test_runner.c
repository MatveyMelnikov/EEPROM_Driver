#include "unity_fixture.h"

TEST_GROUP_RUNNER(eeprom_driver)
{
    RUN_TEST_CASE(eeprom_driver, check_link_success);
}