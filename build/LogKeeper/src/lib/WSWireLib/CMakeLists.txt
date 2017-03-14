include("../../Arduino.cmake")
string(REPLACE "out/build/" "" WSWireLib_SRC ${CMAKE_CURRENT_BINARY_DIR})
set(WSWireLib_SRC ${WSWireLib_SRC}/Library/WSWire)
add_library(WSWire STATIC ${WSWireLib_SRC}/WSWire.cpp
                          ${WSWireLib_SRC}/utility/twi.c)
