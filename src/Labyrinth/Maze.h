#pragma once
#include <iostream>

#include <stack>
#include <vector>

#include <random>

class Maze
{
public:
	Maze(int mazeWidth, int mazeHeight);
	~Maze();

	void ShowMaze();

private:
	struct Cell
	{
		bool m_bPathNorth = false;
		bool m_bPathEast = false;
		bool m_bPathSouth = false;
		bool m_bPathWest = false;
		bool m_isVisited = false;
	};

	Cell* m_pMaze;
	int m_iMazeWidth;
	int m_iMazeHeight;

	void CreateMaze();
	static int GenerateRandIntInRange(int from, int to);
};