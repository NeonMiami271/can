cmake_minimum_required(VERSION 3.9)
project(green)

include_directories(
    ../
    ../../include
    ../../lib
)

set(sources
    power.c
)

add_definitions(-DLINUX)

add_library(power SHARED ${sources})

target_link_libraries(power chai)
