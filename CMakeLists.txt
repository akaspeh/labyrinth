cmake_minimum_required(VERSION 3.20)
project(Labyrinth LANGUAGES CXX)

set(LABYRINTH_SRC_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME})
set(LABYRINTH_INCLUDE_DIRS ${LABYRINTH_SOURCE_DIRS})

file(GLOB_RECURSE EXECUTABLE_SRC 
    ${LABYRINTH_SRC_DIRS}/*.cpp
    )

add_executable(Labyrinth ${EXECUTABLE_SRC} "src/Labyrinth/Maze.h" "src/Labyrinth/Maze.cpp")

target_include_directories(Labyrinth 
    PUBLIC ${LABYRINTH_INCLUDE_DIRS}
    )