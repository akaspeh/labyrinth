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