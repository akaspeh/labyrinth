#include "Maze.h"

Maze::Maze(int width, int height)
	: maze_width_(width), maze_height_(height), visited_cells_(1)
{
	maze_ = new Cell[maze_width_ * maze_height_];

	maze_stack_.push({ 0,0 });
	maze_[0].is_visited = true;

	CreateMaze();
}

Maze::~Maze()
{
	delete[] maze_;
}

void Maze::ShowMaze()
{
	std::string paths = "";

	std::cout << "\t";
	for (int i = 0; i < maze_width_; i++)
	{
		std::cout << i << "\t";
	}

	std::cout << std::endl;

	for (int i = 0; i < maze_height_; i++)
	{
		std::cout << i << "\t";

		for (int j = 0; j < maze_width_; j++)
		{
			if (maze_[i * maze_width_ + j].path_north == true)
			{
				paths += 'N';
			}
			if (maze_[i * maze_width_ + j].path_east == true)
			{
				paths += 'E';
			}
			if (maze_[i * maze_width_ + j].path_south == true)
			{
				paths += 'S';
			}
			if (maze_[i * maze_width_ + j].path_west == true)
			{
				paths += 'W';
			}

			std::cout << paths << "\t";

			paths = "";
		}

		std::cout << std::endl;
	}
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

	auto delta_pos = [&](int x, int y)
	{
		return (maze_stack_.top().second + y) * maze_width_ + (maze_stack_.top().first + x);
	};

	std::vector<Direction> neighbours;

	while (visited_cells_ < maze_width_ * maze_height_)
	{
		// North neighbour
		if (maze_stack_.top().second > 0 && maze_[delta_pos(0, -1)].is_visited == false)
		{
			neighbours.push_back(Direction::NORTH);
		}

		// East neighbour
		if (maze_stack_.top().first < maze_width_ - 1 && maze_[delta_pos(1, 0)].is_visited == false)
		{
			neighbours.push_back(Direction::EAST);
		}

		// South neighbour
		if (maze_stack_.top().second < maze_height_ - 1 && maze_[delta_pos(0, 1)].is_visited == false)
		{
			neighbours.push_back(Direction::SOUTH);
		}

		// West neighbour
		if (maze_stack_.top().first > 0 && maze_[delta_pos(-1, 0)].is_visited == false)
		{
			neighbours.push_back(Direction::WEST);
		}

		if (!neighbours.empty())
		{
			Direction dir = neighbours[GenerateRandIntInRange(0, neighbours.size() - 1)];

			maze_[delta_pos(0, 0)].is_visited = true;
			++visited_cells_;

			switch (dir)
			{
			case Direction::NORTH:
				maze_[delta_pos(0, 0)].path_north = true;
				maze_[delta_pos(0, -1)].path_south = true;
				maze_[delta_pos(0, -1)].is_visited = true;
				maze_stack_.push({ maze_stack_.top().first, maze_stack_.top().second - 1 });
				break;
			case Direction::EAST:
				maze_[delta_pos(0, 0)].path_east = true;
				maze_[delta_pos(1, 0)].path_west = true;
				maze_[delta_pos(1, 0)].is_visited = true;
				maze_stack_.push({ maze_stack_.top().first + 1, maze_stack_.top().second });
				break;
			case Direction::SOUTH:
				maze_[delta_pos(0, 0)].path_south = true;
				maze_[delta_pos(0, 1)].path_north = true;
				maze_[delta_pos(0, 1)].is_visited = true;
				maze_stack_.push({ maze_stack_.top().first, maze_stack_.top().second + 1 });
				break;
			case Direction::WEST:
				maze_[delta_pos(0, 0)].path_west = true;
				maze_[delta_pos(-1, 0)].path_east = true;
				maze_[delta_pos(-1, 0)].is_visited = true;
				maze_stack_.push({ maze_stack_.top().first - 1, maze_stack_.top().second });
				break;
			default:
				break;
			}

			neighbours.clear();
		}
		else
		{
			maze_stack_.pop();
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