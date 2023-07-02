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
 * @since 09/05/2023
 */

#pragma once

#include <algorithm>

#include "assert.hpp"
#include "defaults.hpp"
#include "libc.hpp"
#include "meta.hpp"
#include "utils.hpp"

namespace kstd {
    /*
     * A Box is a union-friendly way to store a pointer, reference or owned value.
     * Primarily used for things like kstd::Option and kstd::Result to make the code
     * a little easier on the eyes and more digestible.
     */
    template<typename T, typename = void>
    struct Box;

    /*
     * Specialization for pointers, always pass-by-value
     */
    template<typename T>
    struct Box<T, meta::DefIf<meta::is_ptr<T>>> final {
        [[maybe_unused]] static constexpr bool is_pointer = true;
        [[maybe_unused]] static constexpr bool is_reference = false;
        [[maybe_unused]] static constexpr bool is_value = false;

        using ValueType = T;
        using Self = Box<ValueType, meta::DefIf<meta::is_ptr<ValueType>>>;
        using StoredValueType = ValueType;
        using BorrowedValueType = ValueType&;
        using ConstBorrowedValueType = ValueType const&;
        using Pointer = ValueType;
        using ConstPointer = const ValueType;

        private:
        ValueType _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Box)

        constexpr Box() noexcept :
                _value(nullptr) {
        }

        constexpr Box(ValueType value) noexcept :// NOLINT
                _value(value) {
        }

        ~Box() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _value == nullptr;
        }

        [[nodiscard]] constexpr auto borrow() noexcept -> BorrowedValueType {
            assert_false(is_empty());
            return _value;
        }

        [[nodiscard]] constexpr auto borrow() const noexcept -> ConstBorrowedValueType {
            assert_false(is_empty());
            return _value;
        }

        [[nodiscard]] constexpr auto get() noexcept -> ValueType {
            assert_false(is_empty());
            return _value;
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> Pointer {
            assert_false(is_empty());
            return _value;
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> ConstPointer {// NOLINT
            assert_false(is_empty());
            return _value;
        }

        [[nodiscard]] constexpr auto operator==(const Self& other) const noexcept -> bool {
            return _value == other._value;
        }

        [[nodiscard]] constexpr auto operator!=(const Self& other) const noexcept -> bool {
            return _value != other._value;
        }

        [[nodiscard]] constexpr auto operator==(ConstPointer pointer) const noexcept -> bool {
            return _value == pointer;
        }

        [[nodiscard]] constexpr auto operator!=(ConstPointer pointer) const noexcept -> bool {
            return _value != pointer;
        }
    };

    /*
     * Specialization for references, stores a pointer
     */
    template<typename T>
    struct Box<T, meta::DefIf<meta::is_ref<T>>> final {
        [[maybe_unused]] static constexpr bool is_pointer = false;
        [[maybe_unused]] static constexpr bool is_reference = true;
        [[maybe_unused]] static constexpr bool is_value = false;

        using ValueType = T;
        using Self = Box<ValueType, meta::DefIf<meta::is_ref<ValueType>>>;
        using NakedValueType = meta::Naked<ValueType>;
        using BorrowedValueType = ValueType;
        using ConstBorrowedValueType = const ValueType;
        using Pointer = NakedValueType*;
        using ConstPointer = const NakedValueType*;
        using StoredValueType = meta::If<meta::is_const<ValueType>, ConstPointer, Pointer>;

        private:
        StoredValueType _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Box)

        constexpr Box() noexcept :
                _value(nullptr) {
        }

        constexpr Box(ValueType value) noexcept :// NOLINT
                _value(&value) {
        }

        ~Box() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _value == nullptr;
        }

        [[nodiscard]] constexpr auto borrow() noexcept -> BorrowedValueType {
            assert_false(is_empty());
            return *_value;
        }

        [[nodiscard]] constexpr auto borrow() const noexcept -> ConstBorrowedValueType {// NOLINT
            assert_false(is_empty());
            return *_value;
        }

        [[nodiscard]] constexpr auto get() noexcept -> ValueType {
            assert_false(is_empty());
            return *_value;
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> Pointer {
            assert_false(is_empty());
            return _value;
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> ConstPointer {
            assert_false(is_empty());
            return _value;
        }

        [[nodiscard]] constexpr auto operator==(const Self& other) const noexcept -> bool {
            return _value == other._value;
        }

        [[nodiscard]] constexpr auto operator!=(const Self& other) const noexcept -> bool {
            return _value != other._value;
        }

        [[nodiscard]] constexpr auto operator==(ValueType ref) const noexcept -> bool {
            return _value == &ref;
        }

        [[nodiscard]] constexpr auto operator!=(ValueType ref) const noexcept -> bool {
            return _value != &ref;
        }
    };

    /*
     * Specialization for owned values, uses move semantics
     */
    template<typename T>
    struct Box<T, meta::DefIf<!meta::is_ref<T> && !meta::is_ptr<T>>> final {
        [[maybe_unused]] static constexpr bool is_pointer = false;
        [[maybe_unused]] static constexpr bool is_reference = false;
        [[maybe_unused]] static constexpr bool is_value = true;

        using ValueType = T;
        using VariantType = std::variant<ValueType, std::array<u8, sizeof(ValueType)>>;
        using Self = Box<ValueType, meta::DefIf<!meta::is_ref<ValueType> && !meta::is_ptr<ValueType>>>;
        using BorrowedValueType = ValueType&;
        using ConstBorrowedValueType = const ValueType&;
        using Pointer = ValueType*;
        using ConstPointer = const ValueType*;

        private:
        VariantType _value;
        bool _has_value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Box)

        constexpr Box() noexcept :
                _has_value(false),
                _value(std::array<u8, sizeof(ValueType)>()) {
        }

        constexpr Box(const ValueType& value) noexcept :// NOLINT
                _value(value),
                _has_value(true) {
        }

        constexpr Box(ValueType&& value) noexcept :// NOLINT
                _value(utils::move(value)),
                _has_value(true) {
        }

        ~Box() noexcept {
            if(_has_value) {
                _value.~VariantType();
            }
        }

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return !_has_value;
        }

        [[nodiscard]] constexpr auto borrow() noexcept -> BorrowedValueType {
            assert_false(is_empty());
            return std::get<ValueType>(_value);
        }

        [[nodiscard]] constexpr auto borrow() const noexcept -> ConstBorrowedValueType {
            assert_false(is_empty());
            return std::get<ValueType>(_value);
        }

        [[nodiscard]] constexpr auto get() noexcept -> ValueType&& {
            assert_false(is_empty());
            _has_value = false;
            return utils::move(std::get<ValueType>(_value));
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> Pointer {
            assert_false(is_empty());
            return &std::get<ValueType>(_value);
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> ConstPointer {
            assert_false(is_empty());
            return &std::get<ValueType>(_value);
        }

        [[nodiscard]] constexpr auto operator==(const Self& other) const noexcept -> bool {
            return std::get<ValueType>(_value) == std::get<ValueType>(other._value);
        }

        [[nodiscard]] constexpr auto operator!=(const Self& other) const noexcept -> bool {
            return std::get<ValueType>(_value) != std::get<ValueType>(other._value);
        }

        [[nodiscard]] constexpr auto operator==(const ValueType& value) const noexcept -> bool {
            return std::get<ValueType>(_value) == value;
        }

        [[nodiscard]] constexpr auto operator!=(const ValueType& value) const noexcept -> bool {
            return std::get<ValueType>(_value) != value;
        }
    };

    template<typename T>
    [[nodiscard]] constexpr auto make_box(T value) noexcept -> Box<T> {
        return Box<T>(utils::move_or_copy(value));
    }
}// namespace kstd