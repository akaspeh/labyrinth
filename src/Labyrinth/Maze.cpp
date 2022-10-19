#include "Maze.h"

Maze::Maze(int mazeWidth, int mazeHeight)
	: m_iMazeWidth(mazeWidth), m_iMazeHeight(mazeHeight)
{
	m_pMaze = new Cell[m_iMazeWidth * m_iMazeHeight];
	CreateMaze();
}

Maze::~Maze()
{
	delete[] m_pMaze;
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
	std::stack<std::pair<int, int>> mazeStack;

	mazeStack.push({ 0,0 });
	m_pMaze[0].m_isVisited = true;

	int visitedCells = 1;

	// for calculating the position of neighbours
	auto offset = [&](int x, int y)
	{
		return (mazeStack.top().second + y) * m_iMazeWidth + (mazeStack.top().first + x);
	};

	std::vector<Direction> neighbours;

	while (visitedCells < m_iMazeWidth * m_iMazeHeight)
	{
		// North neighbour
		if (mazeStack.top().second > 0 && m_pMaze[offset(0, -1)].m_isVisited == false)
		{
			neighbours.push_back(Direction::NORTH);
		}

		// East neighbour
		if (mazeStack.top().first < m_iMazeWidth - 1 && m_pMaze[offset(1, 0)].m_isVisited == false)
		{
			neighbours.push_back(Direction::EAST);
		}

		// South neighbour
		if (mazeStack.top().second < m_iMazeHeight - 1 && m_pMaze[offset(0, 1)].m_isVisited == false)
		{
			neighbours.push_back(Direction::SOUTH);
		}

		// West neighbour
		if (mazeStack.top().first > 0 && m_pMaze[offset(-1, 0)].m_isVisited == false)
		{
			neighbours.push_back(Direction::WEST);
		}

		if (!neighbours.empty())
		{
			Direction dir = neighbours[GenerateRandIntInRange(0, neighbours.size() - 1)];

			m_pMaze[offset(0, 0)].m_isVisited = true;
			++visitedCells;

			switch (dir)
			{
			case Direction::NORTH:
				m_pMaze[offset(0, 0)].m_bPathNorth = true;
				m_pMaze[offset(0, -1)].m_bPathSouth = true;
				m_pMaze[offset(0, -1)].m_isVisited = true;
				mazeStack.push({ mazeStack.top().first, mazeStack.top().second - 1 });
				break;
			case Direction::EAST:
				m_pMaze[offset(0, 0)].m_bPathEast = true;
				m_pMaze[offset(1, 0)].m_bPathWest = true;
				m_pMaze[offset(1, 0)].m_isVisited = true;
				mazeStack.push({ mazeStack.top().first + 1, mazeStack.top().second });
				break;
			case Direction::SOUTH:
				m_pMaze[offset(0, 0)].m_bPathSouth = true;
				m_pMaze[offset(0, 1)].m_bPathNorth = true;
				m_pMaze[offset(0, 1)].m_isVisited = true;
				mazeStack.push({ mazeStack.top().first, mazeStack.top().second + 1 });
				break;
			case Direction::WEST:
				m_pMaze[offset(0, 0)].m_bPathWest = true;
				m_pMaze[offset(-1, 0)].m_bPathEast = true;
				m_pMaze[offset(-1, 0)].m_isVisited = true;
				mazeStack.push({ mazeStack.top().first - 1, mazeStack.top().second });
				break;
			default:
				break;
			}

			neighbours.clear();
		}
		else
		{
			mazeStack.pop();
		}
	}
}

void Maze::ShowMaze()
{
	// k = 0 -	###	\
	// k = 1 -	# #	- entire cell spans 3 rows
	// k = 2 -	###	/

	for (int i = 0; i < m_iMazeHeight; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			std::cout << "#";

			for (int j = 0; j < m_iMazeWidth; j++)
			{
				Cell* curr = &m_pMaze[i * m_iMazeWidth + j];

				switch (k)
				{
				case 0:
					if (!curr->m_bPathNorth)
					{
						std::cout << "###";
					}
					else
					{
						std::cout << "# #";
					}
					break;
				case 1:
					if (!curr->m_bPathEast && curr->m_bPathWest)
					{
						std::cout << "  #";
					}
					else if (curr->m_bPathEast && !curr->m_bPathWest)
					{
						std::cout << "#  ";
					}
					else if (!curr->m_bPathEast && !curr->m_bPathWest)
					{
						std::cout << "# #";
					}
					else
					{
						std::cout << "   ";
					}
					break;
				case 2:
					if (!curr->m_bPathSouth)
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