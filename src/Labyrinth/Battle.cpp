#include "Battle.h"

#include <thread>

void BattleContext::Reset()
{
    m_maze->UpdateMaze();
    m_robotManager.Reset();
    m_closed = false;
}

void BattleContext::Run()
{
    using namespace std::chrono_literals;
    
    std::array<size_t, static_cast<size_t>(Robots::UNKNOWN)> steps;
    steps.fill(-1);
    while (!ShouldClose())
    {
        std::cout << "[LOG]: Battle continues!\n";
        std::this_thread::sleep_for(300ms);
        CLEAR_SCREEN();

        Vec2i goal = m_robotManager.GetRobots()[0]->getGoal();
        MazePrinter::PrintInConsoleRobots(m_maze.get(), m_robotManager.GetRobots(), goal);

        size_t arrived = 0;
        for (IRobot* robot : m_robotManager.GetRobots())
        {
            robot->move();
            if (robot->isArrived())
            {
                ++arrived;
                continue;
            }
            ++steps[static_cast<size_t>(robot->getRobotType())];
        }
        if (arrived == steps.size())
        {
            std::cout << "[LOG]: All robots arrived to goal!\n";
            Close();
        }

        if(m_maze->getUpdateState())
        {
            for (IRobot* robot : m_robotManager.GetRobots())
            {
                robot->UpdatePath();
            }
            m_maze->handleUpdate();
        }
    }

    std::cout << "[LOG]: Battle ended!\n";
    std::cout << std::endl;

    std::cout << "[LOG]: AngryRobot steps - " << steps[static_cast<size_t>(Robots::ANGRY)] << "." << std::endl;
    std::cout << "[LOG]: BoomRobot steps - " << steps[static_cast<size_t>(Robots::BOOM)] << "." << std::endl;
    std::cout << "[LOG]: SimpleRobot steps - " << steps[static_cast<size_t>(Robots::SIMPLE)] << "." << std::endl;
    std::cout << "[LOG]: SlowRobot steps - " << steps[static_cast<size_t>(Robots::SLOW)] << "." << std::endl;
    std::cout << std::endl;

    std::cout << "[LOG]: Enter any symbol to return to the menu\n";

    char c;
    std::cin >> c;
    CLEAR_SCREEN();
    
    Reset();
}