#pragma once

#include "defines.h"
#include "math_util.h"

struct Vec3 {
    float x, y, z;

    FORCE_INLINE constexpr Vec3()
        : x(0.0f), y(0.0f), z(0.0f) {}

    FORCE_INLINE constexpr Vec3(float x, float y, float z)
        : x(x), y(y), z(z) {}

    static const Vec3 zero;
    static const Vec3 up;
    static const Vec3 forward;
    static const Vec3 right;
    static const Vec3 left;

    FORCE_INLINE constexpr Vec3 operator+(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }

    FORCE_INLINE constexpr Vec3 operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }

    FORCE_INLINE constexpr Vec3 operator*(const f32 s) const {
        return Vec3(s * x, s * y, s * z);
    }

    FORCE_INLINE constexpr Vec3 normalized() const {
        if (isZero()) {
            return zero;
        }
        const f32 mag = magnitude();

        return Vec3(x / mag, y / mag, z / mag);
    }

    FORCE_INLINE constexpr f32 magnitude() const {
        return pythagoras3(x, y, z);
    }

    FORCE_INLINE constexpr bool isZero() const {
        return x == 0.0f && y == 0.0f && z == 0.0f;
    }

    FORCE_INLINE constexpr f32 dot(const Vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    FORCE_INLINE constexpr Vec3 cross(const Vec3& v) const {
        return Vec3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    FORCE_INLINE constexpr f32 cos(const Vec3& v) const {
        return this->dot(v) / this->magnitude() / v.magnitude();
    }

    FORCE_INLINE constexpr bool hasSimilarDirection(const Vec3& v, const float maxAngleRadians) const {
        return normalized().dot(v.normalized()) >= std::cos(maxAngleRadians);
    }

};

constexpr Vec3 Vec3::zero    = Vec3(0.0f, 0.0f, 0.0f);
constexpr Vec3 Vec3::up      = Vec3(0.0f, 1.0f, 0.0f);
constexpr Vec3 Vec3::forward = Vec3(0.0f, 0.0f, 1.0f);
constexpr Vec3 Vec3::right   = Vec3(1.0f, 0.0f, 0.0f);
constexpr Vec3 Vec3::left    = Vec3(-1.0f, 0.0f, 0.0f);