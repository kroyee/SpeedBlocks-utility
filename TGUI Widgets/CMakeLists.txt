cmake_minimum_required(VERSION 3.5)

set(EXECUTABLE_NAME tgui.widgets)

project(${EXECUTABLE_NAME})

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -pedantic -O3")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/*.cpp ${CMAKE_SOURCE_DIR}/*.h)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

#target_include_directories(${EXECUTABLE_NAME} PUBLIC path_here)

add_custom_target(run
    COMMAND ${EXECUTABLE_NAME}
    DEPENDS ${EXECUTABLE_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

find_package(SFML 2 COMPONENTS audio network graphics window system)
if(NOT SFML_FOUND)
    set(SFML_ROOT "" CACHE PATH "SFML root directory")
    message(FATAL_ERROR "CMake couldn't find SFML. Set the SFML_ROOT entry to SFML's root directory (containing \"include\" and \"lib\" directories).")
endif()
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

find_package(TGUI 0.8)
if(NOT TGUI_FOUND)
    set(TGUI_ROOT "" CACHE PATH "TGUI root directory")
    message(FATAL_ERROR "CMake couldn't find TGUI. Set the TGUI_ROOT entry to TGUI's root directory (containing \"include\" and \"lib\" directories).")
endif()
include_directories(${TGUI_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${TGUI_LIBRARY})
