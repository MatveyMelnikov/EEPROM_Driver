# File for test cases
# Here you can add platform-specific files (main file, test, etc.)

CFLAGS = -DTEST -DUNITY_INCLUDE_CONFIG_H
TESTS_DIR = Tests
UNITY_DIR = External/Unity-2.5.2

C_INCLUDES += \
-I$(UNITY_DIR)/src \
-I$(UNITY_DIR)/extras/fixture/src \
-I$(UNITY_DIR)/extras/memory/src \
-I$(TESTS_DIR) \
-I$(TESTS_DIR)/mocks/Inc \
-I$(TESTS_DIR)/fakes/Inc

C_SOURCES += \
$(UNITY_DIR)/src/unity.c \
$(UNITY_DIR)/extras/fixture/src/unity_fixture.c \
$(UNITY_DIR)/extras/memory/src/unity_memory.c \
$(TESTS_DIR)/mocks/Src/mock_eeprom_io.c \
$(TESTS_DIR)/fakes/Src/fake_eeprom_time.c

ifeq ($(FOR_TARGET), Y)
C_SOURCES += \
$(TESTS_DIR)/target_tests.c \
$(TESTS_DIR)/target_tests/eeprom_driver/eeprom_test_runner.c \
$(TESTS_DIR)/target_tests/eeprom_driver/eeprom_test.c

include MakefileTarget.mk
else
C_SOURCES += \
$(TESTS_DIR)/host_tests.c \
$(TESTS_DIR)/host_tests/eeprom_driver/eeprom_test_runner.c \
$(TESTS_DIR)/host_tests/eeprom_driver/eeprom_test.c
include MakefileHost.mk
endif
