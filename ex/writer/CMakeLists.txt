cmake_minimum_required(VERSION 3.5)

project(writer)

set(sources
    main.cpp)

include_directories(../../include)

link_directories(../../lib)

add_executable(writer ${sources})

target_link_libraries(writer chai)
