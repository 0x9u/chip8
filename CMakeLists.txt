cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES "src/*.cpp")

add_executable(chip8 ${SOURCES})