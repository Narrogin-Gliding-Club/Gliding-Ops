string(REPLACE "out/build/" "" LowPower_SRC ${CMAKE_CURRENT_BINARY_DIR})
add_library(LowPower STATIC ${LowPower_SRC}/LowPower.cpp)
