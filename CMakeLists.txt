cmake_minimum_required (VERSION 2.6)

project (slider)

set (CMAKE_CXX_STANDARD 11)

#include_directories(~/wirinPi/wiringPi)

SET(CMAKE_CXX_FLAGS -lwiringPi)

#SET(CMAKE_LIBRARY_PATH ~/wirinPi/wiringPi)
set(SOURCES slider/main.cpp slider/context.cpp slider/cart.cpp slider/manage_execution.cpp)

add_executable(slider ${SOURCES})

