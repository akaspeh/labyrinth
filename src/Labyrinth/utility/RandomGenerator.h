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

    static void setSeed(uint32_t seed = 0);

    static size_t generateIndex(size_t from, size_t to, uint32_t seed = 0);
    static Vec2i generateCellCoords(Maze* maze);

private:
    static inline std::random_device m_randDevice;
    static inline std::random_device::result_type m_seed;
    static inline std::mt19937 m_randGenerator;
};