#pragma once

#include <cmath>
#include "defines.h"
#include <concepts>

namespace Math {
    template <typename T>
    constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
        if (v < lo) return lo;
        if (hi < v) return hi;
        return v;
    }

    template<std::floating_point T>
    constexpr T pythagoras2(const T a, const T b) {
        return std::sqrt(a * a + b * b);
    }

    template<std::floating_point T>
    constexpr T pythagoras3(const T a, const T b, const T c) {
        return std::sqrt(a * a + b * b + c * c);
    }

    template<std::floating_point T>
    constexpr T lerp(T from, T to, T alpha) {
        return (1 - alpha) * from + alpha * to;
    }
}

