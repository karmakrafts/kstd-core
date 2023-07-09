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

#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace kstd::utils {
    template<typename T>
    [[nodiscard]] constexpr auto move_or_pass(T&& value) noexcept
            -> std::conditional_t<std::is_reference_v<T> || std::is_pointer_v<T>, T, std::remove_reference_t<T>&&> {
        if constexpr(std::is_reference_v<T> || std::is_pointer_v<T>) {
            return value;
        }
        else {
            return std::move(std::forward<T>(value));
        }
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
     * Creates a new UTF-8 string from the contents of the given UTF-16 string.
     *
     * @param value The UTF-16 string to copy data from into the newly created string.
     * @return A new UTF-8 string with the contents of the given UTF-16 string.
     */
    [[nodiscard]] inline auto to_utf8(const std::wstring& value) noexcept -> std::string {
        return {value.begin(), value.end()};
    }

    /**
     * Creates a new UTF-8 string from the contents of the given UTF-16 string view.
     *
     * @param value The UTF-16 string view to copy data from into the newly created string.
     * @return A new UTF-8 string with the contents of the given UTF-16 string view.
     */
    [[nodiscard]] inline auto to_utf8(std::wstring_view value) noexcept -> std::string {
        return {value.begin(), value.end()};
    }

    /**
     * Creates a new UTF-16 string from the contents of the given UTF-8 string.
     *
     * @param value The UTF-8 string to copy data from into the newly created string.
     * @return A new UTF-16 string with the contents of the given UTF-8 string.
     */
    [[nodiscard]] inline auto to_utf16(const std::string& value) noexcept -> std::wstring {
        return {value.begin(), value.end()};
    }

    /**
     * Creates a new UTF-16 string from the contents of the given UTF-8 string view.
     *
     * @param value The UTF-8 string view to copy data from into the newly created string.
     * @return A new UTF-16 string with the contents of the given UTF-8 string view.
     */
    [[nodiscard]] inline auto to_utf16(std::string_view value) noexcept -> std::wstring {
        return {value.begin(), value.end()};
    }
}// namespace kstd::utils