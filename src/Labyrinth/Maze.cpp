#include "Maze.h"

#include <algorithm>
#include <array>
#include <memory>
#include <sstream>

#include "utility/RandomGenerator.h"
#include "utility/ColorfulText.h"
#include "Robot.h"

Maze::Maze(size_t width, size_t height, uint32_t seed)
    : m_grid(width, height)
{
    RandomGenerator::setSeed(seed);
    UpdateMaze();
}

Maze::Maze(size_t width, size_t height)
    : m_grid(width, height)
    , m_update(true)
{
    RandomGenerator::setSeed();
    UpdateMaze();
}

void Maze::UpdateMaze()
{
    // pair (x, y) - position in the grid
    std::stack<Vec2i> mazeStack;

    mazeStack.push( {0, 0} );
    m_grid[0][0].setVisited();

    // we will store all available directions here
    std::vector<Vec2i> neighbors;
    neighbors.reserve(4); // avoid reallocations

    // we could utilize an array of neighbors to reduce shitty code
    static constexpr std::array<Vec2i, 4> directions = {
        Vec2i( 0, -1),
        Vec2i( 1,  0),
        Vec2i( 0,  1),
        Vec2i(-1,  0),
    };

    while (!mazeStack.empty())
    {
        Vec2i pos = mazeStack.top();

        for (Vec2i delta : directions)
        {
            Vec2i npos = Vec2i(pos.x + delta.x, pos.y + delta.y);
            if (npos.x < 0 || npos.y < 0 || 
                npos.x > m_grid.getWidth() - 1 || npos.y > m_grid.getHeight() - 1 || 
                m_grid[npos.x][npos.y].isVisited())
            {
                continue;
            }

            neighbors.push_back(delta);
        }

        if (!neighbors.empty())
        {
            // get location of random neighbor
            Vec2i delta = neighbors.at(RandomGenerator::generateIndex(0, neighbors.size() - 1));
            // we won't use operator+ overload here as we treat two different types
            // as you may have seen above
            Vec2i npos = Vec2i(pos.x + delta.x, pos.y + delta.y);

            // get neighboring cell
            Cell& ncell = m_grid[npos.x][npos.y];
            // get parent cell
            Cell& pcell = m_grid[pos.x][pos.y];

            // we break a wall in both cells and mark them as visited
            pcell.setVisited();
            ncell.setVisited();
            pcell.breakWall((Direction) delta);
            ncell.breakWall(getOpposite((Direction) delta));
            mazeStack.push(npos);

            // capacity of vector is untouched, so no std::vector::reserve() is needed
            neighbors.clear();
        }
        else mazeStack.pop();
    }
}

bool Maze::breakWall(const Vec2i& pos, const Vec2i& delta)
{
    Cell& cell = m_grid[pos.x][pos.y];
    Vec2i npos = pos + delta;
    if(cell.hasPath((Direction) delta))
    {
        return false;
    }
    if (npos.x < 0 || npos.y < 0 ||
        npos.x > m_grid.getWidth() - 1 || npos.y > m_grid.getHeight() - 1)
    {
        return false;
    }
    Cell& ncell = m_grid[npos.x][npos.y];
    cell.breakWall((Direction) delta);
    ncell.breakWall(getOpposite((Direction) delta));
    m_update = true;
    return true;
}

void MazePrinter::PrintInConsole(Maze* maze, std::optional<cref_type<path_container_type>> path)
{
    bool pathWay = false;

    auto is_path = [&](size_t x, size_t y) -> bool
    {
        if (!path.has_value()) // so that the printing part is cleaner
            return false;

        const path_container_type& container = path.value().get();
        // be careful with size_t -> int32_t conversion
        Vec2i target = Vec2i(x, y); // Vector, that we want to find in path
        return std::any_of(container.begin(), container.end(), [&](const Vec2i& v) { return v == target; });
    };

    for (size_t y = 0; y < maze->getHeight(); y++)
    {
        for (size_t x = 0; x < maze->getWidth(); x++)
        {
            pathWay = is_path(x, y);

            if (pathWay)
            {
                std::stringstream oss;
                if (!(*maze)[x][y].hasPath(Direction::NORTH))
                {
                    std::cout << "##";
                }
                else
                {
                    std::cout << "#";
                    oss << ".";
                    PrintColorful(oss.str(), 6);
                }
            }
            else
            {
                std::cout << (!(*maze)[x][y].hasPath(Direction::NORTH) ? "##" : "# ");
            }

            pathWay = false;
        }
        std::cout << "#" << std::endl;

        for (size_t x = 0; x < maze->getWidth(); x++)
        {
            pathWay = is_path(x, y);

            if (pathWay)
            {
                std::stringstream oss;
                oss << ".";
                if (!(*maze)[x][y].hasPath(Direction::WEST))
                {
                    std::cout << "#";
                    PrintColorful(oss.str(), 6);
                }
                else
                {
                    std::cout << " ";
                    PrintColorful(oss.str(), 6);
                }
            }
            else
            {
                std::cout << (!(*maze)[x][y].hasPath(Direction::WEST) ? "# " : "  ");
            }

            pathWay = false;
        }
        std::cout << "#" << std::endl;
    }

    for (size_t x = 0; x < maze->getWidth(); x++)
    {
        std::cout << "##";
    }

    std::cout << "#" << std::endl;
}

void MazePrinter::PrintInConsoleBold(Maze* maze, std::optional<cref_type<path_container_type>> path)
{
    // k = 0 -	###	\
    // k = 1 -	# #	- entire cell spans 3 rows
    // k = 2 -	###	/

    bool pathWay = false;

    auto is_path = [&](size_t x, size_t y) -> bool
    {
        if (!path.has_value()) // so that the printing part is cleaner
            return false;

        const path_container_type& container = path.value().get();
        // be careful with size_t -> int32_t conversion
        Vec2i target = Vec2i(x, y); // Vector, that we want to find in path
        return std::any_of(container.begin(), container.end(), [&](const Vec2i& v) { return v == target; });
    };

    for (size_t y = 0; y < maze->getHeight(); y++)
    {
        for (size_t k = 0; k < 3; k++)
        {
            std::cout << "#";

            for (size_t x = 0; x < maze->getWidth(); x++)
            {
                Cell& curr = (*maze)[x][y];

                pathWay = is_path(x, y);

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

            std::cout << "#" << std::endl;
        }
    }
}

void MazePrinter::PrintInConsoleRobots(Maze* maze, std::vector<IRobot*>& robots, Vec2i& goal)
{
    std::array<char, static_cast<size_t>(Robots::UNKNOWN)> robotSymbols = {
        'A',
        'B',
        's',
        'S'
    };
    char robotChar = 'u';
    size_t robotsOnOneCell = 0;
    size_t color = 0;

    auto isRobotPos = [&](size_t x, size_t y) -> void
    {
        for (size_t i = 0; i < robots.size(); ++i)
        {
            if (robots[i]->getPos().x == x && robots[i]->getPos().y == y)
            {
                ++robotsOnOneCell;
                robotChar = robotSymbols[static_cast<size_t>(robots[i]->getRobotType())];
                color = static_cast<size_t>(robots[i]->getRobotType());
            }
        }
    };

    for (size_t y = 0; y < maze->getHeight(); y++)
    {
        for (size_t x = 0; x < maze->getWidth(); x++)
        {
            std::cout << (!(*maze)[x][y].hasPath(Direction::NORTH) ? "##" : "# ");
        }
        std::cout << "#" << std::endl;

        for (size_t x = 0; x < maze->getWidth(); x++)
        {
            isRobotPos(x, y);
            if (robotsOnOneCell > 0)
            {
                std::stringstream oss1, oss2;
                if (robotsOnOneCell > 1)
                {
                    oss1 << robotsOnOneCell;
                    oss2 << " " << robotsOnOneCell;
                    color = 5;
                }
                else
                {
                    oss1 << robotChar;
                    oss2 << " " << robotChar;
                }

                if (!(*maze)[x][y].hasPath(Direction::WEST))
                {
                    std::cout << "#";
                    PrintColorful(oss1.str(), color);
                }
                else
                {
                    PrintColorful(oss2.str(), color);
                }
            }
            else if (goal.x == x && goal.y == y)
            {
                std::stringstream oss;
                oss << "0";

                if (!(*maze)[x][y].hasPath(Direction::WEST))
                {
                    std::cout << "#";
                    PrintColorful(oss.str(), 5);
                }
                else
                {
                    std::cout << " ";
                    PrintColorful(oss.str(), 5);
                }
            }
            else
            {
                std::cout << (!(*maze)[x][y].hasPath(Direction::WEST) ? "# " : "  ");
            }
            robotsOnOneCell = 0;
        }
        std::cout << "#" << std::endl;
    }

    for (size_t x = 0; x < maze->getWidth(); x++)
    {
        std::cout << "##";
    }

    std::cout << "#" << std::endl;
}

std::shared_ptr<Maze> SimpleMazeCreator::createMaze(size_t width, size_t height, uint32_t seed) const
{
    if (seed > 0)
        return std::shared_ptr<Maze>(new Maze(width, height, seed));

    return std::shared_ptr<Maze>(new Maze(width, height));
}
