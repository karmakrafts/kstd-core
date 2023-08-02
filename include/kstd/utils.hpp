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

#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#ifdef COMPILER_MSVC
#include <codecvt>
#endif

namespace kstd::utils {
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

}// namespace kstd::utils