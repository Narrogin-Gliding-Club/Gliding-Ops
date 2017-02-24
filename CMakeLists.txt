cmake_minimum_required(VERSION 3.0)

project(Glider-Ops)

set(cxx-warnings "-Wno-error=unused-parameter -Wall -Wextra -Wwrite-strings -Wcast-qual -Wpointer-arith -Wsign-compare -Wundef -Wmissing-declarations -Wredundant-decls -Wmissing-noreturn -Wno-unused-parameter -Wno-missing-field-initializers  -Wcast-align -Werror")
set(cxx-flags "-fno-exceptions -fno-rtti -std=gnu++0x -fno-threadsafe-statics -fmerge-all-constants -fvisibility=hidden -fpic -funwind-tables -finput-charset=utf-8")
set(CMAKE_CXX_FLAGS ${cxx-warnings} ${cxx-flags}
        CACHE STRING "Flags used by the compiler during all build types." FORCE)

add_subdirectory(build)

