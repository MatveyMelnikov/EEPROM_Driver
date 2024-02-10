#include "unity_fixture.h"

TEST_GROUP_RUNNER(anything)
{
    RUN_TEST_CASE(anything, success_test);
    RUN_TEST_CASE(anything, fail_test);
}