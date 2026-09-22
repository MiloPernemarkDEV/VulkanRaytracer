#pragma once

#include <cmath>
#include "defines.h"

namespace Math {
    template <typename T>
    constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
        if (v < lo) return lo;
        if (hi < v) return hi;
        return v;
    }

    constexpr s32 pythagoras2(const s32 a, const s32 b) {
        return static_cast<s32>(std::sqrt(a * a + b * b));
    }

    constexpr f32 pythagoras2(const f32 a, const f32 b) {
        return std::sqrt(a * a + b * b);
    }

    constexpr f64 pythagoras2(const f64 a, const f64 b) {
        return std::sqrt(a * a + b * b);
    }

    constexpr s32 pythagoras3(const s32 a, const s32 b, const s32 c) {
        return static_cast<s32>(std::sqrt(a * a + b * b + c * c));
    }

    constexpr f32 pythagoras3(const f32 a, const f32 b, const f32 c) {
        return std::sqrt(a * a + b * b + c * c);
    }

    constexpr f64 pythagoras3(const f64 a, const f64 b, const f64 c) {
        return std::sqrt(a * a + b * b + c * c);
    }
}

