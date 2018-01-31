# TODO Use find_* for some of the following enviroment.
#find_program(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
set(CMAKE_AR "arm-linux-gnueabihf-ar")
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
set(CMAKE_CXX_COMPILER_WITH_PATH "/usr/bin/arm-linux-gnueabihf-g++")
#set(CMAKE_CXX_FLAGS
#    "-Wall -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -MMD -I./ -I${CMAKE_CURRENT_BINARY_DIR}/include ")
set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
set(CMAKE_C_COMPILER_WITH_PATH "/usr/bin/arm-linux-gnueabihf-gcc")
set(CMAKE_C_FLAGS
    "-Wall -ffunction-sections -fdata-sections -MMD -I./")
set(CMAKE_LINKER "arm-linux-gnueabihf-ld")
set(CMAKE_NM "arm-linux-gnueabihf-nm")
set(CMAKE_OBJCOPY "arm-linux-gnueabihf-objcopy")
set(CMAKE_OBJDUMP "arm-linux-gnueabihf-objdump")
set(CMAKE_RANLIB "arm-linux-gnueabihf-ranlib")
set(CMAKE_STRIP "arm-linux-gnueabihf-strip")

