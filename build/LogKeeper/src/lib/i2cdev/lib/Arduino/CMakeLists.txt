cmake_minimum_required(VERSION 2.8.5)

#set(CMAKE_TOOLCHAIN_FILE ./ArduinoToolchain.cmake)

project(ArduIMU)

# TODO Use find_* for some of the following enviroment.
set(ARDUINO_LIBRARY_SRCDIR "/usr/share/arduino/libraries")
set(ARDUINO_LIBRARY_CORES  "/usr/share/arduino/hardware/arduino/cores/arduino")
set(CMAKE_AR "/usr/bin/avr-ar")
set(AVRSIZE_PROGRAM "/usr/bin/avr-size")
set(CMAKE_BUILD_TYPE "MinSizeRel")
set(CMAKE_CXX_COMPILER
    "/home/pfb/src/G-Meter/jni/Engine/ArduIMU_V3/kludged-avr-g++")
set(CMAKE_CXX_COMPILER_WITH_PATH
    "/home/pfb/src/G-Meter/jni/Engine/ArduIMU_V3/kludgedavr-g+")
set(CMAKE_CXX_FLAGS
    "-Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=101 -I./ -I/usr/share/arduino/libraries/EEPROM -I/usr/share/arduino/libraries/FastSerial -I/usr/share/arduino/libraries/SPI -I/usr/share/arduino/libraries/Wire -I/usr/share/arduino/libraries/Wire/utility -I/usr/share/arduino/hardware/arduino/cores/arduino -I/usr/share/arduino/hardware/arduino/variants/standard -I${G-Meter_SOURCE_DIR}/jni/Engine/ArduIMU_V3")
set(CMAKE_C_COMPILER "/usr/bin/avr-gcc")
set(CMAKE_C_COMPILER_WITH_PATH "/usr/bin/avr-gcc")
set(CMAKE_C_FLAGS
    "-Wall -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=101 -I./ -I/usr/share/arduino/libraries/EEPROM -I/usr/share/arduino/libraries/FastSerial -I/usr/share/arduino/libraries/SPI -I/usr/share/arduino/libraries/Wire -I/usr/share/arduino/libraries/Wire/utility -I/usr/share/arduino/hardware/arduino/cores/arduino -I/usr/share/arduino/hardware/arduino/variants/standard -I{G-Meter_SOURCE_DIR}/jni/Engine/ArduIMU_V3")
set(CMAKE_LINKER "/usr/bin/avr-ld")
set(CMAKE_NM "/usr/bin/avr-nm")
set(MAKE_OBJCOPY "/usr/bin/avr-objcopy")
set(CMAKE_OBJDUMP "/usr/bin/avr-objdump")
set(CMAKE_RANLIB "/usr/bin/avr-ranlib")
set(CMAKE_STRIP "/usr/bin/avr-strip")


add_custom_command(OUTPUT Arduimu.hex
                   COMMAND avr-objcopy -O ihex -R .eeprom Arduimu.elf Arduimu.hex
                  DEPENDS Arduimu.elf
                  )
add_custom_command(OUTPUT Arduimu.eep
                   COMMAND avr-objcopy -O ihex -j .eeprom Arduimu.elf Arduimu.eep
                   DEPENDS Arduimu.elf
                  )
add_custom_command(OUTPUT CalibrateIMUFirmware.hex
                   COMMAND avr-objcopy -O ihex -R .eeprom CalibrateIMUFirmware.elf CalibrateIMUFirmware.hex
                  DEPENDS CalibrateIMUFirmware.elf
                  )
add_custom_command(OUTPUT CalibrateIMUFirmware.eep
                   COMMAND avr-objcopy -O ihex -j .eeprom CalibrateIMUFirmware.elf CalibrateIMUFirmware.eep
                   DEPENDS CalibrateIMUFirmware.elf
                  )
add_custom_command(OUTPUT AllZeros.hex
                   COMMAND avr-objcopy -O ihex -R .eeprom AllZeros.elf AllZeros.hex
                   DEPENDS AllZeros.elf
                  )
add_custom_command(OUTPUT AllZeros.eep
                   COMMAND avr-objcopy -O ihex -j .eeprom AllZeros.elf AllZeros.eep
                   DEPENDS AllZeros.elf
                  )
add_custom_command(OUTPUT AllOnes.hex
                   COMMAND avr-objcopy -O ihex -R .eeprom AllOnes.elf AllOnes.hex
                   DEPENDS AllOnes.elf
                  )
add_custom_command(OUTPUT AllOnes.eep
                   COMMAND avr-objcopy -O ihex -j .eeprom AllOnes.elf AllOnes.eep
                   DEPENDS AllOnes.elf
                  )
add_custom_command(OUTPUT AllTwos.hex
                   COMMAND avr-objcopy -O ihex -R .eeprom AllTwos.elf AllTwos.hex
                   DEPENDS AllTwos.elf
                  )
add_custom_command(OUTPUT AllTwos.eep
                   COMMAND avr-objcopy -O ihex -j .eeprom AllTwos.elf AllTwos.eep
                   DEPENDS AllTwos.elf
                  )
add_custom_command(OUTPUT AllFours.hex
                   COMMAND avr-objcopy -O ihex -R .eeprom AllFours.elf AllFours.hex
                   DEPENDS AllFours.elf
                  )
add_custom_command(OUTPUT AllFours.eep
                   COMMAND avr-objcopy -O ihex -j .eeprom AllFours.elf AllFours.eep
                   DEPENDS AllFours.elf
                  )
add_custom_command(OUTPUT AllEights.hex
                   COMMAND avr-objcopy -O ihex -R .eeprom AllEights.elf AllEights.hex
                   DEPENDS AllEights.elf
                  )
add_custom_command(OUTPUT AllEights.eep
                   COMMAND avr-objcopy -O ihex -j .eeprom AllEights.elf AllEights.eep
                   DEPENDS AllEights.elf
                  )
add_custom_target(hex
                  DEPENDS Arduimu.hex 
                          Arduimu.eep
                          CalibrateIMUFirmware.hex
                          CalibrateIMUFirmware.eep
                          AllZeros.hex
                          AllZeros.eep
                          AllOnes.hex
                          AllOnes.eep
                          AllTwos.hex
                          AllTwos.eep
                          AllFours.hex
                          AllFours.eep
                          AllEights.hex
                          AllEights.eep
                 )
add_custom_target(upload_Arduimu /usr/share/arduino/hardware/tools/avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:Arduimu.hex:i
                 DEPENDS Arduimu.hex
                 COMMENT Uploading Arduimu.hex
                 )
add_custom_target(upload_CalibrateIMUFirmware /usr/share/arduino/hardware/tools/avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:CalibrateIMUFirmware.hex:i
                 DEPENDS CalibrateIMUFirmware.hex
                 COMMENT Uploading CalibrateIMUFirmware.hex
                 )
add_custom_target(upload_AllZeros /usr/share/arduino/hardware/tools/avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:AllZeros.hex:i
                 DEPENDS AllZeros.hex
                 COMMENT Uploading AllZeros.hex
                 )
add_custom_target(upload_AllOnes /usr/share/arduino/hardware/tools/avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:AllOnes.hex:i
                 DEPENDS AllOnes.hex
                 COMMENT Uploading AllOnes.hex
                 )
add_custom_target(upload_AllTwos /usr/share/arduino/hardware/tools/avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:AllTwos.hex:i
                 DEPENDS AllTwos.hex
                 COMMENT Uploading AllTwos.hex
                 )
add_custom_target(upload_AllFours /usr/share/arduino/hardware/tools/avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:AllFours.hex:i
                 DEPENDS AllFours.hex
                 COMMENT Uploading AllFours.hex
                 )
add_custom_target(upload_AllEights /usr/share/arduino/hardware/tools/avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:AllEights.hex:i
                 DEPENDS AllEights.hex
                 COMMENT Uploading AllEights.hex
                 )

#set(ARDUINO_LIBRARY_SRCDIR "/usr/share/arduino/libraries")
#set(ARDUINO_LIBRARY_CORES  "/usr/share/arduino/hardware/arduino/cores/arduino")
add_library(Arduino STATIC
            ${ARDUINO_LIBRARY_SRCDIR}/EEPROM/EEPROM.cpp
            ${ARDUINO_LIBRARY_SRCDIR}/SPI/SPI.cpp
            ${ARDUINO_LIBRARY_SRCDIR}/Wire/Wire.cpp
            ${ARDUINO_LIBRARY_SRCDIR}/Wire/utility/twi.c
           )

set(ARDUIMUSRC ${JNISRC}/Engine/ArduIMU_V3)
add_library(IMU STATIC
            ${ARDUIMUSRC}/HMC58X3.cpp
            ${ARDUIMUSRC}/ITG3200.cpp
            ${ARDUIMUSRC}/MPU60X0.cpp
            ${ARDUIMUSRC}/MS561101BA.cpp
            ${ARDUIMUSRC}/bma180.cpp
           )

add_library(core STATIC
            ${ARDUINO_LIBRARY_CORES}/wiring_digital.c
            ${ARDUINO_LIBRARY_CORES}/wiring_analog.c
            ${ARDUINO_LIBRARY_CORES}/wiring_pulse.c
            ${ARDUINO_LIBRARY_CORES}/wiring_shift.c
            ${ARDUINO_LIBRARY_CORES}/wiring.c
            ${ARDUINO_LIBRARY_CORES}/WInterrupts.c
            ${ARDUINO_LIBRARY_CORES}/main.cpp
            ${ARDUINO_LIBRARY_CORES}/IPAddress.cpp
            ${ARDUINO_LIBRARY_CORES}/HardwareSerial.cpp
            ${ARDUINO_LIBRARY_CORES}/CDC.cpp
            ${ARDUINO_LIBRARY_CORES}/USBCore.cpp
            ${ARDUINO_LIBRARY_CORES}/new.cpp
            ${ARDUINO_LIBRARY_CORES}/WMath.cpp
            ${ARDUINO_LIBRARY_CORES}/Print.cpp
            ${ARDUINO_LIBRARY_CORES}/Stream.cpp
            ${ARDUINO_LIBRARY_CORES}/WString.cpp
            ${ARDUINO_LIBRARY_CORES}/HID.cpp
            ${ARDUINO_LIBRARY_CORES}/Tone.cpp
           )

add_executable(Arduimu.elf
               ${ARDUIMUSRC}/FreeIMU.cpp
               ${ARDUIMUSRC}/I2Cdev.cpp
               ${ARDUIMUSRC}/IMU.cpp
               ${ARDUIMUSRC}/CommunicationUtils.cpp
              )

add_executable(CalibrateIMUFirmware.elf
               ${ARDUIMUSRC}/CalibrateIMUFirmware.cpp
               ${ARDUIMUSRC}/FilteringScheme.cpp
               ${ARDUIMUSRC}/RunningAverage.cpp
               ${ARDUIMUSRC}/MovingAvarageFilter.cpp
               ${ARDUIMUSRC}/FreeIMU.cpp
               ${ARDUIMUSRC}/I2Cdev.cpp
               ${ARDUIMUSRC}/CommunicationUtils.cpp
               ${ARDUIMUSRC}/MovingAvarageFilter.cpp
              )

add_executable(AllZeros.elf
               ${ARDUIMUSRC}/AllZeros.cpp
               ${ARDUIMUSRC}/CommunicationUtils.cpp
              )
add_executable(AllOnes.elf
               ${ARDUIMUSRC}/AllOnes.cpp
               ${ARDUIMUSRC}/CommunicationUtils.cpp
              )
add_executable(AllTwos.elf
               ${ARDUIMUSRC}/AllTwos.cpp
               ${ARDUIMUSRC}/CommunicationUtils.cpp
              )
add_executable(AllFours.elf
               ${ARDUIMUSRC}/AllFours.cpp
               ${ARDUIMUSRC}/CommunicationUtils.cpp
              )
add_executable(AllEights.elf
               ${ARDUIMUSRC}/AllEights.cpp
               ${ARDUIMUSRC}/CommunicationUtils.cpp
              )

target_link_libraries(Arduimu.elf IMU Arduino core)
set_target_properties(Arduimu.elf PROPERTIES LINK_FLAGS -Wl,-lm,--gc-sections,-Map,Arduimu.map)

# Due to some symbols defined in both libm and libgcc force the libm linkage
# to libm only - by putting it first and using -nodefaultlibs. Otherwise get
# relocation errors because libgcc too far from libm.
target_link_libraries(CalibrateIMUFirmware.elf IMU Arduino core m gcc c gcc)
set_target_properties(CalibrateIMUFirmware.elf PROPERTIES LINK_FLAGS "-nodefaultlibs -Wl,-Map,CalibrateIMUFirmware.map,--gc-sections")

target_link_libraries(AllZeros.elf Arduino core)
set_target_properties(AllZeros.elf PROPERTIES LINK_FLAGS -Wl,-lm,--gc-sections,-Map,AllZeros.map)
target_link_libraries(AllOnes.elf Arduino core)
set_target_properties(AllOnes.elf PROPERTIES LINK_FLAGS -Wl,-lm,--gc-sections,-Map,AllOnes.map)
target_link_libraries(AllTwos.elf Arduino core)
set_target_properties(AllTwos.elf PROPERTIES LINK_FLAGS -Wl,-lm,--gc-sections,-Map,AllTwos.map)
target_link_libraries(AllFours.elf Arduino core)
set_target_properties(AllFours.elf PROPERTIES LINK_FLAGS -Wl,-lm,--gc-sections,-Map,AllFours.map)
target_link_libraries(AllEights.elf Arduino core)
set_target_properties(AllEights.elf PROPERTIES LINK_FLAGS -Wl,-lm,--gc-sections,-Map,AllEights.map)

