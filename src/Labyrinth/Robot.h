#pragma once

#include <algorithm>
#include <memory>
#include <array>

#include "utility/RandomGenerator.h"
#include "Pathfinding.h"
#include "Maze.h"

enum class Robots
{
    ANGRY,
    BOOM,
    SIMPLE,
    SLOW,
    UNKNOWN
};

class IRobot
{
public:
    IRobot(const std::shared_ptr<Pathfinder>& pathfinder,
           const std::shared_ptr<Maze>& maze, const Vec2i& start, const Vec2i& goal, const Robots& robotType)
        : m_pos(start)
        , m_start(start)
        , m_maze(maze)
        , m_goal(goal)
        , m_finder(pathfinder)
        , m_robotType(robotType)
        , m_arrived(false)
    {
        UpdatePath();
    }
    virtual ~IRobot() = default;

    virtual void UpdatePath()
    {
        m_path = m_finder->invoke(m_pos, m_goal, Vec2i::Manhattan);
    }
    virtual void reset()
    {
        m_arrived = false;
        m_pos = m_start;
        UpdatePath();
    }

    virtual void move() = 0;

    inline constexpr Robots getRobotType() const { return m_robotType; }

    inline constexpr Vec2i getPos() const { return m_pos; }

    inline constexpr Vec2i getGoal() const { return m_goal; }

    inline constexpr bool isArrived() const { return m_arrived; }
protected:
    Vec2i m_pos;
    Vec2i m_start;
    Vec2i m_goal;
    std::vector<Vec2i> m_path;
    std::shared_ptr<Maze> m_maze;
    std::shared_ptr<Pathfinder> m_finder;
    Robots m_robotType;
    bool m_arrived;
}; // IRobot class

class BoomRobot : public IRobot
{
public:
    BoomRobot(const std::shared_ptr<Pathfinder>& pathfinder,
    const std::shared_ptr<Maze>& maze, const Vec2i& start, const Vec2i& goal,int32_t chance)
        : IRobot(pathfinder,maze,start,goal, Robots::BOOM)
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

    virtual void move() override
    {
        if(m_path.empty())
        {
            m_arrived = true;
            return;
        }
        m_pos = m_path.front();
        m_path.erase(m_path.begin());
        if(boom())
        {
            std::cout << "[LOG]: BOOM! BoomRobot has exploded...\n";
        }
    }
private:
    int32_t m_chance;
}; // BoomRobot class

class SimpleRobot : public IRobot
{
public:
    SimpleRobot(const std::shared_ptr<Pathfinder>& pathfinder,
                const std::shared_ptr<Maze>& maze, const Vec2i& start, const Vec2i& goal)
            : IRobot(pathfinder,maze,start,goal, Robots::SIMPLE)
    {
    }
    virtual ~SimpleRobot() = default;

    virtual void move()
    {
        if(m_path.empty())
        {
            m_arrived = true;
            return;
        }

        m_pos = m_path.front();
        m_path.erase(m_path.begin());

        return;
    }
}; // SimpleRobot class

class SlowRobot : public IRobot
{
public:
    SlowRobot(const std::shared_ptr<Pathfinder>& pathfinder,
              const std::shared_ptr<Maze>& maze, const Vec2i& start, const Vec2i& goal)
        : IRobot(pathfinder,maze,start,goal, Robots::SLOW),
        m_midPoint(RandomGenerator::generateCellCoords(m_maze.get()))
    {
    }
    virtual ~SlowRobot() = default;

    virtual void UpdatePath() override
    {
        m_path = m_finder->invoke(m_pos, m_midPoint, Vec2i::Manhattan);
        std::vector<Vec2i> secondPath = m_finder->invoke(m_midPoint, m_goal, Vec2i::Manhattan);
        m_path.insert(m_path.end(), secondPath.begin(), secondPath.end());
    }

    virtual void move()
    {
        if (m_path.empty())
        {
            m_arrived = true;
            return;
        }

        if (m_pos.x == m_goal.x && m_pos.y == m_goal.y)
        {
            m_arrived = true;
            m_path.clear();
            return;
        }

        m_pos = m_path.front();
        m_path.erase(m_path.begin());

        std::cout << "[LOG]: SlowRobot middle point - " << m_midPoint.x << " " << m_midPoint.y << "." << std::endl;
    }
private:
    Vec2i m_midPoint;

}; // SlowRobot class

class AngryRobot : public IRobot
{
public:
    AngryRobot(const std::shared_ptr<Pathfinder>& pathfinder, const std::shared_ptr<Maze>& maze, const Vec2i& start, const Vec2i& goal)
            : IRobot(pathfinder,maze,start,goal, Robots::ANGRY)
            , m_prevpos(0)
    {
    }
    virtual ~AngryRobot() = default;

    virtual void move()
    {
        if (m_path.empty())
        {
            m_arrived = true;
            return;
        }
        m_prevpos = m_pos;
        m_pos = m_path.front();
        Vec2i delta = m_pos - m_prevpos;
        if (m_maze->breakWall(m_pos, delta))
        {
            std::cout << "[LOG]: GRAAAA! AngryRobot has punched wall..\n";
        }
        
        m_path.erase(m_path.begin());

        return;
    }
private:
    Vec2i m_prevpos;
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

    void Reset()
    {
        // clear robots
        for(IRobot* robot : m_robots)
        {
            robot->reset();
        }
        m_pathfinder->reset();

    }

    inline constexpr std::vector<IRobot*>& GetRobots() { return m_robots; }
    inline constexpr const std::vector<IRobot*>& GetRobots() const { return m_robots; }

private:
    std::vector<IRobot*> m_robots;
    std::shared_ptr<Maze> m_maze;
    std::shared_ptr<Pathfinder> m_pathfinder;
}; // RobotManager class