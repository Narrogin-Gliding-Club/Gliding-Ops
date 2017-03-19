include("../Arduino.cmake")
string(REPLACE "out/build/" "" Arduino_SRC ${CMAKE_CURRENT_BINARY_DIR})
add_executable(Controller.elf
               ${Arduino_SRC}/Controller.cpp
              )
target_link_libraries(Controller.elf 
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/MsTimer2/libMsTimer2.a
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/i2cdevlib/Arduino/I2Cdev/libI2Cdev.a
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/Low-Power/libLowPower.a
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/WSWireLib/libWSWire.a
                      Arduino
                      core
                     )
add_custom_command(OUTPUT Controller.hex
                   COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom Controller.elf Controller.hex
                  DEPENDS Controller.elf
                  )
add_custom_command(OUTPUT Controller.eep
                   COMMAND ${CMAKE_OBJCOPY} -O ihex -j .eeprom Controller.elf Controller.eep
                   DEPENDS Controller.elf
                  )
add_custom_target(hex
                  DEPENDS Controller.hex 
                          Controller.eep
                          Controller.test.hex
                          Controller.test.eep
                 )
add_custom_command(OUTPUT Controller.s
                   COMMAND ${CMAKE_CXX_COMPILER} -S ${CMAKE_CXX_FLAGS} -o Controller.s ${Arduino_SRC}/Controller.cpp
                   DEPENDS ${Arduino_SRC}/Controller.cpp
                  )
add_custom_target(assembler
                  DEPENDS Controller.s
                 )

set(SERIAL_DUT "/dev/ttyACM0")
set(SERIAL_VTD "/dev/ttyACM1")
set(AVRDUDE "/usr/share/arduino/hardware/tools/avrdude")
set(AVRCONF "/usr/share/arduino/hardware/tools/avrdude.conf")
add_custom_target(upload_Controller stty -F ${SERIAL_DUT} ospeed 1200 ispeed 1200
                  COMMAND ${AVRDUDE} -C${AVRCONF} -patmega32u4 -cavr109 -P${SERIAL_DUT} -b57600 -D -Uflash:w:Controller.hex:i
                  DEPENDS Controller.hex
                  COMMENT Uploading Controller.hex
                 )

add_executable(Controller.test.elf
               ${Arduino_SRC}/Controller.test.cpp
              )
target_link_libraries(Controller.test.elf
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/MsTimer2/libMsTimer2.a
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/i2cdevlib/Arduino/I2Cdev/libI2Cdev.a
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/Low-Power/libLowPower.a
                      Arduino
                      core
                     )
add_custom_command(OUTPUT Controller.test.hex
                   COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom Controller.test.elf Controller.test.hex
                  DEPENDS Controller.test.elf
                  )
add_custom_command(OUTPUT Controller.test.eep
                   COMMAND ${CMAKE_OBJCOPY} -O ihex -j .eeprom Controller.test.elf Controller.test.eep
                   DEPENDS Controller.test.elf
                  )
add_custom_target(upload_Controller.test stty -F ${SERIAL_VTD} ospeed 1200 ispeed 1200
                  COMMAND ${AVRDUDE} -C${AVRCONF} -patmega32u4 -cavr109 -P${SERIAL_VTD} -b57600 -D -Uflash:w:Controller.test.hex:i
                  DEPENDS Controller.test.hex
                  COMMENT Uploading Controller.test.hex
                 )

