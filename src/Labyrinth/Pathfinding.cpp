#include "Pathfinding.h"

Pathfinder::Pathfinder(Maze& maze)
    : m_dimensions(maze.Dimensions())
    , m_data(maze.Data())
{
}

std::vector<Vec2> Pathfinder::pathfind(const Vec2& start, const Vec2& goal, const HeuristicFn& heuristic)
{
    static constexpr Vec2 neighbors[] = {
        Vec2{ 0, -1}, // NORTH
        Vec2{ 1,  0}, // EAST
        Vec2{ 0,  1}, // SOUTH
        Vec2{-1,  0}, // WEST
    };

    size_t sz = m_dimensions.x * m_dimensions.y;
    m_pathList.resize(sz);
    m_closedList.resize(sz, false);

    m_pathList[toIndex1D(start)].parent = start; // assign start parent to start so we could recreate the path
    m_openList.push(Node{ .pos = start }); // just assign pos, everything else is zero-initialized
    Vec2 currentPos;

    while (!m_openList.empty())
    {
        currentPos = m_openList.top().pos; // get node with least f value (g + h, to be exact)
        if (currentPos == goal)
        {
            break;
        }

        m_openList.pop();
        m_closedList[toIndex1D(currentPos)] = true;

        for (const Vec2& v : neighbors)
        {
            Vec2 neighborPos = currentPos + v;
            size_t index = toIndex1D(neighborPos);

            if (!isValid(neighborPos) || isWall(currentPos, neighborPos) || m_closedList[index] == true)
                continue;

            uint32_t g = m_pathList[toIndex1D(currentPos)].g + 1;
            uint32_t h = heuristic(neighborPos, goal);
            uint32_t f = g + h;

            uint32_t neighborF = m_pathList[index].g + m_pathList[index].h;
            if (neighborF == 0 || f < neighborF)
            {
                Node n = { neighborPos, currentPos, g, h };
                m_openList.push(n);
                m_pathList[index] = n;
            }
        }
    }

    return recreatePath(goal);
}

std::vector<Vec2> Pathfinder::recreatePath(const Vec2& goal) const
{
    std::vector<Vec2> path;

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

bool Pathfinder::isWall(Vec2& parent, Vec2& neighbor) const
{
    // I get delta between vector neigbor and parent, based on which I will choose the direction
    // Example: Parent = {0, 1} and Neighbor = {0, 2}. Than Delta = {0, 1}, which is Direction::SOUTH
    Vec2 delta = Vec2::Delta(neighbor, parent);
    // now, when we got a direction, we can check if we can move from parent to neighbor
    Cell& cell = m_data[toIndex1D(parent)];
    bool r = true;
    if (delta == Vec2{ 0, -1 })
        r = !cell.hasPath(Direction::NORTH);
    else if (delta == Vec2{ 1, 0 })
        r = !cell.hasPath(Direction::EAST);
    else if (delta == Vec2{ 0, 1 })
        r = !cell.hasPath(Direction::SOUTH);
    else r = !cell.hasPath(Direction::WEST);
    return r;
}