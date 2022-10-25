#pragma once
#include <iostream>

#include <stack>
#include <vector>

#include <random>

#include <optional>

#include "Vec2.h"

enum class Direction
{
    NORTH = 0b1000,
    EAST = 0b0100,
    SOUTH = 0b0010,
    WEST = 0b0001
};

class Cell
{
public:
    Cell() : m_cellNode(0) {}
    ~Cell() = default;

    inline constexpr void breakWall(const Direction& dir) { m_cellNode |= static_cast<uint8_t>(dir); }
    inline constexpr bool hasPath(const Direction& dir) { return (m_cellNode & static_cast<uint8_t>(dir)) == static_cast<uint8_t>(dir); }

    inline void setVisited() { m_cellNode |= 0b10000; }
    inline constexpr bool isVisited() const { return (m_cellNode & 0b10000) == 0b10000; }

    inline constexpr uint8_t getValue() const { return m_cellNode & 0b1111; }

private:
    uint8_t m_cellNode;
};

class Maze
{
public:
    Maze(const size_t mazeWidth, const size_t mazeHeight, const int seed = -1);
    ~Maze() { delete[] m_pMaze; }

    void ShowMazeText(std::optional<const std::vector<Vec2>*> path = std::nullopt);
    void ShowMazeTextBold(std::optional<const std::vector<Vec2>*> path = std::nullopt);

    inline Cell* Data() { return m_pMaze; }
    inline Vec2 Dimensions() const { return Vec2{ static_cast<int>(m_mazeWidth), static_cast<int>(m_mazeHeight) }; }

private:

private:
    size_t m_mazeWidth;
    size_t m_mazeHeight;
    Cell* m_pMaze;

    std::random_device m_randDevice;
    std::random_device::result_type m_seed;
    std::mt19937 m_randGenerator;

    void CreateMaze();
    int GenerateIntInRange(int from, int to);
};