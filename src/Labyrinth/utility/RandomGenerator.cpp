#include "RandomGenerator.h"

std::random_device RandomGenerator::m_randDevice;
std::random_device::result_type RandomGenerator::m_seed;
std::mt19937 RandomGenerator::m_randGenerator;

void RandomGenerator::setSeed(std::optional<std::random_device::result_type> seed)
{
    if (seed.has_value())
    {
        m_seed = seed.value();
    }
    else
    {
        m_seed = m_randDevice();
    }

    std::mt19937 gen(m_seed);
    m_randGenerator = gen;
}

size_t RandomGenerator::generateIndex(size_t from, size_t to)
{
    std::uniform_int_distribution<size_t> randDist(from, to);
    return randDist(m_randGenerator);
}

Vec2i RandomGenerator::generateCellCoords(const Maze& maze)
{
    return { generateIndex(0, static_cast<int32_t>(maze.getWidth() - 1)),
    generateIndex(0, static_cast<int32_t>(maze.getHeight() - 1)) };
}