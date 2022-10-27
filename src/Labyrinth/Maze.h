#pragma once
#include <iostream>

#include <stack>
#include <vector>

#include <random>

#include <optional>
#include <utility>

#include "Vec2.h"


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
    // We use those aliases, as we probably will rewrite the whole maze system in future
    using Vec2i = Vec2<int32_t>;
    using path_container_type = std::vector<Vec2i>; // We should use our own data structure in future, i guess
    template<typename T> using ref_type = std::reference_wrapper<T>;
    template<typename T> using cref_type = ref_type<std::add_const_t<T>>;

public:
    Maze(const size_t mazeWidth = 5, const size_t mazeHeight = 5, const int seed = -1);
    ~Maze() { delete[] m_pMaze; }

    void CreateMaze();
    void ShowMazeText(std::optional<cref_type<path_container_type>> path = std::nullopt);
    void ShowMazeTextBold(std::optional<cref_type<path_container_type>> path = std::nullopt);
    void ChangeSize(const size_t mazeWidth, const size_t Height);

    inline Cell* Data() { return m_pMaze; }
    inline Vec2i Dimensions() const { return Vec2(static_cast<int32_t>(m_mazeWidth), static_cast<int32_t>(m_mazeHeight)); }

private:
    size_t m_mazeWidth;
    size_t m_mazeHeight;
    Cell* m_pMaze;

    std::random_device m_randDevice;
    std::random_device::result_type m_seed;
    std::mt19937 m_randGenerator;

    int GenerateIntInRange(int from, int to);
};