cmake_minimum_required(VERSION 2.8.9)
project(expr)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-Wall -std=c++17)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
 
add_executable(expr ${SOURCES})