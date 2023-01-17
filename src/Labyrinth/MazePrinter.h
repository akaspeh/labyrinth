#pragma once
#include "Maze.h"

class MazePrinter
{
public:
    using path_container_type = std::vector<Vec2i>;
    template<typename T> using ref_type = std::reference_wrapper<T>;
    template<typename T> using cref_type = ref_type<std::add_const_t<T>>;
public:
    MazePrinter() = delete;
    ~MazePrinter() = delete;
    MazePrinter(const MazePrinter& mazePrinter) = delete;
    MazePrinter operator=(const MazePrinter& mazePrinter) = delete;

    static void PrintInConsole(Maze* maze, 
        std::optional<cref_type<path_container_type>> path = std::nullopt);
    static void PrintInConsoleBold(Maze* maze,
        std::optional<cref_type<path_container_type>> path = std::nullopt);
};