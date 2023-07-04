/**
 * @author Alexander Hinze
 * @since 26/04/2023
 */

#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <variant>

namespace kstd {
    // NOLINTBEGIN
    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;
    using isize = std::ptrdiff_t;
    // NOLINTEND

    static_assert(sizeof(i8) == 1);
    static_assert(sizeof(i16) == 2);
    static_assert(sizeof(i32) == 4);
    static_assert(sizeof(i64) == 8);// NOLINT
    static_assert(sizeof(isize) == sizeof(void*));

    // NOLINTBEGIN
    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;
    using usize = std::size_t;
    // NOLINTEND

    static_assert(sizeof(u8) == 1);
    static_assert(sizeof(u16) == 2);
    static_assert(sizeof(u32) == 4);
    static_assert(sizeof(u64) == 8);// NOLINT
    static_assert(sizeof(usize) == sizeof(void*));

    // NOLINTBEGIN
    using f32 = float;
    using f64 = double;
    using f64ex = long double;
    // NOLINTEND

    static_assert(sizeof(f32) == 4);
    static_assert(sizeof(f64) == 8);                         // NOLINT
    static_assert(sizeof(f64ex) >= 8 && sizeof(f64ex) <= 16);// NOLINT

    // Atomics
    // NOLINTBEGIN
    using atomic_i8 = std::atomic_int8_t;
    using atomic_i16 = std::atomic_int16_t;
    using atomic_i32 = std::atomic_int32_t;
    using atomic_i64 = std::atomic_int64_t;
    using atomic_isize = std::atomic_ptrdiff_t;

    using atomic_u8 = std::atomic_uint8_t;
    using atomic_u16 = std::atomic_uint16_t;
    using atomic_u32 = std::atomic_uint32_t;
    using atomic_u64 = std::atomic_uint64_t;
    using atomic_usize = std::atomic_size_t;

    using atomic_f32 = std::atomic<f32>;
    using atomic_f64 = std::atomic<f64>;

    using std::atomic_bool;
    using std::atomic_char;
    using atomic_wchar = std::atomic_wchar_t;
    // NOLINTEND
}// namespace kstd