#include "unity_fixture.h"
#include <stdbool.h>

TEST_GROUP(anything);

TEST_SETUP(anything)
{
}

TEST_TEAR_DOWN(anything)
{
}

TEST(anything, success_test)
{
    TEST_ASSERT_EQUAL(true, true);
}

TEST(anything, fail_test)
{
    FAIL("Fails message");
}
