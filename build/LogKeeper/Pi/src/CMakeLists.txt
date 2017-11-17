string(REPLACE "out/build/" "" Pi_SRC ${CMAKE_CURRENT_BINARY_DIR})
set(BOOST_DIR ${CMAKE_CURRENT_BINARY_DIR}/../lib/boost)
set(WIRINGPI_DIR ${CMAKE_CURRENT_BINARY_DIR}/../lib/wiringPi)
set(BOOST_LIB_DIR ${BOOST_DIR}/lib)
set(WIRINGPI_LIB_DIR ${WIRINGPI_DIR})

add_subdirectory(arm-linux-gnuebihf)
add_subdirectory(x86-linux-default)
