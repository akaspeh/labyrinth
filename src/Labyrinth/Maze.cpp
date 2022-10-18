#include "Maze.h"

Maze::Maze(int maze_width, int maze_height)
	: maze_width_(maze_width), maze_height_(maze_height)
{
	maze_ = new Cell[maze_width_ * maze_height_];
	CreateMaze();
}

Maze::~Maze()
{
	delete[] maze_;
}

void Maze::CreateMaze()
{
	enum class Direction
	{
		NORTH,
		EAST,
		SOUTH,
		WEST
	};

	// pair (x, y) - position in the grid
	std::stack<std::pair<int, int>> maze_stack;

	maze_stack.push({ 0,0 });
	maze_[0].is_visited = true;

	int visited_cells = 1;

	// for calculating the position of neighbours
	auto delta_pos = [&](int x, int y)
	{
		return (maze_stack.top().second + y) * maze_width_ + (maze_stack.top().first + x);
	};

	std::vector<Direction> neighbours;

	while (visited_cells < maze_width_ * maze_height_)
	{
		// North neighbour
		if (maze_stack.top().second > 0 && maze_[delta_pos(0, -1)].is_visited == false)
		{
			neighbours.push_back(Direction::NORTH);
		}

		// East neighbour
		if (maze_stack.top().first < maze_width_ - 1 && maze_[delta_pos(1, 0)].is_visited == false)
		{
			neighbours.push_back(Direction::EAST);
		}

		// South neighbour
		if (maze_stack.top().second < maze_height_ - 1 && maze_[delta_pos(0, 1)].is_visited == false)
		{
			neighbours.push_back(Direction::SOUTH);
		}

		// West neighbour
		if (maze_stack.top().first > 0 && maze_[delta_pos(-1, 0)].is_visited == false)
		{
			neighbours.push_back(Direction::WEST);
		}

		if (!neighbours.empty())
		{
			Direction dir = neighbours[GenerateRandIntInRange(0, neighbours.size() - 1)];

			maze_[delta_pos(0, 0)].is_visited = true;
			++visited_cells;

			switch (dir)
			{
			case Direction::NORTH:
				maze_[delta_pos(0, 0)].path_north = true;
				maze_[delta_pos(0, -1)].path_south = true;
				maze_[delta_pos(0, -1)].is_visited = true;
				maze_stack.push({ maze_stack.top().first, maze_stack.top().second - 1 });
				break;
			case Direction::EAST:
				maze_[delta_pos(0, 0)].path_east = true;
				maze_[delta_pos(1, 0)].path_west = true;
				maze_[delta_pos(1, 0)].is_visited = true;
				maze_stack.push({ maze_stack.top().first + 1, maze_stack.top().second });
				break;
			case Direction::SOUTH:
				maze_[delta_pos(0, 0)].path_south = true;
				maze_[delta_pos(0, 1)].path_north = true;
				maze_[delta_pos(0, 1)].is_visited = true;
				maze_stack.push({ maze_stack.top().first, maze_stack.top().second + 1 });
				break;
			case Direction::WEST:
				maze_[delta_pos(0, 0)].path_west = true;
				maze_[delta_pos(-1, 0)].path_east = true;
				maze_[delta_pos(-1, 0)].is_visited = true;
				maze_stack.push({ maze_stack.top().first - 1, maze_stack.top().second });
				break;
			default:
				break;
			}

			neighbours.clear();
		}
		else
		{
			maze_stack.pop();
		}
	}
}

void Maze::ShowMaze()
{
	// k = 0 -	###	\
	// k = 1 -	# #	- entire cell spans 3 rows
	// k = 2 -	###	/

	for (int i = 0; i < maze_height_; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			std::cout << "#";

			for (int j = 0; j < maze_width_; j++)
			{
				Cell* curr = &maze_[i * maze_width_ + j];

				switch (k)
				{
				case 0:
					if (!curr->path_north)
					{
						std::cout << "###";
					}
					else
					{
						std::cout << "# #";
					}
					break;
				case 1:
					if (!curr->path_east && curr->path_west)
					{
						std::cout << "  #";
					}
					else if (curr->path_east && !curr->path_west)
					{
						std::cout << "#  ";
					}
					else if (!curr->path_east && !curr->path_west)
					{
						std::cout << "# #";
					}
					else
					{
						std::cout << "   ";
					}
					break;
				case 2:
					if (!curr->path_south)
					{
						std::cout << "###";
					}
					else
					{
						std::cout << "# #";
					}
					break;
				default:
					break;
				}
			}

			std::cout << "#";

			std::cout << std::endl;
		}
	}
}

int Maze::GenerateRandIntInRange(int from, int to)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(from, to);

	return dist6(rng);
}