#pragma once

#include <algorithm>
#include <memory>
#include <array>

#include "utility/RandomGenerator.h"
#include "Pathfinding.h"
#include "Maze.h"


class IRobot
{
public:
    IRobot(const std::shared_ptr<Pathfinder>& pathfinder,
           const std::shared_ptr<Maze>& maze, const Vec2i& start, const Vec2i& goal)
        : m_pos(start)
        , m_maze(maze)
        , m_start(start)
        , m_goal(goal)
        , m_finder(pathfinder)
    {
        UpdatePath();
    }
    virtual ~IRobot() = default;

    void UpdatePath()
    {
        m_path = m_finder->invoke(m_start,m_goal,Vec2i::Manhattan);
    }

    inline constexpr Vec2i getPos() const { return m_pos; }

    virtual bool move() = 0;

protected:
    Vec2i m_pos;
    Vec2i m_start;
    Vec2i m_goal;
    std::vector<Vec2i> m_path;
    std::shared_ptr<Maze> m_maze;
    std::shared_ptr<Pathfinder> m_finder;
}; // IRobot class

class BoomRobot : public IRobot
{
public:
    BoomRobot(const std::shared_ptr<Pathfinder>& pathfinder,
    const std::shared_ptr<Maze>& maze, const Vec2i& start, const Vec2i& goal,int32_t chance)
        : IRobot(pathfinder,maze,start,goal)
        , m_chance(std::min(chance,100))
    {
    }
    virtual ~BoomRobot() = default;

    bool boom()
    {
        int32_t result = RandomGenerator::generateIndex(0,100);

        if(!(result < m_chance)){
            return false;
        }

        static constexpr std::array<Vec2i, 4> directions = {
                Vec2i( 0, -1),
                Vec2i( 1,  0),
                Vec2i( 0,  1),
                Vec2i(-1,  0),
        };
        for(const Vec2i& delta : directions)
        {
            m_maze->breakWall(m_pos,delta);
        }
        return true;
    }

    virtual bool move() override
    {
        std::cout << "start!\n";
        if(m_path.empty())
        {
            return true;
        }
        m_pos = m_path.front();
        m_path.erase(m_path.begin());
        std::cout << "preboom!\n";
        if(boom())
        {
            std::cout << "BOOM!\n";
        }
        return false;
    }
private:
    int32_t m_chance;
}; // BoomRobot class

class MediumRobot : public IRobot
{
public:
    MediumRobot(const std::shared_ptr<Pathfinder>& pathfinder,
                const std::shared_ptr<Maze>& maze, const Vec2i& start, const Vec2i& goal)
            : IRobot(pathfinder,maze,start,goal)
    {
    }
    virtual ~MediumRobot() = default;

    virtual bool move()
    {
        if(m_path.empty())
        {
            return true;
        }
        m_pos = m_path.front();
        m_path.erase(m_path.begin());
    }
}; // MediumRobot class

class SlowRobot : public IRobot
{
public:
    SlowRobot(const std::shared_ptr<Pathfinder>& pathfinder,
              const std::shared_ptr<Maze>& maze, const Vec2i& start, const Vec2i& goal)
        : IRobot(pathfinder,maze,start,goal)
    {
    }
    virtual ~SlowRobot() = default;

    virtual bool move()
    {

    }
}; // SlowRobot class

class AngryRobot : public IRobot
{
public:
    AngryRobot(const std::shared_ptr<Maze>& maze, const Vec2i& start, const Vec2i& goal,
               const std::shared_ptr<Pathfinder>& pathfinder)
            : IRobot(pathfinder,maze,start,goal)
    {
    }
    virtual ~AngryRobot() = default;

    virtual bool move()
    {
        // Do smth
    }
}; // AngryRobot class



template<typename T>
static constexpr bool isRobot = std::is_base_of_v<IRobot, T>;

class RobotManager
{
public:
    RobotManager(const std::shared_ptr<Maze>& maze,const std::shared_ptr<Pathfinder>& pathfinder)
        : m_maze(maze)
        , m_pathfinder(pathfinder)
    {
    }
    ~RobotManager() = default;

    template<typename T, typename... Args>
    std::enable_if_t<isRobot<T>, T*> AddRobot(Args&&... args)
    {
        IRobot* robot = m_robots.emplace_back(new T (m_pathfinder, std::forward<Args>(args)...));
        return (T*) robot;
    }

    inline constexpr std::vector<IRobot*>& GetRobots() { return m_robots; }
    inline constexpr const std::vector<IRobot*>& GetRobots() const { return m_robots; }

private:
    std::vector<IRobot*> m_robots;
    std::shared_ptr<Maze> m_maze;
    std::shared_ptr<Pathfinder> m_pathfinder;
}; // RobotManager class