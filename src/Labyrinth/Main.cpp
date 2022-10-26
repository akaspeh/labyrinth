#include "Maze.h"
#include "Pathfinding.h"
#include "Vec2.h"

#include <memory>

int main()
{
    Maze maze(10, 6);

    maze.ShowMazeTextBold();

    Pathfinder finder(maze);
    std::vector<Vec2> path = finder.pathfind(Vec2{ 0, 0 }, Vec2{ maze.Dimensions().x - 1, maze.Dimensions().y - 1 }, Vec2::Manhattan);

    std::cout << std::endl << std::endl;

    maze.ShowMazeTextBold(&path);

    std::cout << std::endl << std::endl;

    maze.ShowMazeText(&path);

    /*std::cout << std::endl << std::endl;

    for (Vec2& vec : path)
    {
        std::cout << vec << std::endl;
    }*/

    return 0;
}