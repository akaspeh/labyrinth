#pragma once

#include <cstdint>
#include <queue>
#include <vector>
#include <functional>
#include <utility>
#include <memory>

#include "Maze.h"

struct Node
{
    Vec2i pos = Vec2i(0);
    Vec2i parent = Vec2i(0); // from which node this one was created
    uint32_t g = 0; // distance from starting node to current
    uint32_t h = 0; // heuristic distance to the goal

    inline bool operator<(const Node& rhs) const { return (g + h) < (rhs.g + rhs.h); }
};

/**
 * @brief Pathfinder object implementation based on A* algorithm 
 * 
 * TODO: 17.11.22 - this class should be fully rewritten
 */
class Pathfinder
{
public:
    using HeuristicFn = std::function<uint32_t(const Vec2i&, const Vec2i&)>;

public:
    Pathfinder(const std::shared_ptr<Maze>& maze);
    ~Pathfinder() = default;

    std::vector<Vec2i> invoke(const Vec2i& start, const Vec2i& goal, const HeuristicFn& heuristic);
private:
    std::vector<Vec2i> recreatePath(const Vec2i& goal) const;
    inline constexpr bool isValid(const Vec2i& v) const { return v.x < m_dimensions.x && v.y < m_dimensions.y; }

    /**
     * @param parent a node's position, from which we will be checking the wall
     * @param neighbor is a move-to node's position, 
     * 
     * undefined behavior if length(neighbor - delta) != 1 
     * 
     * @return true if it is possible to move from parent to neigbor
     * @return false if there is no way to move from parent to neighbor
     */
    bool isWall(const Vec2i& parent, const Vec2i& neighbor) const;
    inline constexpr size_t toIndex1D(const Vec2i& v) const { return static_cast<size_t>((v.y * m_dimensions.x) + v.x); };

private:
    std::vector<Node> m_pathList;
    std::vector<bool> m_closedList;
    Vec2i m_dimensions;
    std::shared_ptr<Maze> m_maze;
};