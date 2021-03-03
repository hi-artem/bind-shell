cmake_minimum_required(VERSION 2.8.12)
project(nc)

add_definitions("-std=c++11")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(nc main.cc)
target_link_libraries(nc ${CONAN_LIBS})
