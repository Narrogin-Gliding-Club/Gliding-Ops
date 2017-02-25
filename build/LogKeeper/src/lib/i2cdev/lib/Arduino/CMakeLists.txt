cmake_minimum_required(VERSION 2.8.5)

# TODO Use find_* for some of the following enviroment.
set(CMAKE_AR "/usr/bin/avr-ar")
set(CMAKE_BUILD_TYPE "MinSizeRel")
set(CMAKE_CXX_COMPILER
    "/home/pfb/src/G-Meter/jni/Engine/ArduIMU_V3/kludged-avr-g++")
set(CMAKE_CXX_COMPILER_WITH_PATH
    "/home/pfb/src/G-Meter/jni/Engine/ArduIMU_V3/kludgedavr-g+")
set(CMAKE_CXX_FLAGS
    "-Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=101 -I./ -I/usr/share/arduino/libraries/EEPROM -I/usr/share/arduino/libraries/FastSerial -I/usr/share/arduino/libraries/SPI -I/usr/share/arduino/libraries/Wire -I/usr/share/arduino/libraries/Wire/utility -I/usr/share/arduino/hardware/arduino/cores/arduino -I/usr/share/arduino/hardware/arduino/variants/standard -I${G-Meter_SOURCE_DIR}/jni/Engine/ArduIMU_V3")
set(CMAKE_C_COMPILER "/usr/bin/avr-gcc")
set(CMAKE_C_COMPILER_WITH_PATH "/usr/bin/avr-gcc")
set(CMAKE_C_FLAGS
    "-Wall -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=101 -I./ -I/usr/share/arduino/libraries/EEPROM -I/usr/share/arduino/libraries/FastSerial -I/usr/share/arduino/libraries/SPI -I/usr/share/arduino/libraries/Wire -I/usr/share/arduino/libraries/Wire/utility -I/usr/share/arduino/hardware/arduino/cores/arduino -I/usr/share/arduino/hardware/arduino/variants/standard -I{G-Meter_SOURCE_DIR}/jni/Engine/ArduIMU_V3")
set(CMAKE_LINKER "/usr/bin/avr-ld")
set(CMAKE_NM "/usr/bin/avr-nm")
set(MAKE_OBJCOPY "/usr/bin/avr-objcopy")
set(CMAKE_OBJDUMP "/usr/bin/avr-objdump")
set(CMAKE_RANLIB "/usr/bin/avr-ranlib")
set(CMAKE_STRIP "/usr/bin/avr-strip")


#set(ARDUINO_LIBRARY_SRCDIR "/usr/share/arduino/libraries")
#set(ARDUINO_LIBRARY_CORES  "/usr/share/arduino/hardware/arduino/cores/arduino")
add_library(Arduino STATIC
            ${ARDUINO_LIBRARY_SRCDIR}/EEPROM/EEPROM.cpp
            ${ARDUINO_LIBRARY_SRCDIR}/SPI/SPI.cpp
            ${ARDUINO_LIBRARY_SRCDIR}/Wire/Wire.cpp
            ${ARDUINO_LIBRARY_SRCDIR}/Wire/utility/twi.c
           )

add_library(core STATIC
            ${ARDUINO_LIBRARY_CORES}/wiring_digital.c
            ${ARDUINO_LIBRARY_CORES}/wiring_analog.c
            ${ARDUINO_LIBRARY_CORES}/wiring_pulse.c
            ${ARDUINO_LIBRARY_CORES}/wiring_shift.c
            ${ARDUINO_LIBRARY_CORES}/wiring.c
            ${ARDUINO_LIBRARY_CORES}/WInterrupts.c
            ${ARDUINO_LIBRARY_CORES}/main.cpp
            ${ARDUINO_LIBRARY_CORES}/IPAddress.cpp
            ${ARDUINO_LIBRARY_CORES}/HardwareSerial.cpp
            ${ARDUINO_LIBRARY_CORES}/CDC.cpp
            ${ARDUINO_LIBRARY_CORES}/USBCore.cpp
            ${ARDUINO_LIBRARY_CORES}/new.cpp
            ${ARDUINO_LIBRARY_CORES}/WMath.cpp
            ${ARDUINO_LIBRARY_CORES}/Print.cpp
            ${ARDUINO_LIBRARY_CORES}/Stream.cpp
            ${ARDUINO_LIBRARY_CORES}/WString.cpp
            ${ARDUINO_LIBRARY_CORES}/HID.cpp
            ${ARDUINO_LIBRARY_CORES}/Tone.cpp
           )

