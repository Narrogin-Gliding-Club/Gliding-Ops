set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(CMAKE_BUILD_TYPE "Debug")
add_definitions(-DPC)
add_executable(controller-x86-linux-default
               ${Pi_SRC}/Controller.cpp
              )
target_link_libraries(controller-x86-linux-default
                      -lboost_system
                      -lboost_exception
                      -lpthread
                     )
