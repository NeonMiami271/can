cmake_minimum_required(VERSION 3.9)
project(chaitest)

include_directories(
    ../
    ../../include
    ../../lib
)

set(sources
    chaitest.c
)

add_definitions(-DLINUX)

add_executable(chaitest ${sources})

target_link_libraries(chaitest chai)