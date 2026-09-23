#pragma once

#include "defines.h"
#include "math_util.h"

namespace Math {
    struct Vec2 {
        float x, y;

        FORCE_INLINE constexpr Vec2() : x(0.0f), y(0.0f) {}

        FORCE_INLINE constexpr Vec2(float x, float y) : x(x), y(y) {}

        static const Vec2 zero;
        static const Vec2 up;
        static const Vec2 forward;
        static const Vec2 right;
        static const Vec2 left;

        FORCE_INLINE constexpr Vec2 operator+(const Vec2& v) const {
            return {x + v.x, y + v.y};
        }

        FORCE_INLINE constexpr Vec2 operator-(const Vec2& v) const {
            return {x - v.x, y - v.y};
        }

        FORCE_INLINE constexpr Vec2 operator*(const f32 s) const {
            return {s * x, s * y};
        }

        [[nodiscard]] FORCE_INLINE constexpr Vec2 normalized() const {
            if (isZero()) {
                return zero;
            }
            const f32 mag = magnitude();

            return {x / mag, y / mag};
        }

        [[nodiscard]] FORCE_INLINE constexpr f32 magnitude() const {
            return Math::pythagoras2(x, y);
        }

        [[nodiscard]] FORCE_INLINE constexpr bool isZero() const {
            return x == 0.0f && y == 0.0f;
        }
    };

    constexpr Vec2 Vec2::zero    = Vec2(0.0f, 0.0f);
    constexpr Vec2 Vec2::up      = Vec2(0.0f, 1.0f);
    constexpr Vec2 Vec2::forward = Vec2(0.0f, 0.0f);
    constexpr Vec2 Vec2::right   = Vec2(1.0f, 0.0f);
    constexpr Vec2 Vec2::left    = Vec2(-1.0f, 0.0f);
}
