# Production code file for the target platform

# C_INCLUDES = 

C_SOURCES += \
Core/Src/main.c \
$(EEPROM_DRIVER)/Src/eeprom_io.c \
$(EEPROM_DRIVER)/Src/eeprom_time.c

include MakefileTarget.mk
