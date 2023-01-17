#pragma once

#include <cstdint>
#include "utility/Direction.h"

template<typename T>
struct Vec2
{
public:
    Vec2() = default;
    constexpr Vec2(T x, T y) : x(x), y(y) {}
    constexpr Vec2(T xy) : x(xy), y(xy) {}
    ~Vec2() = default;

    union { T x, r; };
    union { T y, g; };

    Vec2<T> operator+(const Vec2<T>& rhs) const { return {x + rhs.x, y + rhs.y}; }
    Vec2<T> operator-(const Vec2<T>& rhs) const { return (*this) + (-rhs); }
    Vec2<T> operator-() const { return {-x, -y}; }
    bool operator==(const Vec2<T>& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Vec2<T>& other) const { return !(*this == other); }
    
    explicit operator Direction() const
    {
        // We dont want to be able to cast any other type except of Vec2i
        if constexpr(!std::is_same<T, int32_t>::value) 
            return Direction::UNKNOWN;

        if (*this == Vec2<int32_t>(0, -1))
            return Direction::NORTH;
        else if (*this == Vec2<int32_t>(0, 1))
            return Direction::SOUTH;
        else if (*this == Vec2<int32_t>(1, 0))
            return Direction::EAST;
        else return Direction::WEST;
    }

    template<typename U>
    explicit operator Vec2<U>() const { return Vec2<U>(static_cast<U>(this->x), static_cast<U>(this->y)); }

    static Vec2 Delta(const Vec2& lhs, const Vec2<T>& rhs) { return {lhs.x - rhs.x, lhs.y - rhs.y}; }

    // https://en.wikipedia.org/wiki/Taxicab_geometry
    static uint32_t Manhattan(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        Vec2<T> delta{ abs(lhs.x - rhs.x), abs(lhs.y - rhs.y)};
        return delta.x + delta.y;
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& s, const Vec2<T>& v) { s << v.x << ", " << v.y; return s;}

using Vec2i = Vec2<int32_t>;

