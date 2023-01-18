
#include "Application.h"
#include "Maze.h"

#include <memory>

int main()
{
    std::shared_ptr<MazeFactory> mazeFactory = std::make_shared<SimpleMazeCreator>();
    Application::Init(mazeFactory);
    Application* app = Application::GetInstance();
    app->Run();
    Application::Deinit();
    return 0;
}