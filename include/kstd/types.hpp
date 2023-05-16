/**
 * @author Alexander Hinze
 * @since 26/04/2023
 */

#pragma once

#include <stdint.h> // NOLINT
#include <stddef.h> // NOLINT
#include <atomic>

namespace kstd {
    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;
    using isize = std::ptrdiff_t;

    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;
    using usize = std::size_t;

    using f32 = float;
    using f64 = double;
    using f64ex = long double;

    template<typename T> //
    using Atomic = std::atomic<T>; // Not viable to implement our own atomics
}