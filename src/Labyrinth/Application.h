#pragma once

#include "Battle.h"
#include "Pathfinding.h"

class Application 
{
public:
    ~Application() = default;
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    static Application* GetInstance();

    static void Init(std::shared_ptr<MazeFactory> factory);
    static void Deinit();

    void Run();

protected:
    Application(std::shared_ptr<MazeFactory> factory);

private:
    std::shared_ptr<Maze> m_maze;
    std::shared_ptr<Pathfinder> m_pathfinder;
    BattleContext m_battle;
private:
    static inline Application* s_instance = nullptr;
};


