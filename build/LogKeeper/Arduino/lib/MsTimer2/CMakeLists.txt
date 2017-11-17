string(REPLACE "out/build/" "" MsTimer2_SRC ${CMAKE_CURRENT_BINARY_DIR})
add_library(MsTimer2 STATIC ${MsTimer2_SRC}/MsTimer2.cpp)
