cmake_minimum_required(VERSION 3.25.1)

project(shuffle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(shuffle.x "")

target_sources(
  shuffle.x
  PRIVATE
    shuffle.cpp
  )
