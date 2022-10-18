#pragma once
#include <iostream>

#include <stack>
#include <vector>

#include <random>

class Maze
{
public:
	Maze(int maze_width, int maze_height);
	~Maze();

	void ShowMaze();

private:
	struct Cell
	{
		bool path_north = false;
		bool path_east = false;
		bool path_south = false;
		bool path_west = false;
		bool is_visited = false;
	};

	Cell* maze_;
	int maze_width_;
	int maze_height_;

	void CreateMaze();
	int GenerateRandIntInRange(int from, int to);
};