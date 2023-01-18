#pragma once

#include "Battle.h"
#include "Pathfinding.h"

class Application 
{
public:
    ~Application() = default;
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    inline constexpr BattleContext& GetBattleContext() { return m_battle; }
    inline constexpr const BattleContext& GetBattleContext() const { return m_battle; }

    inline std::shared_ptr<Maze> GetMaze() { return m_maze; }
    inline const std::shared_ptr<Maze> GetMaze() const { return m_maze; }

    static Application* GetInstance();

    static void Init(std::shared_ptr<MazeFactory> factory,size_t Height, size_t Width, size_t seed);
    static void Deinit();

    void Run();


protected:
    Application(std::shared_ptr<MazeFactory> factory, size_t Height, size_t Width, size_t seed);

private:
    std::shared_ptr<Maze> m_maze;
    std::shared_ptr<Pathfinder> m_pathfinder;
    BattleContext m_battle;
private:
    static inline Application* s_instance = nullptr;
};


