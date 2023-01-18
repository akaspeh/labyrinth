#pragma once

#include <iostream>
#include <stack>
#include <vector>
#include <optional>
#include <utility>
#include <memory>

#include "utility/Vec2.h"
#include "utility/Direction.h"

/**
 * @brief This class represents a single "node" of a maze 
 * 
 * TODO: 17.11.22 We can implement operator<< for std::iostream&, thus making Maze::Print() look much cleaner!
 */
class Cell
{
public:
    Cell() = default;
    ~Cell() = default;

    inline constexpr void breakWall(const Direction& dir) { m_cellNode |= static_cast<uint8_t>(dir); }
    inline constexpr bool hasPath(const Direction& dir) const { return (m_cellNode & static_cast<uint8_t>(dir)) == static_cast<uint8_t>(dir); }

    inline void setVisited() { m_cellNode |= 0b10000; }
    inline constexpr bool isVisited() const { return (m_cellNode & 0b10000) == 0b10000; }

    inline constexpr uint8_t getValue() const { return m_cellNode & 0b1111; }

    friend std::iostream& operator<<(std::iostream& stream, const Cell& cell) { /*NOIMPL!*/ return stream; };

private:
    uint8_t m_cellNode = 0b00000;
};

class Row
{
public:
    using container_type = std::vector<Cell>;
    using iterator_type = container_type::iterator;
    using const_iterator_type = container_type::const_iterator;

public:
    Row() = default;
    Row(size_t height)
        : m_cells(height, Cell())
    {   
    }
    ~Row() = default;

    inline constexpr Cell& operator[](size_t idx) { return m_cells.at(idx); }
    inline constexpr const Cell& operator[](size_t idx) const { return m_cells.at(idx); }

    inline constexpr size_t getHeight() const { return m_cells.size(); }
    inline constexpr void setHeight(size_t height) { m_cells.resize(height); }

    // Iterator & for-each support
    inline constexpr iterator_type begin() noexcept { return m_cells.begin(); }
    inline constexpr const_iterator_type cbegin() const noexcept { return m_cells.cbegin(); }
    inline constexpr iterator_type end() noexcept { return m_cells.end(); }
    inline constexpr const_iterator_type cend() const noexcept { return m_cells.cend(); }

    inline constexpr Cell& front() { return m_cells.front(); }
    inline constexpr Cell& back() { return m_cells.back(); }

private:
    std::vector<Cell> m_cells;
};

/**
 * @brief Basically, thid Grid class is a wrapper for 2D std::vector with easy-to-use interface 
 *
 */
class Grid
{
public:
    using container_type = std::vector<Row>;
    using iterator_type = container_type::iterator;
    using const_iterator_type = container_type::const_iterator;

public:
    Grid(size_t width, size_t height)
        : m_height(height)
        , m_rows(width, Row(height))
    {
    }
    ~Grid() = default;

    inline constexpr Row& operator[](size_t idx) { return m_rows.at(idx); }
    inline constexpr const Row& operator[](size_t idx) const { return m_rows.at(idx); }

    inline constexpr size_t getWidth() const { return m_rows.size(); }
    inline void setWidth(size_t width) { m_rows.resize(width, Row(m_height)); }

    // we assert, that m_rows[i] and m_rows[j] will always have the same height 
    inline constexpr size_t getHeight() const { return m_rows[0].getHeight(); }
    void setHeight(size_t height) { for (Row& row : m_rows) row.setHeight(height); }

    // Iterator & for-each support
    inline constexpr iterator_type begin() noexcept { return m_rows.begin(); }
    inline constexpr const_iterator_type cbegin() const noexcept { return m_rows.cbegin(); }
    inline constexpr iterator_type end() noexcept { return m_rows.end(); }
    inline constexpr const_iterator_type cend() const noexcept { return m_rows.cend(); }

    inline constexpr Row& front() { return m_rows.front(); }
    inline constexpr Row& back() { return m_rows.back(); }

    // by standard, capacity of the vector is left untouched - what we need
    void clear() { m_rows.clear(); }

private:
    size_t m_height;
    std::vector<Row> m_rows;
};

class Maze
{
public:
    Maze(size_t width, size_t height, uint32_t seed);
    Maze(size_t width, size_t height);
    ~Maze() = default;

    inline constexpr Row& operator[](size_t idx) { return m_grid.operator[](idx); }
    inline constexpr const Row& operator[](size_t idx) const { return m_grid.operator[](idx); }
    
    inline constexpr size_t getWidth() const { return m_grid.getWidth(); }  
    inline constexpr size_t getHeight() const { return m_grid.getHeight(); }
    void breakWall(const Vec2i& pos, const Vec2i& delta);
    inline constexpr bool getUpdateState() const { return m_update; }
    inline void handleUpdate(){ m_update = false; }

    void UpdateMaze();
private:
    Grid m_grid;
    bool m_update;
};

class IRobot;

class MazePrinter
{
public:
    using path_container_type = std::vector<Vec2i>;
    template<typename T> using ref_type = std::reference_wrapper<T>;
    template<typename T> using cref_type = ref_type<std::add_const_t<T>>;
public:
    MazePrinter() = delete;
    ~MazePrinter() = delete;
    MazePrinter(const MazePrinter& mazePrinter) = delete;
    MazePrinter operator=(const MazePrinter& mazePrinter) = delete;

    static void PrintInConsole(Maze* maze,
        std::optional<cref_type<path_container_type>> path = std::nullopt);
    static void PrintInConsoleBold(Maze* maze,
        std::optional<cref_type<path_container_type>> path = std::nullopt);
    static void PrintInConsoleRobots(Maze* maze, std::vector<IRobot*>& robots, Vec2i& goal);
};

class MazeFactory
{
public:
    MazeFactory() = default;
    virtual ~MazeFactory() = default;

    virtual std::shared_ptr<Maze> createMaze(size_t width, size_t height, 
        std::optional<uint32_t> seed = std::nullopt) const = 0;
};

class SimpleMazeCreator : public MazeFactory
{
    virtual std::shared_ptr<Maze> createMaze(size_t width, size_t height,
        std::optional<uint32_t> seed = std::nullopt) const override;
};