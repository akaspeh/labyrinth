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
			Direction dir = neighbours[rand() % neighbours.size()];

			maze_[delta_pos(0, 0)].is_visited = true;
			++visited_cells_;

			switch (dir)
			{
			case Direction::NORTH:
				maze_[delta_pos(0, 0)].path_north = true;
				maze_[delta_pos(0, -1)].path_south = true;
				maze_stack_.push({ maze_stack_.top().first, maze_stack_.top().second - 1 });
				break;
			case Direction::EAST:
				maze_[delta_pos(0, 0)].path_east = true;
				maze_[delta_pos(1, 0)].path_west = true;
				maze_stack_.push({ maze_stack_.top().first + 1, maze_stack_.top().second });
				break;
			case Direction::SOUTH:
				maze_[delta_pos(0, 0)].path_south = true;
				maze_[delta_pos(0, 1)].path_north = true;
				maze_stack_.push({ maze_stack_.top().first, maze_stack_.top().second + 1 });
				break;
			case Direction::WEST:
				maze_[delta_pos(0, 0)].path_west = true;
				maze_[delta_pos(-1, 0)].path_east = true;
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