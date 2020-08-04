set(LOGKEEPER ${Glider-Ops_SOURCE_DIR}/LogKeeper)
set(WORKSTATION ${LOGKEEPER}/Workstation)
set(COMPUTER ${WORKSTATION}/Computer)
set(ENGINE   ${WORKSTATION}/Engine)
set(GEO      ${WORKSTATION}/Geo)
set(MATH     ${WORKSTATION}/Math)
set(TIME     ${WORKSTATION}/Time)
set(XCSOAR_LIB ${LOGKEEPER}/lib/XCSoar-lib)
set(XCSOAR_INCLUDES ${XCSOAR_LIB}/out/include)
set(XCSOAR_LIBS ${LOGKEEPER}/out/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wno-error=unused-function")

include_directories(${COMPUTER}
                    ${WORKSTATION}
                    ${ENGINE}
                    ${GEO}
                    ${WORKSTATION}/Time
                    ${WORKSTATION}/Math)
#link_directories(${XCSOAR_LIBS})
#link_libraries(Geo-shared-${T}
#               Time-shared-${T})

add_executable(flyingcomputer-${T} ${COMPUTER}/FlyingComputerMain.cpp
                                   ${COMPUTER}/FlyingComputer.cpp
                                   ${COMPUTER}/FlyingState.cpp
                                   ${COMPUTER}/Info.cpp
                                   ${ENGINE}/Navigation/Aircraft.cpp
                                   ${GEO}/GeoPoint.cpp
                                   ${GEO}/GeoVector.cpp
                                   ${GEO}/Math.cpp
                                   ${MATH}/Angle.cpp
                                   ${TIME}/BrokenDate.cpp
                                   ${TIME}/BrokenDateTime.cpp
                                   ${TIME}/BrokenTime.cpp
                                   ${TIME}/DeltaTime.cpp)
target_link_libraries(flyingcomputer-${T} ${})
