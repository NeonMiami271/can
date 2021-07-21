cmake_minimum_required(VERSION 3.9)
project(green)

include_directories(
    ../
    ../../include
    ../../lib
)

set(sources
    canmon.c
)

add_definitions(-DLINUX)

add_executable(canmon ${sources})

target_link_libraries(canmon chai)
