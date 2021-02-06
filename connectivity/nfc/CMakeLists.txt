# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
project(mbed-nfc LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

# DG hack for testing

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
