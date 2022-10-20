cmake_minimum_required (VERSION 3.6)

set(NFD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/nfd")

include_directories(
	${NFD_PATH}/src/include
)

SET(NFD_SRC
    ${NFD_PATH}/src/nfd_common.c
)

if(WIN32)
    list(APPEND NFD_SRC ${NFD_PATH}/src/nfd_win.cpp)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    
    list(APPEND NFD_SRC ${NFD_PATH}/src/nfd_gtk.c)
endif()

add_library(nfd ${NFD_SRC})
target_link_libraries(nfd ${GTK3_LIBRARIES})
target_include_directories(nfd PRIVATE ${GTK3_INCLUDE_DIRS})