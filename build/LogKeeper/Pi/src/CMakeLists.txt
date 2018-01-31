string(REPLACE "out/build/" "" Pi_SRC ${CMAKE_CURRENT_BINARY_DIR})
set(BOOST_DIR ${CMAKE_CURRENT_BINARY_DIR}/../lib/boost)
set(WIRINGPI_DIR ${CMAKE_CURRENT_BINARY_DIR}/../lib/wiringPi)
set(WIRINGPI_LIB_DIR ${WIRINGPI_DIR})

add_custom_target(Log-Keeper-Pi-src
                  COMMAND echo Pi_SRC = ${Pi_SRC}
                  COMMAND echo BOOST_DIR = ${BOOST_DIR}
                  COMMAND echo WIRINGPI_DIR = ${WIRINGPI_DIR}
                  COMMAND echo WIRINGPI_LIB_DIR = ${WIRINGPI_LIB_DIR}
                 )

add_subdirectory(arm-linux-gnuebihf)
add_subdirectory(x86-linux-default)
