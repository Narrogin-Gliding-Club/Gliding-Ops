add_compile_options(-I${BOOST_DIR}/include -DBOOST_NO_EXCEPTIONS)

add_executable(controller-arm-linux-gnuebihf
               ${Pi_SRC}/Controller.cpp
              )
target_link_libraries(controller-arm-linux-gnuebihf
                      ${BOOST_LIB_DIR}/libboost_system.a
                      ${BOOST_LIB_DIR}/libboost_exception.a
                      -lpthread
                     )
