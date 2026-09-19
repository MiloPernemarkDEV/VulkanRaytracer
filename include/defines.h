#pragma once

#include <cstdint>

using s16 = int16_t; 
using s32 = int32_t;
using s64 = int64_t;

using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double; 

constexpr int operator""_MB(unsigned long long int x) {
	return x * 1024 * 1024;
}

template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
	if (v < lo) return lo;
	if (hi < v) return hi;
	return v;
}