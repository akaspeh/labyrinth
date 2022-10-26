#include "Maze.h"
#include "Pathfinding.h"
#include "Vec2.h"

#include <memory>

int main()
{
    Maze maze(10, 6);

    maze.ShowMazeTextBold();

    Pathfinder finder(maze);
    std::vector<Vec2i> path = finder.pathfind(Vec2i(0), Vec2i(maze.Dimensions().x - 1, maze.Dimensions().y - 1), Vec2i::Manhattan);

    std::cout << std::endl << std::endl;

    maze.ShowMazeTextBold(path);

    std::cout << std::endl << std::endl;

    maze.ShowMazeText(path);
    return 0;
}