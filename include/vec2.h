#pragma once

#include "defines.h"
#include "math_util.h"

namespace Math {
    struct vec2 {
        float x, y;

        FORCE_INLINE constexpr vec2() : x(0.0f), y(0.0f) {}
        FORCE_INLINE constexpr vec2(float x, float y) : x(x), y(y) {}

        static const vec2 zero;
        static const vec2 up;
        static const vec2 forward;
        static const vec2 right;
        static const vec2 left;

        FORCE_INLINE constexpr vec2 operator+(const vec2& v) const {
            return {x + v.x, y + v.y};
        }

        FORCE_INLINE constexpr vec2 operator-(const vec2& v) const {
            return {x - v.x, y - v.y};
        }

        FORCE_INLINE constexpr vec2 operator*(const f32 s) const {
            return {s * x, s * y};
        }

        [[nodiscard]] FORCE_INLINE constexpr vec2 normalized() const {
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

        template<std::floating_point T>
        FORCE_INLINE constexpr vec2 lerp(const vec2& to, T alpha) {
            vec2 r = *this;
            r.x = Math::lerp(r.x, to.x, alpha);
            r.y = Math::lerp(r.y, to.y, alpha);
            return r;
        }

        template<std::floating_point T>
        FORCE_INLINE constexpr vec2 clamp(T min, T max) {
            return vec2(
                Math::clamp(x, min, max),
                Math::clamp(y, min, max)
            );
        }
    };

    constexpr vec2 vec2::zero    = vec2(0.0f, 0.0f);
    constexpr vec2 vec2::up      = vec2(0.0f, 1.0f);
    constexpr vec2 vec2::forward = vec2(0.0f, 0.0f);
    constexpr vec2 vec2::right   = vec2(1.0f, 0.0f);
    constexpr vec2 vec2::left    = vec2(-1.0f, 0.0f);
}
