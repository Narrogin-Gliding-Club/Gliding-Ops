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
                          Controller.adsl.test.hex
                          Controller.adsl.test.eep
                          Controller.flarm.test.hex
                          Controller.flarm.test.eep
                 )

set(SERIAL "/dev/ttyACM0")
set(AVRDUDE "/usr/share/arduino/hardware/tools/avrdude")
set(AVRCONF "/usr/share/arduino/hardware/tools/avrdude.conf")
add_custom_target(upload_Controller stty -F ${SERIAL} ospeed 1200 ispeed 1200
                  COMMAND ${AVRDUDE} -C${AVRCONF} -patmega32u4 -cavr109 -P${SERIAL} -b57600 -D -Uflash:w:Controller.hex:i
                  DEPENDS Controller.hex
                  COMMENT Uploading Controller.hex
                 )

add_executable(Controller.adsl.test.elf
               ${Arduino_SRC}/Controller.common.test.cpp
               ${Arduino_SRC}/Controller.adsl.test.cpp
              )
target_link_libraries(Controller.adsl.test.elf
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/MsTimer2/libMsTimer2.a
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/i2cdevlib/Arduino/I2Cdev/libI2Cdev.a
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/Low-Power/libLowPower.a
                      Arduino
                      core
                     )
add_custom_command(OUTPUT Controller.adsl.test.hex
                   COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom Controller.adsl.test.elf Controller.adsl.test.hex
                  DEPENDS Controller.adsl.test.elf
                  )
add_custom_command(OUTPUT Controller.adsl.test.eep
                   COMMAND ${CMAKE_OBJCOPY} -O ihex -j .eeprom Controller.adsl.test.elf Controller.adsl.test.eep
                   DEPENDS Controller.adsl.test.elf
                  )
add_custom_target(upload_Controller.adsl.test stty -F ${SERIAL} ospeed 1200 ispeed 1200
                  COMMAND ${AVRDUDE} -C${AVRCONF} -patmega32u4 -cavr109 -P${SERIAL} -b57600 -D -Uflash:w:Controller.adsl.test.hex:i
                  DEPENDS Controller.adsl.test.hex
                  COMMENT Uploading Controller.adsl.test.hex
                 )

add_executable(Controller.flarm.test.elf
               ${Arduino_SRC}/Controller.common.test.cpp
               ${Arduino_SRC}/Controller.flarm.test.cpp
              )
target_link_libraries(Controller.flarm.test.elf
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/MsTimer2/libMsTimer2.a
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/i2cdevlib/Arduino/I2Cdev/libI2Cdev.a
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/Low-Power/libLowPower.a
                      Arduino
                      core
                     )
add_custom_command(OUTPUT Controller.flarm.test.hex
                   COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom Controller.flarm.test.elf Controller.flarm.test.hex
                  DEPENDS Controller.flarm.test.elf
                  )
add_custom_command(OUTPUT Controller.flarm.test.eep
                   COMMAND ${CMAKE_OBJCOPY} -O ihex -j .eeprom Controller.flarm.test.elf Controller.flarm.test.eep
                   DEPENDS Controller.flarm.test.elf
                  )
add_custom_target(upload_Controller.flarm.test stty -F ${SERIAL} ospeed 1200 ispeed 1200
                  COMMAND ${AVRDUDE} -C${AVRCONF} -patmega32u4 -cavr109 -P${SERIAL} -b57600 -D -Uflash:w:Controller.flarm.test.hex:i
                  DEPENDS Controller.flarm.test.hex
                  COMMENT Uploading Controller.flarm.test.hex
                 )
