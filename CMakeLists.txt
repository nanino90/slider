cmake_minimum_required (VERSION 2.8)
project (slider)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
        message(FATAL_ERROR "GCC version >=4.9 required.")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.3)
        message(FATAL_ERROR "Clang version >=3.3 required.")
    endif()
else()
    message(WARNING "Your compiler (${CMAKE_CXX_COMPILER_ID}) has not been tested on this project. Only Clang and GCC has been tested. Please report any problems at the project page on GitHub.")
endif()

find_package(Boost 1.54.0 COMPONENTS system thread coroutine context filesystem REQUIRED)
message("Boost include dir: ${Boost_INCLUDE_DIR}")
message("Boost libraries: ${Boost_LIBRARIES}")
include_directories(${Boost_INCLUDE_DIR})

#TODO: add requirement for version 1.0.1g (can it be done in one line?)
find_package(Threads REQUIRED)
message("Threads libraries/flag: ${CMAKE_THREAD_LIBS_INIT}")

include_directories(slider)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -lwiringPi)

set(SLIDER_SOURCES slider/main.cpp slider/context.cpp slider/cart.cpp slider/parse.cpp)

add_executable(slider ${SLIDER_SOURCES})
target_link_libraries(slider ${Boost_LIBRARIES})
target_link_libraries(slider ${CMAKE_THREAD_LIBS_INIT})

