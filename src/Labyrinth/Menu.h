#pragma once

#include "Pathfinding.h"
#include "Maze.h"
#include "Vec2.h"


class Menu {
private:
   Maze m_maze{2,2};
public:

    void CreateMaze(){
        int width,height;
        std::cout << "Enter width\n";
        std::cin >> width;
        std::cout << "Enter height\n";
        std::cin >> height;
        m_maze.ChangeSize(width, height);
        m_maze.CreateMaze();
    }

    void ShowMaze(){
        m_maze.ShowMazeTextBold();
        std::cout << std::endl << std::endl;
        m_maze.ShowMazeText();

    }

    void PathFind(){

        Pathfinder finder(m_maze);
        std::vector<Vec2i> path = finder.pathfind(Vec2i(0), Vec2i(m_maze.Dimensions().x - 1, m_maze.Dimensions().y - 1), Vec2i::Manhattan);

        std::cout << std::endl << std::endl;

        m_maze.ShowMazeTextBold(path);
    }

    // void CreateMazeFromTemplate(){}
};
