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

#include "defaults.hpp"
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

        constexpr auto drop() noexcept -> void {
        }

        constexpr auto reset(ValueType value) noexcept -> void {
            _value = value;
        }

        [[nodiscard]] constexpr auto borrow() noexcept -> BorrowedValueType {
            return _value;
        }

        [[nodiscard]] constexpr auto borrow() const noexcept -> ConstBorrowedValueType {
            return _value;
        }

        [[nodiscard]] constexpr auto get() noexcept -> ValueType {
            return _value;
        }

        constexpr auto operator=(ValueType value) noexcept -> Self& {
            _value = value;
            return *this;
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> Pointer {
            return _value;
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> ConstPointer {// NOLINT
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
        constexpr Box() noexcept :
                _value(nullptr) {
        }

        constexpr Box(ValueType value) noexcept :// NOLINT
                _value(&value) {
        }

        constexpr Box(const Self& other) noexcept = default;

        constexpr Box(Self&& other) noexcept = default;

        ~Box() noexcept = default;

        constexpr auto drop() noexcept -> void {
        }

        constexpr auto reset(ValueType value) noexcept -> void {
            _value = &value;
        }

        [[nodiscard]] constexpr auto borrow() noexcept -> BorrowedValueType {
            return *_value;
        }

        [[nodiscard]] constexpr auto borrow() const noexcept -> ConstBorrowedValueType {// NOLINT
            return *_value;
        }

        [[nodiscard]] constexpr auto get() noexcept -> ValueType {
            return *_value;
        }

        constexpr auto operator=(const Self& other) noexcept -> Self& = default;

        constexpr auto operator=(Self&&) noexcept -> Self& = default;

        constexpr auto operator=(ValueType value) noexcept -> Self& {
            _value = &value;
            return *this;
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> Pointer {
            return _value;
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> ConstPointer {
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
        static_assert(meta::is_move_assignable<T>, "Box type is not move assignable");
        static_assert(meta::is_move_constructible<T>, "Box type is not move constructible");
        static_assert(meta::is_default_constructible<T>, "Box type is not default constructible");

        [[maybe_unused]] static constexpr bool is_pointer = false;
        [[maybe_unused]] static constexpr bool is_reference = false;
        [[maybe_unused]] static constexpr bool is_value = true;

        using ValueType = T;
        using Self = Box<ValueType, meta::DefIf<!meta::is_ref<ValueType> && !meta::is_ptr<ValueType>>>;
        using BorrowedValueType = ValueType&;
        using ConstBorrowedValueType = const ValueType&;
        using Pointer = ValueType*;
        using ConstPointer = const ValueType*;

        private:
        ValueType _value;

        public:
        constexpr Box() noexcept :
                _value(ValueType()) {
        }

        constexpr Box(const ValueType& value) noexcept :// NOLINT
                _value(value) {
        }

        constexpr Box(ValueType&& value) noexcept :// NOLINT
                _value(utils::move(value)) {
        }

        constexpr Box(const Self& other) noexcept :
                _value(other._value) {
        }

        constexpr Box(Self&& other) noexcept :
                _value(utils::move(other._value)) {
        }

        ~Box() noexcept {
            drop();
        }

        constexpr auto drop() noexcept -> void {
            if constexpr(meta::is_destructible<T>) { _value.~ValueType(); }
        }

        constexpr auto reset(const ValueType& value) noexcept -> void {
            drop();
            _value = value;
        }

        constexpr auto reset(ValueType&& value) noexcept -> void {
            drop();
            _value = utils::move(value);
        }

        [[nodiscard]] constexpr auto borrow() noexcept -> BorrowedValueType {
            return _value;
        }

        [[nodiscard]] constexpr auto borrow() const noexcept -> ConstBorrowedValueType {
            return _value;
        }

        [[nodiscard]] constexpr auto get() noexcept -> ValueType&& {
            return utils::move(_value);
        }

        constexpr auto operator=(const Self& other) noexcept -> Self& {
            drop();
            _value = other._value;
            return *this;
        }

        constexpr auto operator=(Self&& other) noexcept -> Self& {
            drop();
            _value = utils::move(other._value);
            return *this;
        }

        constexpr auto operator=(const ValueType& value) noexcept -> Self& {
            drop();
            _value = value;
            return *this;
        }

        constexpr auto operator=(ValueType&& value) noexcept -> Self& {
            drop();
            _value = utils::move(value);
            return *this;
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> Pointer {
            return &_value;
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> ConstPointer {
            return &_value;
        }

        [[nodiscard]] constexpr auto operator==(const Self& other) const noexcept -> bool {
            return _value == other._value;
        }

        [[nodiscard]] constexpr auto operator!=(const Self& other) const noexcept -> bool {
            return _value != other._value;
        }

        [[nodiscard]] constexpr auto operator==(const ValueType& value) const noexcept -> bool {
            return _value == value;
        }

        [[nodiscard]] constexpr auto operator!=(const ValueType& value) const noexcept -> bool {
            return _value != value;
        }
    };

    template<typename T>
    [[nodiscard]] constexpr auto make_box(T value) noexcept -> Box<T> {
        return Box<T>(utils::move_or_copy(value));
    }
}// namespace kstd