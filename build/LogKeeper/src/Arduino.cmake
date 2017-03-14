# TODO Use find_* for some of the following enviroment.
set(CMAKE_AR "/usr/bin/avr-ar")
set(CMAKE_BUILD_TYPE "MinSizeRel")
# These should be "/usr/bin/avr-g++" but CMake insists on adding a -rdynamic
# flag so use an intervening script to fix up. Remove ASAP!
set(CMAKE_CXX_COMPILER "${Glider-Ops_SOURCE_DIR}/bin/kludged-avr-g++")
set(CMAKE_CXX_COMPILER_WITH_PATH "${Glider-Ops_SOURCE_DIR}/bin/kludged-avr-g++")
set(CMAKE_CXX_FLAGS
    "-Wall -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -mmcu=atmega32u4 -DF_CPU=16000000L -MMD -DUSB_VID=0x2341 -DUSB_PID=0x8036 -DARDUINO=105 -D__PROG_TYPES_COMPAT__ -I./ -I/usr/share/arduino/libraries/EEPROM -I/usr/share/arduino/libraries/FastSerial -I/usr/share/arduino/libraries/SPI -I/usr/share/arduino/libraries/Wire -I/usr/share/arduino/libraries/Wire/utility -I/usr/share/arduino/hardware/arduino/cores/arduino -I/usr/share/arduino/hardware/arduino/variants/leonardo -I${Glider-Ops_SOURCE_DIR}/LogKeeper/src/lib/MsTimer2 -I${Glider-Ops_SOURCE_DIR}/LogKeeper/src/lib/i2cdevlib/Arduino/I2Cdev -I${Glider-Ops_SOURCE_DIR}/LogKeeper/src/lib/Low-Power -I${Glider-Ops_SOURCE_DIR}/LogKeeper/src/lib/WSWireLib/Library/WSWire")
# These should be "/usr/bin/avr-gcc" but CMake insists on adding a -rdynamic
# flag so use an intervening script to fix up. Remove ASAP!
set(CMAKE_C_COMPILER "${Glider-Ops_SOURCE_DIR}/bin/kludged-avr-gcc")
set(CMAKE_C_COMPILER_WITH_PATH "${Glider-Ops_SOURCE_DIR}/bin/kludged-avr-gcc")
set(CMAKE_C_FLAGS
    "-Wall -ffunction-sections -fdata-sections -mmcu=atmega32u4 -DF_CPU=16000000L -MMD -DUSB_VID=0x2341 -DUSB_PID=0x8036 -DARDUINO=105 -D__PROG_TYPES_COMPAT__ -I./ -I/usr/share/arduino/libraries/EEPROM -I/usr/share/arduino/libraries/FastSerial -I/usr/share/arduino/libraries/SPI -I/usr/share/arduino/libraries/Wire -I/usr/share/arduino/libraries/Wire/utility -I/usr/share/arduino/hardware/arduino/cores/arduino -I/usr/share/arduino/hardware/arduino/variants/leonardo")
set(CMAKE_LINKER "/usr/bin/avr-ld")
set(CMAKE_NM "/usr/bin/avr-nm")
set(CMAKE_OBJCOPY "/usr/bin/avr-objcopy")
set(CMAKE_OBJDUMP "/usr/bin/avr-objdump")
set(CMAKE_RANLIB "/usr/bin/avr-ranlib")
set(CMAKE_STRIP "/usr/bin/avr-strip")

