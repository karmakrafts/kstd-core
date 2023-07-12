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
        using value_type        = T;
        using self              = Box<value_type, _>;
        using reference         = value_type&;
        using const_reference   = const value_type&;
        using pointer           = value_type*;
        using const_pointer     = const value_type*;
        // clang-format on

        private:
        std::variant<value_type, Void> _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Box, self, constexpr)

        constexpr Box() noexcept :
                _value(Void()) {
        }

        constexpr Box(value_type value) noexcept :// NOLINT
                _value(std::move(value)) {
        }

        ~Box() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return std::holds_alternative<Void>(_value);
        }

        [[nodiscard]] constexpr auto get() noexcept -> reference {
            assert_false(is_empty());
            return std::get<value_type>(_value);
        }

        [[nodiscard]] constexpr auto get() const noexcept -> const_reference {
            assert_false(is_empty());
            return std::get<value_type>(_value);
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> reference {
            return get();
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> const_reference {
            return get();
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> pointer {
            return &get();
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> const_pointer {
            return &get();
        }

        [[nodiscard]] constexpr auto operator==(const self& other) const noexcept -> bool {
            return is_empty() == other.is_empty() && std::get<value_type>(_value) == std::get<value_type>(other._value);
        }

        [[nodiscard]] constexpr auto operator!=(const self& other) const noexcept -> bool {
            return is_empty() != other.is_empty() || std::get<value_type>(_value) != std::get<value_type>(other._value);
        }

        [[nodiscard]] constexpr auto operator==(const value_type& value) const noexcept -> bool {
            return !is_empty() && std::get<value_type>(_value) == value;
        }

        [[nodiscard]] constexpr auto operator!=(const value_type& value) const noexcept -> bool {
            return is_empty() || std::get<value_type>(_value) != value;
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
        using value_type        = T;
        using naked_value_type  = std::decay_t<value_type>;
        using self              = Box<value_type, std::enable_if_t<std::is_pointer_v<T>>>;
        using reference         = value_type&;
        using const_reference   = const value_type&;
        using pointer           = value_type;
        using const_pointer     = const value_type;
        // clang-format on

        private:
        value_type _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Box, self, constexpr)

        constexpr Box() noexcept :
                _value(nullptr) {
        }

        constexpr Box(value_type value) noexcept :// NOLINT
                _value(value) {
        }

        ~Box() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _value == nullptr;
        }

        [[nodiscard]] constexpr auto get() noexcept -> reference {
            return _value;
        }

        [[nodiscard]] constexpr auto get() const noexcept -> const_reference {
            return _value;
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> reference {
            return get();
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> const_reference {
            return get();
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> pointer {
            return get();
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> const_pointer {// NOLINT
            return get();
        }

        [[nodiscard]] constexpr auto operator==(const self& other) const noexcept -> bool {
            return is_empty() == other.is_empty() && _value == other._value;
        }

        [[nodiscard]] constexpr auto operator!=(const self& other) const noexcept -> bool {
            return is_empty() != other.is_empty() || _value != other._value;
        }

        [[nodiscard]] constexpr auto operator==(const_pointer pointer) const noexcept -> bool {
            return !is_empty() && _value == pointer;
        }

        [[nodiscard]] constexpr auto operator!=(const_pointer pointer) const noexcept -> bool {
            return is_empty() || _value != pointer;
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
        using value_type        = T;
        using reference         = value_type&;
        using const_reference   = const value_type&;
        using naked_value_type  = std::decay_t<value_type>;
        using pointer           = naked_value_type*;
        using const_pointer     = const naked_value_type*;
        using self              = Box<value_type, std::enable_if_t<std::is_reference_v<T>>>;
        using stored_type       = std::conditional_t<std::is_const_v<std::remove_reference_t<value_type>>,
                                                    const_pointer, pointer>;
        // clang-format on

        private:
        stored_type _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Box, self, constexpr)

        constexpr Box() noexcept :
                _value(nullptr) {
        }

        constexpr Box(value_type value) noexcept :// NOLINT
                _value(&value) {
        }

        ~Box() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _value == nullptr;
        }

        [[nodiscard]] constexpr auto get() noexcept -> reference {
            assert_false(is_empty());
            return *_value;
        }

        [[nodiscard]] constexpr auto get() const noexcept -> const_reference {// NOLINT
            assert_false(is_empty());
            return *_value;
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> reference {
            return get();
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> const_reference {// NOLINT
            return get();
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> pointer {
            return &get();
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> const_pointer {
            return &get();
        }

        [[nodiscard]] constexpr auto operator==(const self& other) const noexcept -> bool {
            return is_empty() == other.is_empty() && _value == other._value;
        }

        [[nodiscard]] constexpr auto operator!=(const self& other) const noexcept -> bool {
            return is_empty() != other.is_empty() || _value != other._value;
        }

        [[nodiscard]] constexpr auto operator==(value_type ref) const noexcept -> bool {
            return !is_empty() && _value == &ref;
        }

        [[nodiscard]] constexpr auto operator!=(value_type ref) const noexcept -> bool {
            return is_empty() || _value != &ref;
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return !is_empty();
        }
    };
#ifdef BUILD_DEBUG
#endif
}// namespace kstd