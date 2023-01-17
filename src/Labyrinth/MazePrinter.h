#pragma once
#include "Maze.h"

class MazePrinter
{
public:
    MazePrinter() = delete;
    ~MazePrinter() = delete;
    MazePrinter(const MazePrinter& mazePrinter) = delete;
    MazePrinter operator=(const MazePrinter& mazePrinter) = delete;

    static void PrintInConsole(Maze* maze, 
        std::optional<Maze::cref_type<Maze::path_container_type>> path = std::nullopt);
    static void PrintInConsoleBold(Maze* maze,
        std::optional<Maze::cref_type<Maze::path_container_type>> path = std::nullopt);
};