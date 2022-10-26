#include "Maze.h"

#include <algorithm>

Maze::Maze(const size_t mazeWidth, const size_t mazeHeight, const int seed)
    : m_mazeWidth(mazeWidth), m_mazeHeight(mazeHeight),
    m_pMaze(new Cell[mazeWidth * mazeHeight]),
    m_seed((seed >= 0) ? static_cast<std::random_device::result_type>(seed) : m_randDevice()),
    m_randGenerator(m_seed)

{
    CreateMaze();
}

void Maze::CreateMaze()
{
    // pair (x, y) - position in the grid
    std::stack<Vec2i> mazeStack;

    mazeStack.push(Vec2i(0));
    m_pMaze[0].setVisited();

    size_t visitedCells = 1;

    // for calculating the position of neighbours
    auto offset = [&](const size_t x, const size_t y)
    {
        return (mazeStack.top().y + y) * m_mazeWidth + (mazeStack.top().x + x);
    };

    std::vector<Direction> neighbours;

    while (visitedCells < m_mazeWidth * m_mazeHeight)
    {
        // North neighbour
        if (mazeStack.top().y > 0 && !m_pMaze[offset(0, -1)].isVisited())
        {
            neighbours.push_back(Direction::NORTH);
        }

        // East neighbour
        if (mazeStack.top().x < m_mazeWidth - 1 && !m_pMaze[offset(1, 0)].isVisited())
        {
            neighbours.push_back(Direction::EAST);
        }

        // South neighbour
        if (mazeStack.top().y < m_mazeHeight - 1 && !m_pMaze[offset(0, 1)].isVisited())
        {
            neighbours.push_back(Direction::SOUTH);
        }

        // West neighbour
        if (mazeStack.top().x > 0 && !m_pMaze[offset(-1, 0)].isVisited())
        {
            neighbours.push_back(Direction::WEST);
        }

        if (!neighbours.empty())
        {
            Direction& dir = neighbours[static_cast<size_t>(GenerateIntInRange(0, static_cast<int>(neighbours.size()) - 1))];

            m_pMaze[offset(0, 0)].setVisited();
            ++visitedCells;

            switch (dir)
            {
            case Direction::NORTH:
                m_pMaze[offset(0, 0)].breakWall(Direction::NORTH);
                m_pMaze[offset(0, -1)].breakWall(Direction::SOUTH);
                m_pMaze[offset(0, -1)].setVisited();
                mazeStack.push({ mazeStack.top().x, mazeStack.top().y - 1 });
                break;
            case Direction::EAST:
                m_pMaze[offset(0, 0)].breakWall(Direction::EAST);
                m_pMaze[offset(1, 0)].breakWall(Direction::WEST);
                m_pMaze[offset(1, 0)].setVisited();
                mazeStack.push({ mazeStack.top().x + 1, mazeStack.top().y });
                break;
            case Direction::SOUTH:
                m_pMaze[offset(0, 0)].breakWall(Direction::SOUTH);
                m_pMaze[offset(0, 1)].breakWall(Direction::NORTH);
                m_pMaze[offset(0, 1)].setVisited();
                mazeStack.push({ mazeStack.top().x, mazeStack.top().y + 1 });
                break;
            case Direction::WEST:
                m_pMaze[offset(0, 0)].breakWall(Direction::WEST);
                m_pMaze[offset(-1, 0)].breakWall(Direction::EAST);
                m_pMaze[offset(-1, 0)].setVisited();
                mazeStack.push({ mazeStack.top().x - 1, mazeStack.top().y });
                break;
            default:
                break;
            }

            neighbours.clear();
        }
        else
        {
            mazeStack.pop();
        }
    }
}

void Maze::ShowMazeText(std::optional<Maze::cref_type<Maze::path_container_type>> path)
{
    bool pathWay = false;

    auto is_path = [&](const size_t row, const size_t col) -> bool
    {
        if (!path.has_value()) // so that the printing part is cleaner
            return false;

        const Maze::path_container_type& container = path.value().get();
        Vec2 target = Vec2(static_cast<int32_t>(col), static_cast<int32_t>(row)); // Vector, that we want to find in path
        return std::any_of(container.begin(), container.end(), [&](const Vec2i& v) { return v == target; });
    };

    for (size_t row = 0; row < m_mazeHeight; ++row)
    {
        for (size_t col = 0; col < m_mazeWidth; ++col)
        {
            pathWay = is_path(row, col);

            if (pathWay)
            {
                std::cout << (!m_pMaze[row * m_mazeWidth + col].hasPath(Direction::NORTH) ? "##" : "#.");
            }
            else
            {
                std::cout << (!m_pMaze[row * m_mazeWidth + col].hasPath(Direction::NORTH) ? "##" : "# ");
            }

            pathWay = false;
        }

        std::cout << "#" << std::endl;

        for (size_t col = 0; col < m_mazeWidth; ++col)
        {
            pathWay = is_path(row, col);

            if (pathWay)
            {
                std::cout << (!m_pMaze[row * m_mazeWidth + col].hasPath(Direction::WEST) ? "#." : "..");
            }
            else
            {
                std::cout << (!m_pMaze[row * m_mazeWidth + col].hasPath(Direction::WEST) ? "# " : "  ");
            }

            pathWay = false;
        }

        std::cout << "#" << std::endl;
    }

    for (size_t col = 0; col < m_mazeWidth; ++col)
    {
        std::cout << "##";
    }

    std::cout << "#" << std::endl;
}

void Maze::ShowMazeTextBold(std::optional<Maze::cref_type<Maze::path_container_type>> path)
{
    // k = 0 -	###	\
	// k = 1 -	# #	- entire cell spans 3 rows
    // k = 2 -	###	/

    bool pathWay = false;

    auto is_path = [&](const size_t row, const size_t col) -> bool
    {
        if (!path.has_value()) // so that the printing part is cleaner
            return false;

        const Maze::path_container_type& container = path.value().get();
        Vec2 target = Vec2(static_cast<int32_t>(col), static_cast<int32_t>(row)); // Vector, that we want to find in path
        return std::any_of(container.begin(), container.end(), [&](const Vec2i& v) { return v == target; });
    };

    for (size_t row = 0; row < m_mazeHeight; ++row)
    {
        for (size_t k = 0; k < 3; k++)
        {
            std::cout << "#";

            for (size_t col = 0; col < m_mazeWidth; ++col)
            {
                Cell& curr = m_pMaze[row * m_mazeWidth + col];

                pathWay = is_path(row, col);

                switch (k)
                {
                case 0:
                    if (!curr.hasPath(Direction::NORTH))
                    {
                        std::cout << "###";
                    }
                    else
                    {
                        std::cout << (pathWay ? "#.#" : "# #");
                    }
                    break;
                case 1:
                    if (!curr.hasPath(Direction::EAST) && curr.hasPath(Direction::WEST))
                    {
                        std::cout << (pathWay ? "..#" : "  #");
                    }
                    else if (curr.hasPath(Direction::EAST) && !curr.hasPath(Direction::WEST))
                    {
                        std::cout << (pathWay ? "#.." : "#  ");
                    }
                    else if (!curr.hasPath(Direction::EAST) && !curr.hasPath(Direction::WEST))
                    {
                        std::cout << (pathWay ? "#.#" : "# #");
                    }
                    else
                    {
                        std::cout << (pathWay ? "..." : "   ");
                    }
                    break;
                case 2:
                    if (!curr.hasPath(Direction::SOUTH))
                    {
                        std::cout << "###";
                    }
                    else
                    {
                        std::cout << (pathWay ? "#.#" : "# #");
                    }
                    break;
                default:
                    break;
                }

                pathWay = false;
            }

            std::cout << "#";

            std::cout << std::endl;
        }
    }
}

int Maze::GenerateIntInRange(int from, int to)
{
    std::uniform_int_distribution<int> randDist(from, to);
    return randDist(m_randGenerator);
}