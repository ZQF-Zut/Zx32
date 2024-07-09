# OutPath
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.bin/x32-${CMAKE_BUILD_TYPE}/")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.bin/x64-${CMAKE_BUILD_TYPE}/")
endif()

# Link
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # LTCG
    cmake_policy(SET CMP0069 NEW) 
    set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT IPO_CHECK_RESULT OUTPUT output)
    if(IPO_CHECK_RESULT)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
    else()
        message(WARNING "IPO is not supported: ${output}")
    endif()

    # Static Library
    set(BUILD_SHARED_LIBS OFF)

    # Static Runtime
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
endif()

# Compiler
if(MSVC)
    # Macro
    add_definitions(-DUNICODE -D_UNICODE)
    
    # Compile Flags
    add_compile_options(/W4)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Gy)
        add_compile_options(/Zc:inline)
    endif()
else()
    add_compile_options(-Wextra)
endif()
