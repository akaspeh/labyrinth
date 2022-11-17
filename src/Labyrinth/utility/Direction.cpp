#include "utility/Direction.h"

Direction getOpposite(Direction dir)
{
    switch (dir)
    {
        case Direction::NORTH: return Direction::SOUTH;
        case Direction::EAST:  return Direction::WEST;
        case Direction::SOUTH: return Direction::NORTH;
        case Direction::WEST:  return Direction::EAST;
        default: return Direction::UNKNOWN;
    }
}