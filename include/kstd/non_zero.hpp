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
 * @since 22/05/2023
 */

#pragma once

#include "assert.hpp"
#include "defaults.hpp"

namespace kstd {
    template<typename T>
    struct NonZero final {
        using ValueType = T;
        using Self [[maybe_unused]] = NonZero<T>;

        private:
        ValueType _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(NonZero)

        constexpr NonZero() noexcept :
                _value(static_cast<ValueType>(0)) {
        }

        constexpr NonZero(ValueType value) noexcept :// NOLINT
                _value(value) {
            assert_true(value != static_cast<ValueType>(0));
        }

        ~NonZero() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _value == static_cast<ValueType>(0);
        }

        [[nodiscard]] constexpr auto get() noexcept -> ValueType {
            assert_false(is_empty());
            return _value;
        }

        [[nodiscard]] constexpr auto get() const noexcept -> ValueType {
            assert_false(is_empty());
            return _value;
        }
    };

    template<typename T>
    [[nodiscard]] constexpr auto make_non_zero(T value) noexcept -> NonZero<T> {
        return NonZero<T>(value);
    }
}// namespace kstd