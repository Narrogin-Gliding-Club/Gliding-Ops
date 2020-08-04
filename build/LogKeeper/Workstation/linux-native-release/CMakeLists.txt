set(CMAKE_BUILD_TYPE Release)
set(T "linux-native-release")
set(USE_MEMORY_CANVAS y)
add_definitions(-DHAVE_POSIX
                -DHAVE_STDINT_H
                -DHAVE_UNISTD_H
                -DHAVE_VASPRINTF
                -DUSE_MEMORY_CANVAS
                -DUSE_POLL_EVENT
                -DUSE_FREETYPE
                -DHAVE_VASPRINTF)
set(HAVE_POSIX TRUE)
include(../Include.cmake)
