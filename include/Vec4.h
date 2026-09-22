#pragma once

#include "defines.h"
#include "math_util.h"

struct Vec4 {
    float x, y, z, w;

    FORCE_INLINE constexpr Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

    FORCE_INLINE constexpr Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    FORCE_INLINE constexpr Vec4 operator+(const Vec4& v) const {
        return {x + v.x, y + v.y, z + v.z, w + v.w};
    }

    FORCE_INLINE constexpr Vec4 operator-(const Vec4& v) const {
        return {x - v.x, y - v.y, z - v.z, w - v.w};
    }

    FORCE_INLINE constexpr Vec4 operator*(const f32 s) const {
        return {s * x, s * y, s * z, s * w};
    }

    [[nodiscard]] FORCE_INLINE constexpr bool isZero() const {
        return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
    }
};