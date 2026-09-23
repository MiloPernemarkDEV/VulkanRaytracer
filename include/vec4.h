#pragma once

#include "defines.h"

namespace Math {
    struct vec4 {
        float x, y, z, w;

        FORCE_INLINE constexpr vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
        FORCE_INLINE constexpr vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        FORCE_INLINE constexpr vec4 operator+(const vec4& v) const {
            return {x + v.x, y + v.y, z + v.z, w + v.w};
        }

        FORCE_INLINE constexpr vec4 operator-(const vec4& v) const {
            return {x - v.x, y - v.y, z - v.z, w - v.w};
        }

        FORCE_INLINE constexpr vec4 operator*(const f32 s) const {
            return {s * x, s * y, s * z, s * w};
        }

        [[nodiscard]] FORCE_INLINE constexpr bool isZero() const {
            return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
        }
    };
}
