#include "Battle.h"

void BattleContext::Reset()
{
    m_robotManager.Reset();
    m_closed = false;
}

void BattleContext::Run()
{
    Reset();
    m_robotManager.AddRobot<BoomRobot>(m_maze,Vec2i(0),Vec2i(5),100);
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

        for (IRobot* robot : m_robotManager.GetRobots())
        {
            robot->move();
        }

        if(m_maze->getUpdateState())
        {
            for (IRobot* robot : m_robotManager.GetRobots())
            {
                robot->UpdatePath();
            }
            m_maze->handleUpdate();
        }

        MazePrinter::PrintInConsoleRobots(m_maze.get(), m_robotManager.GetRobots());
    }
}