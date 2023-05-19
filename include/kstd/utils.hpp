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

#include "meta.hpp"

namespace kstd {
    namespace {
        template<typename T, usize SIZE, usize INDEX>
        constexpr auto _fill_array(T (& array)[SIZE], const T& value) noexcept -> void {
            array[INDEX] = value;

            if constexpr (INDEX < SIZE) {
                _fill_array<T, SIZE, INDEX + 1>(array, value);
            }
        }
    }

    template<typename T, usize SIZE>
    constexpr auto fill_array(T (& array)[SIZE], const T& value) noexcept -> void {
        _fill_array<T, SIZE, 0>(array, value);
    }

    template<typename T>
    [[nodiscard]] constexpr auto forward(T value) noexcept -> T {
        return value;
    }

    template<typename T>
    [[nodiscard]] constexpr auto forward(meta::remove_ref<T>& value) noexcept -> T&& {
        return static_cast<T&&>(value);
    }

    template<typename T>
    [[nodiscard]] constexpr auto forward(meta::remove_ref<T>&& value) noexcept -> T&& {
        return static_cast<T&&>(value);
    }

    template<typename T>
    [[nodiscard]] constexpr auto move(T&& value) noexcept -> meta::remove_ref<T>&& {
        return static_cast<meta::remove_ref<T>&&>(value);
    }

    template<typename T>
    [[nodiscard]] constexpr auto move_or_copy(T&& value) noexcept -> decltype(auto) {
        if constexpr ((meta::is_ref<T> || meta::is_ptr<T>) || !meta::is_move_constructible<T>) {
            return value;
        }
        else {
            return move(value);
        }
    }
}