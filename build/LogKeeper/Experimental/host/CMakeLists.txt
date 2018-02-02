set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(CMAKE_BUILD_TYPE "Debug")
add_definitions(-DPC)
add_executable(net ${EXPERIMENTAL_SRC}/Net.cpp)
target_link_libraries(net -lfann)
