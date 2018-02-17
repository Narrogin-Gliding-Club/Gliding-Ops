string(REPLACE "out/build/" "" wiringPi_SRC ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(LogKeeper-Pi-lib-wiringPi
                  COMMAND true
                  COMMAND echo CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}
                  COMMAND echo wiringPi_SRC = ${wiringPi_SRC}
                  COMMAND echo CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}
                 )

#set(CMAKE_C_FLAGS "-O2 -D_GNU_SOURCE -I. -Wformat=2 -Wall -Wextra -Winline fPIC")
add_custom_target(libwiringPi export PATH=/usr/bin:/bin:/opt/pi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin \; export DESTDIR=${CMAKE_CURRENT_BINARY_DIR} \; export PREFIX="" \; cd ${wiringPi_SRC}/wiringPi \; make CC=${CMAKE_C_COMPILER} install-static
                  COMMAND rm ${wiringPi_SRC}/wiringPi/libwiringPi.a
#                  DEPENDS echovars
                  WORKING_DIRECTORY ${wiringPi_SRC}/wiringPi
                 )


