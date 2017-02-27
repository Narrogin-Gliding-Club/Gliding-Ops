# TODO Use find_* for some of the following enviroment.
set(CMAKE_AR "/usr/bin/avr-ar")
set(CMAKE_BUILD_TYPE "MinSizeRel")
set(CMAKE_CXX_COMPILER "/usr/bin/avr-g++")
set(CMAKE_CXX_COMPILER_WITH_PATH "/usr/bin/avr-g++")
set(CMAKE_CXX_FLAGS
    "-Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=101 -I./ -I/usr/share/arduino/libraries/EEPROM -I/usr/share/arduino/libraries/FastSerial -I/usr/share/arduino/libraries/SPI -I/usr/share/arduino/libraries/Wire -I/usr/share/arduino/libraries/Wire/utility -I/usr/share/arduino/hardware/arduino/cores/arduino -I/usr/share/arduino/hardware/arduino/variants/standard")
set(CMAKE_C_COMPILER "/usr/bin/avr-gcc")
set(CMAKE_C_COMPILER_WITH_PATH "/usr/bin/avr-gcc")
set(CMAKE_C_FLAGS
    "-Wall -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=101 -I./ -I/usr/share/arduino/libraries/EEPROM -I/usr/share/arduino/libraries/FastSerial -I/usr/share/arduino/libraries/SPI -I/usr/share/arduino/libraries/Wire -I/usr/share/arduino/libraries/Wire/utility -I/usr/share/arduino/hardware/arduino/cores/arduino -I/usr/share/arduino/hardware/arduino/variants/standard")
set(CMAKE_LINKER "/usr/bin/avr-ld")
set(CMAKE_NM "/usr/bin/avr-nm")
set(MAKE_OBJCOPY "/usr/bin/avr-objcopy")
set(CMAKE_OBJDUMP "/usr/bin/avr-objdump")
set(CMAKE_RANLIB "/usr/bin/avr-ranlib")
set(CMAKE_STRIP "/usr/bin/avr-strip")

