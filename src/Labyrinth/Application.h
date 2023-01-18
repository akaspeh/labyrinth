#pragma once

#include "Battle.h"
#include "Pathfinding.h"

class Application {
public:
    Application(std::shared_ptr<MazeFactory> factory);
    ~Application() = default;

    void Run();

private:
    std::shared_ptr<Maze> m_maze;
    std::shared_ptr<Pathfinder> m_pathfinder;
    BattleContext m_battle;
};


