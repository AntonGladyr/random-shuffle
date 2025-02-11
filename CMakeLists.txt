cmake_minimum_required(VERSION 3.25.1)

project(shuffle VERSION 1.0.0 LANGUAGES CXX)

# -----------------------------------------------------------------------------
# C++ Standard Configuration
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------------------------------------------------------
# Find Required Packages
# -----------------------------------------------------------------------------
find_package(OpenMP REQUIRED)

# -----------------------------------------------------------------------------
# Source Files
# -----------------------------------------------------------------------------
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# -----------------------------------------------------------------------------
# Target Definition
# -----------------------------------------------------------------------------
add_executable(shuffle ${SOURCES})

target_include_directories(shuffle PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(shuffle PRIVATE OpenMP::OpenMP_CXX)

# -----------------------------------------------------------------------------
# Compiler Options
# -----------------------------------------------------------------------------
if(MSVC)
    target_compile_options(shuffle PRIVATE /W4 /WX)
else()
    target_compile_options(shuffle PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
