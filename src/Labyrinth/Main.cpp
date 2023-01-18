
#include "Application.h"
#include "Maze.h"

#include <memory>

int main()
{
    std::shared_ptr<MazeFactory> mazeFactory = std::make_shared<SimpleMazeCreator>();
    std::unique_ptr<Application> application = std::make_unique<Application>(mazeFactory);
    application->Run();
    return 0;
}