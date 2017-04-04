cmake_minimum_required(VERSION 2.8.5)
string(REPLACE "out/build/" "" Boost_SRC ${CMAKE_CURRENT_BINARY_DIR})
include("../../Pi.cmake")

set(BOOST_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(BOOST_LIBS ${BOOST_LIB_DIR}/libboost_atomic.a
               ${BOOST_LIB_DIR}/libboost_chrono.a
               ${BOOST_LIB_DIR}/libboost_container.a
               ${BOOST_LIB_DIR}/libboost_date_time.a
               ${BOOST_LIB_DIR}/libboost_exception.a
               ${BOOST_LIB_DIR}/libboost_filesystem.a
               ${BOOST_LIB_DIR}/libboost_graph.a
               ${BOOST_LIB_DIR}/libboost_locale.a
               ${BOOST_LIB_DIR}/libboost_log.a
               ${BOOST_LIB_DIR}/libboost_log_setup.a
               ${BOOST_LIB_DIR}/libboost_math_c99.a
               ${BOOST_LIB_DIR}/libboost_math_c99f.a
               ${BOOST_LIB_DIR}/libboost_math_c99l.a
               ${BOOST_LIB_DIR}/libboost_math_tr1.a
               ${BOOST_LIB_DIR}/libboost_math_tr1f.a
               ${BOOST_LIB_DIR}/libboost_math_tr1l.a
               ${BOOST_LIB_DIR}/libboost_program_options.a
               ${BOOST_LIB_DIR}/libboost_random.a
               ${BOOST_LIB_DIR}/libboost_regex.a
               ${BOOST_LIB_DIR}/libboost_serialization.a
               ${BOOST_LIB_DIR}/libboost_signals.a
               ${BOOST_LIB_DIR}/libboost_system.a
               ${BOOST_LIB_DIR}/libboost_thread.a
               ${BOOST_LIB_DIR}/libboost_timer.a
               ${BOOST_LIB_DIR}/libboost_wave.a
   )

#add_custom_command(OUTPUT ${BOOST_LIBS}
#                   COMMAND export BOOST_BUILD_PATH=${Boost_SRC}/../ \; ${Boost_SRC}/b2 --prefix=${CMAKE_CURRENT_BINARY_DIR}
#                  )

add_custom_target(libboost export PATH=${PATH}:/opt/pi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin \; export BOOST_BUILD_PATH=${Boost_SRC}/../ \; export BOOST_ROOT=${Boost_SRC} \; ${Boost_SRC}/b2 --prefix=${CMAKE_CURRENT_BINARY_DIR} --build-dir=${CMAKE_CURRENT_BINARY_DIR} --without-atomic --without-coroutine --without-mpi --without-context --without-graph_parallel --without-iostreams --without-python --without-test --without-thread install
                 DEPENDS ${BOOST_LIBS}
                 WORKING_DIRECTORY ${Boost_SRC}
                 )


