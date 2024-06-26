cmake_minimum_required (VERSION 3.6)

if(WIN32)
	set(wxBUILD_SHARED OFF CACHE BOOL "Static build")
	add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/lib/wxWidgets)
	set(WX_LIBS wx::net wx::core wx::base)
	#set(WX_SETUP_INCLUDE "${CMAKE_CURRENT_BINARY_DIR}/lib/vc_lib/mswud")
endif()

if(UNIX)
	find_package(wxWidgets REQUIRED COMPONENTS net core base)
    include(${wxWidgets_USE_FILE})
	set(WX_LIBS ${wxWidgets_LIBRARIES})
	#set(WX_SETUP_INCLUDE "${CMAKE_CURRENT_BINARY_DIR}/lib/wx/include/gtk3-unicode-static-3.1")
endif()