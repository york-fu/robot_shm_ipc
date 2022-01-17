cmake_minimum_required(VERSION 3.0.0)
project(template VERSION 0.1.0)

add_compile_options(-std=c++11)

option(EXAMPLE "build example" ON)

include(CTest)
enable_testing()

add_subdirectory(src)

if(EXAMPLE)
  add_subdirectory(example)
endif(EXAMPLE)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
