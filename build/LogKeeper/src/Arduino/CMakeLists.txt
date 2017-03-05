include("../Arduino.cmake")
string(REPLACE "out/build/" "" Arduino_SRC ${CMAKE_CURRENT_BINARY_DIR})
add_executable(Controller.elf
               ${Arduino_SRC}/Controller.cpp
              )
target_link_libraries(Controller.elf 
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/MsTimer2/libMsTimer2.a
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/i2cdevlib/Arduino/I2Cdev/libI2Cdev.a
                      ${Glider-Ops_BINARY_DIR}/build/LogKeeper/src/lib/Low-Power/libLowPower.a
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
                 )

set(SERIAL "/dev/ttyACM0")  # Presumably this is correct!?
set(AVRDUDE "/usr/share/arduino/hardware/tools/avrdude")
set(AVRCONF "/usr/share/arduino/hardware/tools/avrdude.conf")
add_custom_target(upload_Controller stty -F ${SERIAL} ospeed 1200 ispeed 1200
                  COMMAND ${AVRDUDE} -C${AVRCONF} -patmega32u4 -cavr109 -P${SERIAL} -b57600 -D -Uflash:w:Controller.hex:i
                  DEPENDS Controller.hex
                  COMMENT Uploading Controller.hex
                 )
