cmake_minimum_required(VERSION 3.20)
project(Labyrinth LANGUAGES CXX)

option(LABYRINTH_DEBUG "Enable debug mode" OFF)

set(CMAKE_CXX_STANDARD 20)

set(LABYRINTH_SRC_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME})
set(LABYRINTH_INCLUDE_DIRS ${LABYRINTH_SRC_DIRS})

file(GLOB_RECURSE EXECUTABLE_SRC 
    ${LABYRINTH_SRC_DIRS}/*.cpp
    )

if (LABYRINTH_DEBUG)
    # For future debugging features
    add_compile_definitions(LABYRINTH_DEBUG)
    add_definitions(-Wall -Wextra)
endif()

add_executable(Labyrinth ${EXECUTABLE_SRC} "src/Labyrinth/MazePrinter.h" "src/Labyrinth/utility/RandomGenerator.h"   "src/Labyrinth/utility/RandomGenerator.cpp")

target_include_directories(Labyrinth 
    PUBLIC ${LABYRINTH_INCLUDE_DIRS}
    )