#pragma once

enum class Direction
{
    NORTH = 0b1000,
    EAST = 0b0100,
    SOUTH = 0b0010,
    WEST = 0b0001,
    UNKNOWN = 0b0000,
};

Direction getOpposite(Direction dir);