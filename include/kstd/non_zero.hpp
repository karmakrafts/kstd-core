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
#include "box.hpp"
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
        using Self = NonZero<ValueType>;
        using Reference = ValueType&;
        using ConstReference = const ValueType&;

        friend struct std::hash<Self>;

        private:
        ValueType _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(NonZero, Self, constexpr)

        constexpr NonZero() noexcept :
                _value {static_cast<ValueType>(0)} {
        }

        explicit constexpr NonZero(ValueType value) noexcept :
                _value {value} {
            assert_true(value != static_cast<ValueType>(0));
        }

        ~NonZero() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _value == static_cast<ValueType>(0);
        }

        [[nodiscard]] constexpr auto get() noexcept -> Reference {
            assert_false(is_empty());
            return _value;
        }

        [[nodiscard]] constexpr auto get() const noexcept -> ConstReference {
            assert_false(is_empty());
            return _value;
        }

        [[nodiscard]] constexpr operator Reference() noexcept {// NOLINT
            return get();
        }

        [[nodiscard]] constexpr operator ConstReference() const noexcept {// NOLINT
            return get();
        }
    };

    // Specialization for Box to simplify logic and possibly save a few bytes
    template<typename T>
    struct Box<NonZero<T>, void> final {
        using ValueType = T;
        using NzValueType = NonZero<ValueType>;
        using Self = Box<NzValueType, void>;
        using Reference = NzValueType&;
        using ConstReference = const NzValueType&;
        using Pointer = NzValueType*;
        using ConstPointer = const NzValueType*;

        private:
        NzValueType _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Box, Self, constexpr);

        constexpr Box() noexcept :
                _value {} {
        }

        constexpr Box(NzValueType value) noexcept :
                _value {std::move(value)} {
        }

        ~Box() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _value.is_empty();
        }

        [[nodiscard]] constexpr auto get() noexcept -> Reference {
            assert_false(is_empty());
            return _value;
        }

        [[nodiscard]] constexpr auto get() const noexcept -> ConstReference {
            assert_false(is_empty());
            return _value;
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> Reference {
            return &_value;
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> ConstReference {
            return &_value;
        }

        [[nodiscard]] constexpr auto operator==(const Self& other) const noexcept -> bool {
            return is_empty() == other.is_empty() && _value == other._value;
        }

        [[nodiscard]] constexpr auto operator!=(const Self& other) const noexcept -> bool {
            return is_empty() != other.is_empty() || _value != other._value;
        }

        [[nodiscard]] constexpr auto operator==(const ValueType& value) const noexcept -> bool {
            return !is_empty() && _value == value;
        }

        [[nodiscard]] constexpr auto operator!=(const ValueType& value) const noexcept -> bool {
            return is_empty() || _value != value;
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return !is_empty();
        }
    };
}// namespace kstd

KSTD_DEFAULT_HASH_T(KSTD_TEMPLATE((typename T)), (kstd::NonZero<T>), value._value)