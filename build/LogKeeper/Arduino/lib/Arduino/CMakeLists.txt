cmake_minimum_required(VERSION 2.8.5)

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

