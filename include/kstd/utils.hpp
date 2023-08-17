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
 * @since 11/05/2023
 */

#pragma once

#include "types.hpp"
#include "unicode.hpp"

#include <utility>

#define KSTD_DEFAULT_HASH(t, ...)                                                                                      \
    template<>                                                                                                         \
    struct std::hash<t> final {                                                                                        \
        inline auto operator()(const t& value) const noexcept -> size_t {                                              \
            return kstd::utils::hash(__VA_ARGS__);                                                                     \
        }                                                                                                              \
    }

namespace kstd::utils {
    namespace {
        template<typename HEAD, typename... TAIL>
        constexpr auto hash(usize& value, HEAD head, TAIL&&... tail) noexcept -> void {
            std::hash<std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<HEAD>>>> hasher {};
            value ^= hasher(head) + 0x9E3779B9 + (value << 6) + (value >> 2);
            if constexpr(sizeof...(TAIL) > 0) {
                hash_impl<TAIL...>(value, std::forward<TAIL>(tail)...);
            }
        }
    }// namespace

    // TODO: Add documentation
    template<typename... TYPES>
    [[nodiscard]] constexpr auto hash(TYPES&&... values) noexcept -> usize {
        usize result = 0;
        hash<TYPES...>(result, std::forward<TYPES>(values)...);
        return result;
    }

    /**
     * Transmutes the type of the given value while retaining the exact memory layout.
     * This is equal to performing an std::bitcast, but since that function is not portable
     * on C++17, we need to implement it ourselves.
     *
     * @tparam R The type to transmute the given reference into.
     * @tparam T The type of the reference to transmute. Usually deduced automatically.
     * @param value The reference to transmute into a different type.
     * @return A reference to the given object of the specified return type.
     */
    template<typename R, typename T>
    [[nodiscard]] constexpr auto transmute(T& value) noexcept -> R& {
        return *reinterpret_cast<R*>(&value);// NOLINT
    }

    /**
     * Transmutes the type of the given value while retaining the exact memory layout.
     * This is equal to performing an std::bitcast, but since that function is not portable
     * on C++17, we need to implement it ourselves.
     *
     * @tparam R The type to transmute the given reference into.
     * @tparam T The type of the reference to transmute. Usually deduced automatically.
     * @param value The reference to transmute into a different type.
     * @return A reference to the given object of the specified return type.
     */
    template<typename R, typename T>
    [[nodiscard]] constexpr auto transmute(const T& value) noexcept -> const R& {
        return *reinterpret_cast<const R*>(&value);// NOLINT
    }

    /**
     * Converts the given std::string into an std::wstring using the kstd::unicode API.
     * @tparam TRAITS The character traits applied to the resulting string.
     * @tparam ALLOCATOR The allocator used for allocating the converted string.
     * @param value The value to convert to a wide string.
     * @return A new std::wstring with the UTF-16 encoded contents of the given UTF-8 std::string.
     */
    template<typename TRAITS = std::char_traits<wchar_t>, typename ALLOCATOR = std::allocator<wchar_t>>
    [[nodiscard]] inline auto to_wcs(const std::string& value) noexcept
            -> std::basic_string<wchar_t, TRAITS, ALLOCATOR> {
        return unicode::convert<wchar_t, TRAITS, ALLOCATOR>(value);
    }

    /**
     * Converts the given std::wstring into an std::string using the kstd::unicode API.
     * @tparam TRAITS The character traits applied to the resulting string.
     * @tparam ALLOCATOR The allocator used for allocating the converted string.
     * @param value The value to convert to a multibyte string.
     * @return A new std::string with the UTF-8 encoded contents of the given UTF-16 string.
     */
    template<typename TRAITS = std::char_traits<char>, typename ALLOCATOR = std::allocator<char>>
    [[nodiscard]] inline auto to_mbs(const std::wstring& value) noexcept -> std::basic_string<char, TRAITS, ALLOCATOR> {
        return unicode::convert<char, TRAITS, ALLOCATOR>(value);
    }
}// namespace kstd::utils