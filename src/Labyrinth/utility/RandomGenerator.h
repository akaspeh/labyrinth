#pragma once
#include <random>

#include "Maze.h"

class RandomGenerator
{
public:
    RandomGenerator() = delete;
    ~RandomGenerator() = delete;
    RandomGenerator(const RandomGenerator& randomGenerator) = delete;
    RandomGenerator operator=(const RandomGenerator& randomGenerator) = delete;

    static void setSeed(std::optional<std::random_device::result_type> seed = std::nullopt);

    static size_t generateIndex(size_t from, size_t to);
    static Vec2i generateCellCoords(Maze* maze);

private:
    static std::random_device m_randDevice;
    static std::random_device::result_type m_seed;
    static std::mt19937 m_randGenerator;
};