#pragma once
#include <iostream>

#include <stack>
#include <vector>

#include <random>

#include "Vec2.h"

struct Cell
{
	bool m_bPathNorth = false;
	bool m_bPathEast = false;
	bool m_bPathSouth = false;
	bool m_bPathWest = false;
	bool m_isVisited = false;
};

enum class Direction
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

class Maze
{
public:
	Maze(int mazeWidth, int mazeHeight);
	~Maze();

	// This should be rewritten using std::stringstream?
	void ShowMaze(const std::vector<Vec2>& path);

	inline Cell* Data() { return m_pMaze; }
	inline Vec2 Dimensions() const { return Vec2{m_iMazeWidth, m_iMazeHeight}; } 

private:
	Cell* m_pMaze;
	int m_iMazeWidth;
	int m_iMazeHeight;

	void CreateMaze();
	static int GenerateRandIntInRange(int from, int to);
};