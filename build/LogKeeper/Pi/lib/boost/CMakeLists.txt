add_custom_target(libboost
                  DEPENDS libboost-arm-linux-gnueabihf
                 )

add_subdirectory(arm-linux-gnueabihf-gcc)
add_subdirectory(x86-linux-default-gcc)
