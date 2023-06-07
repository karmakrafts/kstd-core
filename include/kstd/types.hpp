/**
 * @author Alexander Hinze
 * @since 26/04/2023
 */

#pragma once

#include <atomic>
#include <stddef.h>// NOLINT
#include <stdint.h>// NOLINT

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

    template<typename T>
    using Atomic = std::atomic<T>;// Not viable to implement our own atomics

    namespace limits {
        // min
        template<typename T>
        [[nodiscard]] constexpr auto min() noexcept -> T;

        template<>
        [[nodiscard]] constexpr auto min<u8>() noexcept -> u8 {
            return 0;
        }

        template<>
        [[nodiscard]] constexpr auto min<u16>() noexcept -> u16 {
            return 0;
        }

        template<>
        [[nodiscard]] constexpr auto min<u32>() noexcept -> u32 {
            return 0;
        }

        template<>
        [[nodiscard]] constexpr auto min<u64>() noexcept -> u64 {
            return 0;
        }

        template<>
        [[nodiscard]] constexpr auto min<i8>() noexcept -> i8 {
            return -0x80;
        }

        template<>
        [[nodiscard]] constexpr auto min<i16>() noexcept -> i16 {
            return -0x8000;
        }

        template<>
        [[nodiscard]] constexpr auto min<i32>() noexcept -> i32 {
            return -0x8000'0000;// NOLINT
        }

        template<>
        [[nodiscard]] constexpr auto min<i64>() noexcept -> i64 {
            return -0x8000'0000'0000'0000;// NOLINT
        }

        template<>
        [[nodiscard]] constexpr auto min<f32>() noexcept -> f32 {
            return 1.175494351E-38;
        }

        template<>
        [[nodiscard]] constexpr auto min<f64>() noexcept -> f64 {
            return 2.2250738585072014E-308;
        }

        // max
        template<typename T>
        [[nodiscard]] constexpr auto max() noexcept -> T;

        template<>
        [[nodiscard]] constexpr auto max<u8>() noexcept -> u8 {
            return 0xFF;
        }

        template<>
        [[nodiscard]] constexpr auto max<u16>() noexcept -> u16 {
            return 0xFFFF;
        }

        template<>
        [[nodiscard]] constexpr auto max<u32>() noexcept -> u32 {
            return 0xFFFF'FFFF;
        }

        template<>
        [[nodiscard]] constexpr auto max<u64>() noexcept -> u64 {
            return 0xFFFF'FFFF'FFFF'FFFF;
        }

        template<>
        [[nodiscard]] constexpr auto max<i8>() noexcept -> i8 {
            return 0x7F;
        }

        template<>
        [[nodiscard]] constexpr auto max<i16>() noexcept -> i16 {
            return 0x7FFF;
        }

        template<>
        [[nodiscard]] constexpr auto max<i32>() noexcept -> i32 {
            return 0x7FFF'FFFF;
        }

        template<>
        [[nodiscard]] constexpr auto max<i64>() noexcept -> i64 {
            return 0x7FFF'FFFF'FFFF'FFFF;
        }

        template<>
        [[nodiscard]] constexpr auto max<f32>() noexcept -> f32 {
            return 3.402823466E+38;
        }

        template<>
        [[nodiscard]] constexpr auto max<f64>() noexcept -> f64 {
            return 1.7976931348623158E+308;
        }
    }// namespace limits
}// namespace kstd