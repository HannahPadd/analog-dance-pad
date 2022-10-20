cmake_minimum_required (VERSION 3.6)

# GLFW
set(GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/GLFW")

option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)

find_package(Vulkan REQUIRED)
add_subdirectory (${GLFW_DIR})

# ImGui
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui")

include_directories(
	${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${GLFW_DIR}/deps
)

add_library(imgui
    # Already being included by Application.cpp
    #${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    #${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp

    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

target_link_libraries(imgui PUBLIC glfw Vulkan::Vulkan)