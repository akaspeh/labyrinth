#include "Battle.h"

void BattleContext::Reset()
{
    m_maze->UpdateMaze();
    m_robotManager.Reset();
    m_closed = false;
}

void BattleContext::Run()
{
    std::array<size_t, static_cast<size_t>(Robots::UNKNOWN)> steps;
    steps.fill(-1);
    static constexpr char VALID_CHAR = 'a';
    while (!ShouldClose())
    {
        std::cout << "[LOG]: Battle continues!\n";
        std::cout << "[LOG]: Enter 'a' to keep iterating, enter any other symbol to stop game loop\n";
        char c;
        std::cin >> c;
        CLEAR_SCREEN();
        if (c != VALID_CHAR)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Closed!\n";
            Close();
        }

        Vec2i goal = m_robotManager.GetRobots()[0]->getGoal();

        MazePrinter::PrintInConsoleRobots(m_maze.get(), m_robotManager.GetRobots(), goal);

        for (IRobot* robot : m_robotManager.GetRobots())
        {
            if (!robot->move())
            {
                ++steps[static_cast<size_t>(robot->getRobotType())];
            }
        }

        std::cout << std::endl;
        std::cout << "[LOG]: AngryRobot steps - " << steps[static_cast<size_t>(Robots::ANGRY)] << "." << std::endl;
        std::cout << "[LOG]: BoomRobot steps - " << steps[static_cast<size_t>(Robots::BOOM)] << "." << std::endl;
        std::cout << "[LOG]: SimpleRobot steps - " << steps[static_cast<size_t>(Robots::SIMPLE)] << "." << std::endl;
        std::cout << "[LOG]: SlowRobot steps - " << steps[static_cast<size_t>(Robots::SLOW)] << "." << std::endl;
        std::cout << std::endl;

        if(m_maze->getUpdateState())
        {
            for (IRobot* robot : m_robotManager.GetRobots())
            {
                robot->UpdatePath();
            }
            m_maze->handleUpdate();
        }
    }
    Reset();
}