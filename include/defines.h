#pragma once

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

constexpr u64 secondsToNano(u64 seconds) {
    return seconds * 1000000000ULL;
}

#define VK_CHECK(function, errormsg) \
    do { \
        if ((function) != VK_SUCCESS) { \
            throw std::runtime_error(errormsg); \
        } \
    } while (0)

#if defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define FORCE_INLINE inline __attribute__((always_inline))
#else
#define FORCE_INLINE inline
#endif