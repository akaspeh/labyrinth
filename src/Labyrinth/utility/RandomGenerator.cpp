#include "RandomGenerator.h"

std::random_device RandomGenerator::m_randDevice;
std::random_device::result_type RandomGenerator::m_seed;
std::mt19937 RandomGenerator::m_randGenerator;

void RandomGenerator::setSeed(std::random_device::result_type seed)
{
    if (seed <= 0)
    {
        m_seed = m_randDevice();
    }
    else
    {
        m_seed = seed;
    }
    std::mt19937 gen(m_seed);
    m_randGenerator = gen;
}

size_t RandomGenerator::generateIndex(size_t from, size_t to)
{
    std::uniform_int_distribution<size_t> randDist(from, to);
    return randDist(m_randGenerator);
}