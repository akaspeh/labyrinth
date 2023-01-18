#include "Pathfinding.h"

#include <memory>

Pathfinder::Pathfinder(const std::shared_ptr<Maze>& maze)
    : m_dimensions(Vec2i(maze->getWidth(), maze->getHeight()))
    , m_maze(maze)
{
}

void Pathfinder::reset()
{
    m_pathList.clear();
    m_closedList.clear();
}

std::vector<Vec2i> Pathfinder::invoke(const Vec2i& start, const Vec2i& goal, const HeuristicFn& heuristic)
{
    static Vec2i neighbors[] = {
        Vec2i{ 0, -1}, // NORTH
        Vec2i{ 1,  0}, // EAST
        Vec2i{ 0,  1}, // SOUTH
        Vec2i{-1,  0}, // WEST
    };

    size_t sz = m_dimensions.x * m_dimensions.y;
    reset();
    m_pathList.resize(sz);
    m_closedList.resize(sz, false);
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList; // ...

    m_pathList[toIndex1D(start)].parent = start; // assign start parent to start so we could recreate the path
    openList.push(Node{ .pos = start }); // just assign pos, everything else is zero-initialized
    Vec2i currentPos;

    while (!openList.empty())
    {
        currentPos = openList.top().pos; // get node with least f value (g + h, to be exact)
        if (currentPos == goal)
        {
            break;
        }

        // Mark node as closed one (as we just traversed it)
        openList.pop();
        m_closedList[toIndex1D(currentPos)] = true;

        for (const Vec2i& v : neighbors)
        {
            Vec2i neighborPos = Vec2i(currentPos.x + v.x, currentPos.y + v.y);
            size_t index = toIndex1D(neighborPos);

            if (!isValid(neighborPos) || isWall((Vec2i) currentPos, (Vec2i) neighborPos) || m_closedList[index])
                continue;

            // we count new f, g and h
            uint32_t g = m_pathList[toIndex1D(currentPos)].g + 1;
            uint32_t h = heuristic(neighborPos, goal);
            uint32_t f = g + h;

            // if neighbors f == 0 (means we didn't traverse it yet) or is less than our current f
            // then we assign our f to the node and push it into openList
            uint32_t neighborF = m_pathList[index].g + m_pathList[index].h;
            if (neighborF == 0 || f < neighborF)
            {
                Node n = { neighborPos, currentPos, g, h };
                openList.push(n);
                m_pathList[index] = n;
            }
        }
    }

    return recreatePath(goal);
}

std::vector<Vec2i> Pathfinder::recreatePath(const Vec2i& goal) const
{
    std::vector<Vec2i> path;

    Vec2 current = goal;
    size_t index = toIndex1D(current);

    while (m_pathList[index].parent != current)
    {
        path.push_back(current);
        current = m_pathList[index].parent;
        index = toIndex1D(current);
    }

    std::reverse(path.begin(), path.end());
    return path;
}

bool Pathfinder::isWall(const Vec2i& parent, const Vec2i& neighbor) const
{
    // I get delta between vector neigbor and parent, based on which I will choose the direction
    // Example: Parent = {0, 1} and Neighbor = {0, 2}. Then Delta = {0, 1}, which is Direction::SOUTH
    Vec2i delta = Vec2i::Delta(neighbor, parent);
    // now, when we got a direction, we can check if we can move from parent to neighbor
    Cell& cell = (*m_maze)[parent.x][parent.y];
    return !cell.hasPath((Direction) delta); // we are able to cast vec2 to direction due to vec2 operator()
}