string(REPLACE "out/build/" "" I2Cdev_SRC ${CMAKE_CURRENT_BINARY_DIR})
add_library(I2Cdev STATIC ${I2Cdev_SRC}/I2Cdev.cpp)
