#include "Application.h"

#include <limits>
#include <memory>

#include "Maze.h"

enum Opcode
{
    START_BATTLE,
    PRINT_MAZE,
    PRINT_PATH
}; // Opcode enum

static void waitForEnter()
{
    std::cout << "Press enter to continue\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

Application::Application(std::shared_ptr<MazeFactory> factory, size_t Height, size_t Width, size_t seed)
    : m_maze(factory->createMaze(Width, Height, seed))
    , m_pathfinder(std::make_shared<Pathfinder>(m_maze))
    , m_battle(m_maze)
{
    std::cout << "Aplication start" << std::endl;
}

Application* Application::GetInstance()
{
    return s_instance;
}

void Application::Init(std::shared_ptr<MazeFactory> factory, size_t Height, size_t Width, size_t seed)
{
    if (!s_instance)
    {
        s_instance = new Application(factory,Height,Width,seed);
    }
}

void Application::Deinit()
{
    delete s_instance;
}

void Application::Run()
{
    bool stopped = false;
    while(!stopped)
    {
        CLEAR_SCREEN();
        std::cout << "Enter the opcode!\n";
        std::cout << "0 - Start battle\n";
        std::cout << "1 - Print maze\n";
        std::cout << "2 - Print path\n";
        std::cout << "Else - Exit\n";

        uint32_t opcode;
        std::cin >> opcode;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (opcode)
        {
        case START_BATTLE:
            m_battle.Run();
            break;
        case PRINT_MAZE:
            MazePrinter::PrintInConsole(m_maze.get());
            waitForEnter();
            break;
        case PRINT_PATH:
        {
            std::cout << "Print coordinates of start and end points!\n";
            std::cout << "Start point: ";
            Vec2i start;
            std::cin >> start.x;
            std::cin >> start.y;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            std::cout << "End point: ";
            Vec2i end;
            std::cin >> end.x;
            std::cin >> end.y;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            std::cout << "Path:\n";
            std::vector<Vec2i> path = m_pathfinder->invoke(start, end, Vec2i::Manhattan);
            MazePrinter::PrintInConsole(m_maze.get(), path);
            waitForEnter();
        }; break;
        default: // Exit
            stopped = true;
            break;
        }
    }
}