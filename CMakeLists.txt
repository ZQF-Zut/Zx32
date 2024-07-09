# Zx32 Main
cmake_minimum_required(VERSION 3.20)

# Project Name
project(Zut_Zx32_Main)

# Only MSVC
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(FATAL_ERROR "Zut::Zx32 Only Windows")
endif()

# On Test Preset
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    include(CMakeMycfg.cmake)
endif()

# Source
add_subdirectory(src)

# On Test Config
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(test)
endif()