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
    /**
     * Non-zero can be used for either runtime validation of pointers or parameters,
     * as well as an optimization in conjunction with types like kstd::Option in certain cases.
     *
     * @tparam T The type of integer or pointer to be stored within the non-zero type.
     */
    template<typename T>
    struct NonZero final {
        static_assert(std::is_integral_v<T> || std::is_pointer_v<T>, "Type is neither an integer nor a pointer");

        using ValueType = T;
        using Self = NonZero<T>;
        using BorrowedValueType = ValueType&;
        using ConstBorrowedValueType = const ValueType&;

        private:
        ValueType _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(NonZero, Self, constexpr)

        constexpr NonZero() noexcept :
                _value(static_cast<ValueType>(0)) {
        }

        constexpr NonZero(ValueType value) noexcept :// NOLINT
                _value(value) {
            assert_true(value != static_cast<ValueType>(0));
        }

        ~NonZero() noexcept = default;

        [[nodiscard]] constexpr operator ValueType() const noexcept {
            return _value;
        }

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _value == static_cast<ValueType>(0);
        }

        [[nodiscard]] constexpr auto get() noexcept -> BorrowedValueType {
            assert_false(is_empty());
            return _value;
        }

        [[nodiscard]] constexpr auto get() const noexcept -> ConstBorrowedValueType {
            assert_false(is_empty());
            return _value;
        }
    };
}// namespace kstd