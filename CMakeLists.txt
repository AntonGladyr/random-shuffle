cmake_minimum_required(VERSION 3.25.1)

project(shuffle VERSION 1.0.0 LANGUAGES CXX)

# -----------------------------------------------------------------------------
# C++ Standard Configuration
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------------------------------------------------------
# Source Files
# -----------------------------------------------------------------------------
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

# -----------------------------------------------------------------------------
# Target Definition
# -----------------------------------------------------------------------------
add_executable(shuffle ${SOURCES})

target_include_directories(shuffle PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -----------------------------------------------------------------------------
# Compiler Options
# -----------------------------------------------------------------------------
if(MSVC)
    target_compile_options(shuffle PRIVATE /W4 /WX)
else()
    target_compile_options(shuffle PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
