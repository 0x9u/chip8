cmake_minimum_required(VERSION 3.10)

project(chip8 VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "src/*.cpp")

add_executable(chip8 ${SOURCES})

find_package(SDL2 REQUIRED)

target_include_directories(chip8 PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(chip8 PRIVATE ${SDL2_LIBRARIES})

target_compile_options(chip8 PRIVATE -fno-omit-frame-pointer -fsanitize=address,undefined -g)
target_link_options(chip8 PRIVATE -fsanitize=address,undefined -g)