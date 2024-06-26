cmake_minimum_required(VERSION 3.0)

add_definitions(-D_BSD_SOURCE -D_DEFAULT_SOURCE)   # To enable various useful macros and functions on Unices
remove_definitions(-D_UNICODE -DUNICODE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Workaround for an old CMake, which does not understand CMAKE_C_STANDARD.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")
endif()


set(AVRDUDE_SOURCES
    arduino.c
    avr.c
    # avrftdi.c
    # avrftdi_tpi.c
    avrpart.c
    avr910.c
    bitbang.c
    buspirate.c
    butterfly.c
    config.c
    config_gram.c
    # confwin.c
    crc16.c
    # dfu.c
    fileio.c
    # flip1.c
    # flip2.c
    # ft245r.c
    # jtagmkI.c
    # jtagmkII.c
    # jtag3.c
    lexer.c
    linuxgpio.c
    lists.c
    # par.c
    pgm.c
    pgm_type.c
    pickit2.c
    pindefs.c
    # ppi.c
    # ppiwin.c
    safemode.c
    ser_avrdoper.c
    serbb_posix.c
    serbb_win32.c
    ser_posix.c
    ser_win32.c
    stk500.c
    stk500generic.c
    stk500v2.c
    term.c
    update.c
    # usbasp.c
    # usb_hidapi.c
    # usb_libusb.c
    # usbtiny.c
    wiring.c

    main.c
    avrdude-slic3r.hpp
    avrdude-slic3r.cpp
)
if (MSVC)
    set(AVRDUDE_SOURCES ${AVRDUDE_SOURCES}
        windows/utf8.c
        windows/unistd.cpp
        windows/getopt.c
    )
elseif (MINGW)
    set(AVRDUDE_SOURCES ${AVRDUDE_SOURCES}
        windows/utf8.c
    )
endif()

include(bin2h)

bin2h(
    SOURCE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/avrdude-slic3r.conf
    VARIABLE_NAME avrdude_slic3r_conf
    HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/avrdude-slic3r.conf.h
    ADD_WARNING_TEXT
)

add_library(avrdude STATIC ${AVRDUDE_SOURCES})

#add_executable(avrdude-slic3r main-standalone.cpp)
#target_link_libraries(avrdude-slic3r avrdude)

#encoding_check(avrdude)
#encoding_check(avrdude-slic3r)

# Make avrdude-slic3r.conf.h includable:
target_include_directories(avrdude SYSTEM PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

if (WIN32)
    target_compile_definitions(avrdude PRIVATE WIN32NATIVE=1)
    if(MSVC)
        target_include_directories(avrdude SYSTEM PRIVATE windows)    # So that sources find the getopt.h windows drop-in
    endif(MSVC)
endif()
