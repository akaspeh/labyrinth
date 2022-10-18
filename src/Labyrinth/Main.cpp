#include "Application.h"
#include "Maze.h"

#include <memory>

int main()
{
	std::unique_ptr<Application> application = std::make_unique<Application>();
	application->run();
	
	Maze maze(40, 10);
	maze.ShowMaze();
}