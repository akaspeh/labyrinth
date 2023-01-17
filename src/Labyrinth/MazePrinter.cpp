#include "MazePrinter.h"

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
                std::cout << (!(*maze)[x][y].hasPath(Direction::NORTH) ? "##" : "#.");
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
                std::cout << (!(*maze)[x][y].hasPath(Direction::WEST) ? "#." : "..");
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
