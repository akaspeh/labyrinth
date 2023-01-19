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
        , m_robotManager(maze, std::make_shared<Pathfinder>(maze))
    {
    }
    ~BattleContext() = default;

    void Reset();
    void Run(); 

    inline constexpr bool ShouldClose() const { return m_closed; }
    inline void Close() { m_closed = true; }

    inline constexpr RobotManager& GetRobotManager() { return m_robotManager; }
    inline constexpr const RobotManager& GetRobotManager() const { return m_robotManager; }

private:
    std::shared_ptr<Maze> m_maze;
    bool m_closed;
    RobotManager m_robotManager;
}; // Game class