// Copyright 2023 Karma Krafts & associates
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @author Alexander Hinze
 * @since 18/08/2023
 */

#pragma once

#include "types.hpp"
#include "utils.hpp"

#include <type_traits>

// NOLINTBEGIN
#define KSTD_HASH_T(T, t, x)                                                                                           \
    T struct std::hash<KSTD_UNPAREN(t)> {                                                                              \
        inline auto operator()([[maybe_unused]] const KSTD_UNPAREN(t) & value) const noexcept -> size_t {              \
            return x;                                                                                                  \
        }                                                                                                              \
    };

#define KSTD_HASH(t, x) KSTD_HASH_T(template<>, t, x)
#define KSTD_DEFAULT_HASH_T(T, t, ...) KSTD_HASH_T(T, t, kstd::hash(__VA_ARGS__))
#define KSTD_DEFAULT_HASH(t, ...) KSTD_HASH(t, kstd::hash(__VA_ARGS__))
// NOLINTEND

namespace kstd {
    constexpr auto combined_hash_into(usize& first, usize second) noexcept -> void {
        first ^= second + 0x9E3779B9 + (first << 6) + (first >> 2);
    }

    [[nodiscard]] constexpr auto combined_hash(usize first, usize second) noexcept -> usize {
        usize result = first;
        combined_hash_into(result, second);
        return result;
    }

    template<typename HEAD, typename... TAIL>
    constexpr auto hash_into(usize& value, HEAD head, TAIL&&... tail) noexcept -> void {
        using Type = std::remove_cv_t<std::remove_reference_t<HEAD>>;
        const std::hash<Type> hasher {};
        combined_hash_into(value, hasher(head));
        if constexpr(sizeof...(TAIL) > 0) {
            hash_into<TAIL...>(value, std::forward<TAIL>(tail)...);
        }
    }

    template<typename... TYPES>
    [[nodiscard]] constexpr auto hash(TYPES&&... values) noexcept -> usize {
        usize result = 0;
        hash_into<TYPES...>(result, std::forward<TYPES>(values)...);
        return result;
    }

    template<typename ITERATOR>
    [[nodiscard]] constexpr auto hash_range(ITERATOR begin, ITERATOR end) noexcept -> usize {
        usize result = 0;
        while(begin != end) {
            combined_hash_into(result,  hash(*begin));
            ++begin;
        }
        return result;
    }
}// namespace kstd