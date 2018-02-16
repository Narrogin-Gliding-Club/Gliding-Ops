# TODO Use find_* for some of the following enviroment.
#find_program(CMAKE_CXX_COMPILER "armv7-rpi2-linux-gnueabihf-g++")
set(ENV{PATH} $ENV{PATH}:$ENV{HOME}/x-tools/armv7-rpi2-linux-gnueabihf/bin/)
set(CMAKE_AR "armv7-rpi2-linux-gnueabihf-ar")
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_COMPILER "armv7-rpi2-linux-gnueabihf-g++")
set(CMAKE_CXX_COMPILER_WITH_PATH "$ENV{HOME}/x-tools/armv7-rpi2-linux-gnueabihf/bin/armv7-rpi2-linux-gnueabihf-g++")
#set(CMAKE_CXX_FLAGS
#    "-Wall -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -MMD -I./ -I${CMAKE_CURRENT_BINARY_DIR}/include ")
set(CMAKE_C_COMPILER "armv7-rpi2-linux-gnueabihf-gcc")
set(CMAKE_C_COMPILER_WITH_PATH "$ENV{HOME}/x-tools/armv7-rpi2-linux-gnueabihf/bin/armv7-rpi2-linux-gnueabihf-gcc")
set(CMAKE_C_FLAGS
    "-Wall -ffunction-sections -fdata-sections -MMD -I./")
set(CMAKE_LINKER "armv7-rpi2-linux-gnueabihf-ld")
set(CMAKE_NM "armv7-rpi2-linux-gnueabihf-nm")
set(CMAKE_OBJCOPY "armv7-rpi2-linux-gnueabihf-objcopy")
set(CMAKE_OBJDUMP "armv7-rpi2-linux-gnueabihf-objdump")
set(CMAKE_RANLIB "armv7-rpi2-linux-gnueabihf-ranlib")
set(CMAKE_STRIP "armv7-rpi2-linux-gnueabihf-strip")

