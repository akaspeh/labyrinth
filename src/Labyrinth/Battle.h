#pragma once

#include <vector>
#include <memory>
#include <limits>

#include "Maze.h"
#include "Robot.h"

#if defined(_WIN32) || defined(WIN32)
    #define CLEAR_SCREEN() system("cls")
#else
    #define CLEAR_SCREEN() system("clear")
#endif

class BattleContext
{
public:
    BattleContext(const std::shared_ptr<Maze>& maze)
        : m_maze(maze)
        , m_closed(false)
        , m_robotManager(maze,std::make_shared<Pathfinder>(maze))
    {
    }
    ~BattleContext() = default;

    void Run()
    {
        m_robotManager.AddRobot<BoomRobot>(m_maze,Vec2i(0),Vec2i(5),100);
        static constexpr char VALID_CHAR = 'a';
        m_closed = false;
        while (!ShouldClose())
        {
            std::cout << "Battle continues!\n";
            std::cout << "Enter 'a' to keep iterating, enter any other symbol to stop game loop\n";
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

            std::cout << "Iteration!\n";
            // TODO: Add pathfinding for each robot, update their paths and positions
            for (IRobot* robot : m_robotManager.GetRobots())
            {
                robot->move();
                std::cout << "Doing something for robots\n";
            }

            MazePrinter::PrintInConsole(m_maze.get());
        }
    }

    inline constexpr bool ShouldClose() const { return m_closed; }
    inline void Close() { m_closed = true; }

    inline constexpr RobotManager& GetRobotManager() { return m_robotManager; } 
    inline constexpr const RobotManager& GetRobotManager() const { return m_robotManager; } 

private:
    std::shared_ptr<Maze> m_maze;
    bool m_closed;
    RobotManager m_robotManager;
}; // Game class