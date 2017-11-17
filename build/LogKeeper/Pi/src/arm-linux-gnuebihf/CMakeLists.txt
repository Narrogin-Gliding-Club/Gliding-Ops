add_compile_options(-I${BOOST_DIR}/include -DBOOST_NO_EXCEPTIONS)
add_compile_options(-I${WIRINGPI_LIB_DIR}/include)

add_definitions(-DPI)

add_custom_target(echoPivars
                  COMMAND echo WIRINGPI_LIB_DIR = ${WIRINGPI_LIB_DIR}
                 )

add_executable(controller-arm-linux-gnuebihf
               ${Pi_SRC}/Controller.cpp
              )
# This relies on Debian's libc6-dev:armhf being installed!
target_link_libraries(controller-arm-linux-gnuebihf
                      ${BOOST_LIB_DIR}/libboost_system.a
                      ${BOOST_LIB_DIR}/libboost_exception.a
                      ${WIRINGPI_LIB_DIR}/lib/libwiringPi.a
                      /usr/lib/arm-linux-gnueabihf/libc.a
                      -lpthread
                     )
#add_dependencies(controller-arm-linux-gnuebihf echoPivars)
