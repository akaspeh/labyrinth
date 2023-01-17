#pragma once

#include <memory>

#include "Maze.h"

class IRobot
{
public:
    IRobot(const std::shared_ptr<Maze>& maze)
        : m_maze(maze)
    {
    }
    virtual ~IRobot() = default;

    virtual void move(const Vec2i& pos) = 0;

protected:
    Vec2i m_pos;
    std::shared_ptr<Maze> m_maze;
}; // IRobot class

class FastRobot : public IRobot
{
public:
    FastRobot(const std::shared_ptr<Maze>& maze)
        : IRobot(maze)
    {
    }
    virtual ~FastRobot() = default;

    virtual void move(const Vec2i& pos)
    {
        // Do smth
    }
}; // FastRobot class

class SlowRobot : public IRobot
{
public:
    SlowRobot(const std::shared_ptr<Maze>& maze)
        : IRobot(maze)
    {
    }
    virtual ~SlowRobot() = default;

    virtual void move(const Vec2i& pos)
    {
        // Do smth
    }
}; // SlowRobot class

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