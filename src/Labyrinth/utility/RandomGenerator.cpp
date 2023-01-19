#include "RandomGenerator.h"

void RandomGenerator::setSeed(uint32_t seed)
{
    if (seed > 0)
    {
        m_seed = seed;
    }
    else
    {
        m_seed = m_randDevice();
    }

    std::mt19937 gen(m_seed);
    m_randGenerator = gen;
}

size_t RandomGenerator::generateIndex(size_t from, size_t to, uint32_t seed)
{
    std::uniform_int_distribution<size_t> randDist(from, to);
    return randDist(m_randGenerator);
}

Vec2i RandomGenerator::generateCellCoords(Maze* maze)
{
    return { static_cast<int32_t>(generateIndex(0, maze->getWidth() - 1)),
    static_cast<int32_t>(generateIndex(0, maze->getHeight() - 1))};
}