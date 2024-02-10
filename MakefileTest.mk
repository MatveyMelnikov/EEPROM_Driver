# File for test cases
# Here you can add platform-specific files (main file, test, etc.)

CFLAGS = -DTEST -DUNITY_INCLUDE_CONFIG_H
TESTS_DIR = Tests
UNITY_DIR = External/Unity-2.5.2

C_INCLUDES = \
-I$(UNITY_DIR)/src \
-I$(UNITY_DIR)/extras/fixture/src \
-I$(UNITY_DIR)/extras/memory/src \
-I$(TESTS_DIR)

C_SOURCES = \
$(UNITY_DIR)/src/unity.c \
$(UNITY_DIR)/extras/fixture/src/unity_fixture.c \
$(UNITY_DIR)/extras/memory/src/unity_memory.c

ifeq ($(FOR_TARGET), Y)
C_SOURCES += \
$(TESTS_DIR)/all_tests.c \
$(TESTS_DIR)/anything/anything_test.c \
$(TESTS_DIR)/anything/anything_test_runner.c

include MakefileTarget.mk
else
C_SOURCES += \
Core/Src/any_clear_logic.c \
$(TESTS_DIR)/host_tests.c \
$(TESTS_DIR)/any_clear_logic/any_clear_logic_runner.c \
$(TESTS_DIR)/any_clear_logic/any_clear_logic_test.c
include MakefileHost.mk
endif
