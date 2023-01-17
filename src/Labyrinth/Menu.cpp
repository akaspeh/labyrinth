#include "Menu.h"

#include "utility/RandomGenerator.h"

Menu::~Menu()
{
    delete m_maze;
}

void Menu::CreateMaze()
{
    size_t width;
    size_t height;
    std::cout << "Enter width\n"; std::cin >> width;
    std::cout << "Enter height\n"; std::cin >> height;

    if (m_maze)
        delete m_maze;

    m_maze = new Maze(width, height);
}

void Menu::ShowMaze() const
{
    MazePrinter::PrintInConsoleBold(m_maze);
    std::cout << std::endl << std::endl;
    MazePrinter::PrintInConsole(m_maze);
}

void Menu::PathFind()
{
    Pathfinder finder(*m_maze);
    Vec2i goal = Vec2i(m_maze->getWidth() - 1, m_maze->getHeight() - 1);
    std::vector<Vec2i> path = finder.pathfind(Vec2i(0), goal, Vec2i::Manhattan);

    std::cout << std::endl << std::endl;

    MazePrinter::PrintInConsoleBold(m_maze, path);
}