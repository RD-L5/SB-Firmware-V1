cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(pico_sdk_import.cmake)

project(SMTBED C CXX ASM)

pico_sdk_init()


add_subdirectory(freertos)
add_subdirectory(main)