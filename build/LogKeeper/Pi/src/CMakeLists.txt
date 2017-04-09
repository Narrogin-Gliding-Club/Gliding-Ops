string(REPLACE "out/build/" "" Pi_SRC ${CMAKE_CURRENT_BINARY_DIR})
set(BOOST_DIR ${CMAKE_CURRENT_BINARY_DIR}/../lib/boost)
set(BOOST_LIB_DIR ${BOOST_DIR}/lib)

add_subdirectory(arm-linux-gnuebihf)
add_subdirectory(x86-linux-default)
