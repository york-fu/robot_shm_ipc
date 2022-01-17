cmake_minimum_required(VERSION 3.0.0)
project(cppShmIpc VERSION 0.1.0)

include_directories(./)
aux_source_directory(./ SRCS)

add_library(${PROJECT_NAME} ${SRCS})
# set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ./)
target_include_directories(${PROJECT_NAME} PUBLIC ./)
