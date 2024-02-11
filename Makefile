TESTS = Y
FOR_TARGET = Y
EEPROM_DRIVER = External/eeprom_driver

C_INCLUDES = \
-I$(EEPROM_DRIVER)/Inc

C_SOURCES = \
$(EEPROM_DRIVER)/Src/eeprom_driver.c \
$(EEPROM_DRIVER)/Src/eeprom_isr.c \

ifeq ($(TESTS), Y)
include MakefileTest.mk
else # For target only
include MakefileProduction.mk
endif
