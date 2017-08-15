cmake_minimum_required (VERSION 2.6)

project (slider)

set (CMAKE_CXX_STANDARD 11)

add_executable(slider slider/main.cpp slider/context.cpp slider/cart.cpp)

