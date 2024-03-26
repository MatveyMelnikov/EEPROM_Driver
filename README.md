# EEPROM driver
### Launch
* ```make``` - building a production version of the code (does not contain tests);
* ```make -f MakefileTest.mk``` - building a test version. Depending on the [FOR_TARGET](https://github.com/MatveyMelnikov/EEPROM_Driver/blob/master/MakefileTest.mk) parameter,
either file [MakefileTestTarget.mk](https://github.com/MatveyMelnikov/EEPROM_Driver/blob/master/MakefileTestTarget.mk) (for the microcontroller) or file [MakefileTestHost.mk](https://github.com/MatveyMelnikov/EEPROM_Driver/blob/master/MakefileTestHost.mk) (for launching on the development system) is launched.;

### Structure
The driver itself is located [here](https://github.com/MatveyMelnikov/EEPROM_Driver/tree/master/External/eeprom_driver).
Tests using the [Unity library](https://github.com/MatveyMelnikov/EEPROM_Driver/tree/master/External/Unity-2.5.2) are implemented [here](https://github.com/MatveyMelnikov/EEPROM_Driver/tree/master/Tests). The tests are divided into tests for the host (development system) and target (stm32).
The main test module is [mock_eeprom_io](https://github.com/MatveyMelnikov/EEPROM_Driver/tree/master/Tests/mocks). It replaces the [IO module](https://github.com/MatveyMelnikov/EEPROM_Driver/blob/master/External/eeprom_driver/Inc/eeprom_io.h), 
which is an interface to the I2C hardware interface. The mock allows you to monitor the calls that the driver generates.
![sheme_eeprom](https://github.com/MatveyMelnikov/EEPROM_Driver/assets/55649891/7a7ecd3a-50b0-49e3-ade1-4de5838e2a50)
