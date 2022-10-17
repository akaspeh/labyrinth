#include "Maze.h"

Maze::Maze(int width, int height)
	: maze_width_(width), maze_height_(height), visited_cells_(1)
{
	maze_ = new Cell[maze_width_ * maze_height_];
	memset(maze_, 0x00, maze_width_ * maze_height_ * sizeof(int));

	maze_stack_.push({ 0,0 });
	maze_[0].is_visited = true;

	CreateMaze();
}

Maze::~Maze()
{
	delete maze_;
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

	auto deltaPos = [&](int x, int y)
	{
		return (maze_stack_.top().second + y) * maze_width_ + (maze_stack_.top().first + x);
	};

	std::vector<Direction> neighbours;

	while (visited_cells_ < maze_width_ * maze_height_)
	{
		// North neighbour
		if (maze_stack_.top().second > 0 && maze_[deltaPos(0, -1)].is_visited == false)
		{
			neighbours.push_back(Direction::NORTH);
		}

		// East neighbour
		if (maze_stack_.top().first < maze_width_ - 1 && maze_[deltaPos(1, 0)].is_visited == false)
		{
			neighbours.push_back(Direction::EAST);
		}

		// South neighbour
		if (maze_stack_.top().second < maze_height_ - 1 && maze_[deltaPos(0, 1)].is_visited == false)
		{
			neighbours.push_back(Direction::SOUTH);
		}

		// West neighbour
		if (maze_stack_.top().first > 0 && maze_[deltaPos(-1, 0)].is_visited == false)
		{
			neighbours.push_back(Direction::WEST);
		}

		if (!neighbours.empty())
		{
			Direction dir = neighbours[rand() % neighbours.size()];

			maze_[deltaPos(0, 0)].is_visited = true;
			++visited_cells_;

			switch (dir)
			{
			case Direction::NORTH:
				maze_[deltaPos(0, 0)].path_north = true;
				maze_[deltaPos(0, -1)].path_south = true;
				maze_stack_.push({ maze_stack_.top().first, maze_stack_.top().second - 1 });
				break;
			case Direction::EAST:
				maze_[deltaPos(0, 0)].path_east = true;
				maze_[deltaPos(1, 0)].path_west = true;
				maze_stack_.push({ maze_stack_.top().first + 1, maze_stack_.top().second });
				break;
			case Direction::SOUTH:
				maze_[deltaPos(0, 0)].path_south = true;
				maze_[deltaPos(0, 1)].path_north = true;
				maze_stack_.push({ maze_stack_.top().first, maze_stack_.top().second + 1 });
				break;
			case Direction::WEST:
				maze_[deltaPos(0, 0)].path_west = true;
				maze_[deltaPos(-1, 0)].path_east = true;
				maze_stack_.push({ maze_stack_.top().first - 1, maze_stack_.top().second });
				break;
			default:
				break;
			}
		}
		else
		{
			maze_stack_.pop();
		}
	}
}