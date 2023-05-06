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
 * @since 05/05/2023
 */

#pragma once

#include <type_traits>

#define KSTD_BITFLAGS(t, n, ...) enum class n : t {                  \
    NONE = 0,                                                        \
    __VA_ARGS__                                                      \
};                                                                   \
[[nodiscard]] constexpr auto operator |(n a, n b) noexcept -> n {    \
    return static_cast<n>(static_cast<t>(a) | static_cast<t>(b));    \
}                                                                    \
[[nodiscard]] constexpr auto operator &(n a, n b) noexcept -> n {    \
    return static_cast<n>(static_cast<t>(a) & static_cast<t>(b));    \
}                                                                    \
[[nodiscard]] constexpr auto operator ^(n a, n b) noexcept -> n {    \
    return static_cast<n>(static_cast<t>(a) ^ static_cast<t>(b));    \
}                                                                    \
[[nodiscard]] constexpr auto operator ~(n x) noexcept -> n {         \
    return static_cast<n>(~static_cast<t>(x));                       \
}                                                                    \
constexpr auto operator |=(n& a, n b) noexcept -> n& {               \
    a = a | b;                                                       \
    return a;                                                        \
}                                                                    \
constexpr auto operator &=(n& a, n b) noexcept -> n& {               \
    a = a & b;                                                       \
    return a;                                                        \
}                                                                    \
constexpr auto operator ^=(n& a, n b) noexcept -> n& {               \
    a = a ^ b;                                                       \
    return a;                                                        \
}