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
#include <type_traits>

#include "assert.hpp"
#include "defaults.hpp"
#include "libc.hpp"
#include "utils.hpp"
#include "void.hpp"

namespace kstd {
    /**
     * A box implementation which is either empty or stores an owned value.
     * This means that the lifetime of the object contained within the box,
     * is the same as the box's lifetime itself.
     *
     * @tparam T The value type stored within the Box type.
     */
    template<typename T, typename _ = void>
    struct Box {
        static_assert(!std::is_same_v<std::decay_t<T>, Void>, "Type cannot be Void");

        // clang-format off
        using ValueType         = T;
        using NakedValueType    = std::remove_const_t<ValueType>;
        using Self              = Box<ValueType, _>;
        using Reference         = NakedValueType&;
        using ConstReference    = const NakedValueType&;
        using Pointer           = NakedValueType*;
        using ConstPointer      = const NakedValueType*;
        // clang-format on

        private:
        std::variant<NakedValueType, Void> _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Box, Self, constexpr)

        constexpr Box() noexcept :
                _value {Void {}} {
        }

        constexpr Box(ValueType value) noexcept :// NOLINT
                _value {std::move(value)} {
        }

        ~Box() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return std::holds_alternative<Void>(_value);
        }

        [[nodiscard]] constexpr auto get() noexcept -> Reference {
            assert_false(is_empty());
            return std::get<NakedValueType>(_value);
        }

        [[nodiscard]] constexpr auto get() const noexcept -> ConstReference {
            assert_false(is_empty());
            return std::get<NakedValueType>(_value);
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> Reference {
            return get();
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> ConstReference {
            return get();
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> Pointer {
            return &get();
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> ConstPointer {
            return &get();
        }

        [[nodiscard]] constexpr auto operator==(const Self& other) const noexcept -> bool {
            return is_empty() == other.is_empty() &&
                   std::get<NakedValueType>(_value) == std::get<NakedValueType>(other._value);
        }

        [[nodiscard]] constexpr auto operator!=(const Self& other) const noexcept -> bool {
            return is_empty() != other.is_empty() ||
                   std::get<NakedValueType>(_value) != std::get<NakedValueType>(other._value);
        }

        [[nodiscard]] constexpr auto operator==(ConstReference value) const noexcept -> bool {
            return !is_empty() && std::get<NakedValueType>(_value) == value;
        }

        [[nodiscard]] constexpr auto operator!=(ConstReference value) const noexcept -> bool {
            return is_empty() || std::get<NakedValueType>(_value) != value;
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return !is_empty();
        }
    };

    /**
     * A box implementation which is either empty or stores a pointer value.
     * This means that the contained value will always be passed by value,
     * since that most efficient for pointers.
     *
     * @tparam T The pointer type stored within the Box type.
     */
    template<typename T>
    struct Box<T, std::enable_if_t<std::is_pointer_v<T>>> final {
        static_assert(!std::is_same_v<std::decay_t<T>, Void>, "Type cannot be Void");

        // clang-format off
        using ValueType         = T;
        using NakedValueType    = std::remove_cv_t<std::remove_pointer_t<ValueType>>;
        using Self              = Box<ValueType, std::enable_if_t<std::is_pointer_v<T>>>;
        using Reference         = ValueType&;
        using ConstReference    = const ValueType&;
        using Pointer           = NakedValueType*;
        using ConstPointer      = const NakedValueType*;
        // clang-format on

        private:
        std::variant<ValueType, Void> _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Box, Self, constexpr)

        constexpr Box() noexcept :
                _value {Void {}} {
        }

        constexpr Box(ValueType value) noexcept :// NOLINT
                _value {value} {
        }

        ~Box() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return std::holds_alternative<Void>(_value);
        }

        [[nodiscard]] constexpr auto get() noexcept -> Reference {
            assert_false(is_empty());
            return std::get<ValueType>(_value);
        }

        [[nodiscard]] constexpr auto get() const noexcept -> ConstReference {
            assert_false(is_empty());
            return std::get<ValueType>(_value);
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> Reference {
            return get();
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> ConstReference {
            return get();
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> Pointer {
            return get();
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> ConstPointer {// NOLINT
            return get();
        }

        [[nodiscard]] constexpr auto operator==(const Self& other) const noexcept -> bool {
            return is_empty() == other.is_empty() && std::get<ValueType>(_value) == std::get<ValueType>(other._value);
        }

        [[nodiscard]] constexpr auto operator!=(const Self& other) const noexcept -> bool {
            return is_empty() != other.is_empty() || std::get<ValueType>(_value) != std::get<ValueType>(other._value);
        }

        [[nodiscard]] constexpr auto operator==(ConstPointer pointer) const noexcept -> bool {
            return !is_empty() && std::get<ValueType>(_value) == pointer;
        }

        [[nodiscard]] constexpr auto operator!=(ConstPointer pointer) const noexcept -> bool {
            return is_empty() || std::get<ValueType>(_value) != pointer;
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return !is_empty();
        }
    };

    /**
     * A box implementation which is either empty or stores a reference.
     * This means that the contained value will always be passed by value,
     * since that's the only thing that makes sense for references.
     * This implementation stores a pointer internally, to make this type
     * trivially copyable;
     * It takes the address of the given reference upon
     * construction and dereferences the address when the value is retrieved.
     * This implies a runtime assertion in debug mode which checks that the
     * reference is still valid, since we don't the underlying object's lifetime.
     *
     * @tparam T The pointer type stored within the Box type.
     */
    template<typename T>
    struct Box<T, std::enable_if_t<std::is_reference_v<T>>> final {
        static_assert(!std::is_same_v<std::decay_t<T>, Void>, "Type cannot be Void");

        // clang-format off
        using ValueType         = T;
        using NakedValueType    = std::remove_reference_t<std::remove_cv_t<std::decay_t<ValueType>>>;
        using ConstReference    = const NakedValueType&;
        using Reference         = std::conditional_t<std::is_const_v<std::remove_reference_t<ValueType>>,
                                    ConstReference,
                                    NakedValueType&>;
        using ConstPointer      = const NakedValueType*;
        using Pointer           = std::conditional_t<std::is_const_v<std::remove_reference_t<ValueType>>,
                                    ConstPointer,
                                    NakedValueType*>;
        using Self              = Box<ValueType, std::enable_if_t<std::is_reference_v<T>>>;
        using StoredType        = std::conditional_t<std::is_const_v<std::remove_reference_t<ValueType>>,
                                    ConstPointer, Pointer>;
        // clang-format on

        private:
        StoredType _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Box, Self, constexpr)

        constexpr Box() noexcept :
                _value {nullptr} {
        }

        constexpr Box(ValueType value) noexcept :// NOLINT
                _value {&value} {
        }

        ~Box() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _value == nullptr;
        }

        [[nodiscard]] constexpr auto get() noexcept -> Reference {
            assert_false(is_empty());
            return *_value;
        }

        [[nodiscard]] constexpr auto get() const noexcept -> ConstReference {// NOLINT
            assert_false(is_empty());
            return *_value;
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> Reference {
            return get();
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> ConstReference {// NOLINT
            return get();
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> Pointer {
            return &get();
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> ConstPointer {
            return &get();
        }

        [[nodiscard]] constexpr auto operator==(const Self& other) const noexcept -> bool {
            return is_empty() == other.is_empty() && _value == other._value;
        }

        [[nodiscard]] constexpr auto operator!=(const Self& other) const noexcept -> bool {
            return is_empty() != other.is_empty() || _value != other._value;
        }

        [[nodiscard]] constexpr auto operator==(ValueType ref) const noexcept -> bool {
            return !is_empty() && _value == &ref;
        }

        [[nodiscard]] constexpr auto operator!=(ValueType ref) const noexcept -> bool {
            return is_empty() || _value != &ref;
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return !is_empty();
        }
    };
#ifdef BUILD_DEBUG
#endif
}// namespace kstd