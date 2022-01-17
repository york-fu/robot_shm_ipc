cmake_minimum_required(VERSION 3.0.0)
project(example VERSION 0.1.0)

include_directories(./)

add_executable(state ./state.cpp)
target_link_libraries(state cppShmIpc)

add_executable(command ./command.cpp)
target_link_libraries(command cppShmIpc)
