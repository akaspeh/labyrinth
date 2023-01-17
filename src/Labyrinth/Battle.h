#pragma once

#include <vector>
#include <memory>

#include "Robot.h"

template<typename T>
static constexpr bool isRobot = std::is_base_of_v<IRobot, T>;

class RobotManager
{
public:
    RobotManager(const std::shared_ptr<Maze>& maze)
        : m_maze(maze)
    {
    }
    ~RobotManager() = default;

    template<typename T>
    std::enable_if_t<isRobot<T>, T*> AddRobot()
    {
        return (T*) m_robots.push_back(new T(m_maze));
    }

    inline constexpr std::vector<IRobot*>& GetRobots() { return m_robots; }
    inline constexpr const std::vector<IRobot*>& GetRobots() const { return m_robots; }

private:
    std::vector<IRobot*> m_robots;
    std::shared_ptr<Maze> m_maze;
}; // RobotManager class

class BattleContext
{
public:
    BattleContext(const std::shared_ptr<Maze>& maze)
        : m_maze(maze)
        , m_closed(false)
        , m_robotManager(maze)
    {
    }
    ~BattleContext() = default;

    void Run()
    {
        static constexpr char VALID_CHAR = ' '; // space
        while (!ShouldClose())
        {
            char c;
            std::cin >> c;
            std::cout << c << std::endl;
            if (c != VALID_CHAR)
            {
                std::cout << "Closed!\n";
                Close();
            }

            std::cout << "Iteration!\n";
            for (IRobot* robot : m_robotManager.GetRobots())
            {
                // Do something with robots
                std::cout << "Doint something for robots\n";
            }
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