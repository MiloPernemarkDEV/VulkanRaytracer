#pragma once

#include "defines.h"
#include "math_util.h"
#include <concepts>

namespace Math {
   struct vec3 {
        float x, y, z;

        FORCE_INLINE constexpr vec3() : x(0.0f), y(0.0f), z(0.0f) {}
        FORCE_INLINE constexpr vec3(float x, float y, float z) : x(x), y(y), z(z) {}

        static const vec3 zero;
        static const vec3 up;
        static const vec3 forward;
        static const vec3 right;
        static const vec3 left;

        FORCE_INLINE constexpr vec3 operator+(const vec3& v) const {
            return {x + v.x, y + v.y, z + v.z};
        }

        FORCE_INLINE constexpr vec3 operator-(const vec3& v) const {
            return {x - v.x, y - v.y, z - v.z};
        }

        FORCE_INLINE constexpr vec3 operator*(const f32 s) const {
            return {s * x, s * y, s * z};
        }

        [[nodiscard]] FORCE_INLINE constexpr vec3 normalized() const {
            if (isZero()) {
                return zero;
            }
            const f32 mag = magnitude();

            return {x / mag, y / mag, z / mag};
        }

        [[nodiscard]] FORCE_INLINE constexpr f32 magnitude() const {
            return Math::pythagoras3(x, y, z);
        }

        [[nodiscard]] FORCE_INLINE constexpr bool isZero() const {
            return x == 0.0f && y == 0.0f && z == 0.0f;
        }

        [[nodiscard]] FORCE_INLINE constexpr f32 dot(const vec3& v) const {
            return x * v.x + y * v.y + z * v.z;
        }

        [[nodiscard]] FORCE_INLINE constexpr vec3 cross(const vec3& v) const {
            return {
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x
            };
        }

        [[nodiscard]] FORCE_INLINE constexpr f32 cos(const vec3& v) const {
            return this->dot(v) / this->magnitude() / v.magnitude();
        }

        [[nodiscard]] FORCE_INLINE constexpr bool hasSimilarDirection(const vec3& v, const float maxAngleRadians) const {
            return normalized().dot(v.normalized()) >= std::cos(maxAngleRadians);
        }

        template<std::floating_point T>
        FORCE_INLINE constexpr vec3 lerp(const vec3& to, T alpha) {
            vec3 r = *this;
            r.x = Math::lerp(r.x, to.x, alpha);
            r.y = Math::lerp(r.y, to.y, alpha);
            r.z = Math::lerp(r.z, to.z, alpha);
            return r;
        }

        template<std::floating_point T>
        FORCE_INLINE constexpr vec3 clamp(T min, T max) {
            return vec3(
                Math::clamp(x, min, max),
                Math::clamp(y, min, max),
                Math::clamp(z, min, max)
            );
        }
    };
}

