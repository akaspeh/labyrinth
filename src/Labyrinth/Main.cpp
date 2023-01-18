
#include "Application.h"
#include "Maze.h"

#include <memory>

int main()
{
    std::shared_ptr<MazeFactory> mazeFactory = std::make_shared<SimpleMazeCreator>();
    Application::Init(mazeFactory,20,20);
    Application* app = Application::GetInstance();

    app->GetBattleContext().GetRobotManager().AddRobot<SlowRobot>(app->GetMaze(),Vec2i(0),Vec2i(10));
    app->GetBattleContext().GetRobotManager().AddRobot<SimpleRobot>(app->GetMaze(),Vec2i(0,19),Vec2i(10));
    app->GetBattleContext().GetRobotManager().AddRobot<AngryRobot>(app->GetMaze(),Vec2i(19,0),Vec2i(10));
    app->GetBattleContext().GetRobotManager().AddRobot<BoomRobot>(app->GetMaze(),Vec2i(19),Vec2i(10),30);
    app->Run();
    Application::Deinit();
    return 0;
}