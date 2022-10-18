#pragma once
#include <iostream>

#include <stack>
#include <vector>

#include <random>

class Maze
{
private:
	struct Cell
	{
		bool path_north = false;
		bool path_east = false;
		bool path_south = false;
		bool path_west = false;
		bool is_visited = false;
	};
public:
	Maze(int width, int height);
	~Maze();

	void ShowMaze();

private:
	int maze_width_;
	int maze_height_;
	Cell* maze_;

	int visited_cells_;
	std::stack<std::pair<int, int>> maze_stack_;

	void CreateMaze();
	int GenerateRandIntInRange(int from, int to);
};