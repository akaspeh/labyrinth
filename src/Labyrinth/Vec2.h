#pragma once

#include <cstdint>

struct Vec2
{
public:
    Vec2() = default;
    ~Vec2() = default;

    int32_t x{};
    int32_t y{};

    inline Vec2 operator+(const Vec2& rhs) const { return {x + rhs.x, y + rhs.y}; }
    inline Vec2 operator-(const Vec2& rhs) const { return (*this) + (-rhs); }
    inline Vec2 operator-() const { return {-x, -y}; }
    inline bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
    inline bool operator!=(const Vec2& other) const { return !(*this == other); }
    friend inline std::ostream& operator<<(std::ostream& s, const Vec2& v) { s << v.x << ", " << v.y; return s;}

    inline static Vec2 Delta(const Vec2& lhs, const Vec2& rhs) { return {lhs.x - rhs.x, lhs.y - rhs.y}; }

    // https://en.wikipedia.org/wiki/Taxicab_geometry
    inline static constexpr uint32_t Manhattan(const Vec2& lhs, const Vec2& rhs)
    {
        Vec2 delta{ abs(lhs.x - rhs.x), abs(lhs.y - rhs.y)};
        return delta.x + delta.y;
    }
};