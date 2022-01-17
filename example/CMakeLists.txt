cmake_minimum_required(VERSION 3.0.0)
project(example VERSION 0.1.0)

include_directories(./)

add_executable(driver ./driver.cpp)
target_link_libraries(driver cppShmIpc)

add_executable(controller ./controller.cpp)
target_link_libraries(controller cppShmIpc)

add_executable(monitor ./monitor.cpp)
target_link_libraries(monitor cppShmIpc)
