cmake_minimum_required(VERSION 3.0)

project(Glider-Ops)

set(CMAKE_CXX_FLAGS
    "-Wno-error=unused-parameter -Wall -Wextra -Wwrite-strings -Wcast-qual -Wpointer-arith -Wsign-compare -Wmissing-declarations -Wredundant-decls -Wmissing-noreturn -Wno-unused-parameter -Wno-missing-field-initializers  -Wcast-align -Werror -fno-exceptions -fno-rtti -std=gnu++0x -fno-threadsafe-statics -fmerge-all-constants -fvisibility=hidden -fpic -funwind-tables -finput-charset=utf-8 "
    CACHE STRING "Flags used by the compiler during all build types." FORCE)

# Common constants.
set(ARDUINO_LIBRARY_SRCDIR "/usr/share/arduino/libraries")
set(ARDUINO_LIBRARY_CORES  "/usr/share/arduino/hardware/arduino/cores/arduino")
set(AVRSIZE_PROGRAM "/usr/bin/avr-size")
add_subdirectory(build)

