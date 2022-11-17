#pragma once

#include "Pathfinding.h"
#include "Maze.h"
#include "utility/Vec2.h"

class Menu 
{
public:
    Menu() = default;
    ~Menu();

    void CreateMaze();

    void ShowMaze() const;

    // this method should become const, because Maze will soon become immutable towards pathfinder
    void PathFind();

    // void CreateMazeFromTemplate(){}

private:
    Maze* m_maze = nullptr;

};
