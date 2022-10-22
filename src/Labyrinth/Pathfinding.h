#pragma once

#include <cstdint>
#include <queue>
#include <vector>
#include <functional>
#include <utility>

#include "Maze.h"
#include "Vec2.h"

struct Node
{
    Vec2 pos{};
    Vec2 parent{}; // from which node this one was created
    uint32_t g = 0; // distance from starting node to current
    uint32_t h = 0; // heuristic distance to the goal

    inline bool operator<(const Node& rhs) const { return (g + h) < (rhs.g + rhs.h); }
};

class Pathfinder
{
public:
    using HeuristicFn = std::function<uint32_t(const Vec2&, const Vec2&)>;
private:
    std::priority_queue<Node> m_openList;    
    std::vector<Node> m_pathList;
    std::vector<bool> m_closedList;
    Vec2 m_dimensions;
    Cell* m_data;

public:
    Pathfinder(Maze& maze);
    ~Pathfinder() = default;

    std::vector<Vec2> pathfind(const Vec2& start, const Vec2& goal, const HeuristicFn& heuristic);
private:
    std::vector<Vec2> recreatePath(const Vec2& goal) const;
    inline constexpr bool isValid(const Vec2& v) const 
    { 
        return v.x < m_dimensions.x && v.x >= 0 
            && v.y < m_dimensions.y && v.y >= 0; 
    }

    /**
     * @param parent a node's position, from which we will be checking the wall
     * @param neighbor is a move-to node's position, 
     * @return true if it is possible to move from parent to neigbor
     * @return false if there is no way to move from parent to neighbor
     */
    bool isWall(Vec2& parent, Vec2& neighbor) const;
    inline constexpr size_t toIndex1D(const Vec2& v) const { return (v.y * m_dimensions.x) + v.x; };
};