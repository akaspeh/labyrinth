#include "Maze.h"
#include "Pathfinding.h"
#include "Vec2.h"

#include <memory>

int main()
{
	Maze maze(40, 10);

	Pathfinder finder(maze);
	std::vector<Vec2> path = finder.pathfind(Vec2{0, 0}, Vec2{7, 5}, Vec2::Manhattan);
	maze.ShowMaze(path);
	for (Vec2& vec : path)
	{
		std::cout << vec << std::endl;
	}
}