set(GLIDING_OPS ${CMAKE_HOME_DIRECTORY})
set(GLIDING_OPS_SRC ${GLIDING_OPS}/src)
set(OUTPUT_INCLUDE ${XCSoar-the-library_BINARY_DIR}/include)
add_subdirectory(linux-native-debug)
add_subdirectory(linux-native-release)

add_custom_target(linux-native
                  DEPENDS linux-native-debug
                          linux-native-release
                          linux-native-sanitize)
add_custom_target(linux-release  DEPENDS linux-native-release)
add_custom_target(linux-debug    DEPENDS linux-native-debug)
