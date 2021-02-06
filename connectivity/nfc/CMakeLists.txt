# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
project(mbed-nfc LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

# DG hack for testing


# Testing!
# From https://github.com/google/googletest/blob/master/googletest/README.md

# Download and unpack googletest at configure time
configure_file(GoogleTest-CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(munfc-test)
target_compile_features(munfc-test PUBLIC cxx_std_17)
target_link_libraries(munfc-test mbed-nfc gtest_main)
add_test(NAME munfc-test COMMAND munfc-test)

add_library(mbed-nfc STATIC)
target_compile_features(mbed-nfc PUBLIC c_std_11)
add_subdirectory(libraries)

target_include_directories(mbed-nfc
    PUBLIC
        .
        ./include
        ./include/nfc
        ./include/nfc/ndef
        ./include/nfc/ndef/common
)

target_sources(mbed-nfc
    PRIVATE
        # source/NFCController.cpp
        # source/NFCControllerDriver.cpp
        # source/NFCEEPROM.cpp
        # source/NFCEEPROMDriver.cpp
        # source/NFCNDEFCapable.cpp
        # source/NFCRemoteEndpoint.cpp
        # source/NFCRemoteInitiator.cpp
        # source/NFCTarget.cpp
        # source/Type4RemoteInitiator.cpp

        # source/ndef/MessageBuilder.cpp
        # source/ndef/MessageParser.cpp
        # source/ndef/RecordParser.cpp

        # source/ndef/common/Mime.cpp
        # source/ndef/common/SimpleMessageParser.cpp
        # source/ndef/common/Text.cpp
        # source/ndef/common/URI.cpp
        # source/ndef/common/util.cpp
)

target_compile_definitions(mbed-nfc
    PUBLIC
        MBED_CONF_NFC_PRESENT=1
)

# symlinks
add_subdirectory(drivers/nfc/PN512)
# add_subdirectory(events)
# add_subdirectory(cxxsupport)

# target_link_libraries(mbed-nfc
#    PRIVATE
#        mbed-cxx
#        mbed-events
# )
