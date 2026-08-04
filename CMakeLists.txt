cmake_minimum_required(VERSION 3.10)

project(srb2deltaquest C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# prefer sdl2 by default
set(SRB2DQ_CONFIG_BACKEND "sdl2")

# main sources
add_subdirectory(src)
