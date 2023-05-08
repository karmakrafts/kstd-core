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
 * @since 06/05/2023
 */

#pragma once

#include <cstring>
#include <bit>
#include <utility>
#include "kstd/concepts.hpp"
#include "kstd/types.hpp"

namespace kstd {
    template<typename T> //
    KSTD_REQUIRES(!std::is_void<T>::value)
    struct Option;

    namespace {
        template<typename T> KSTD_REQUIRES(!std::is_void<T>::value)
        union OptionInner final {
            using value_type = typename std::conditional<std::is_reference<T>::value, typename std::remove_reference<T>::type*, T>::type;

            private:

            friend class Option<T>;

            value_type _value;
            u8 _dummy[sizeof(value_type)];

            static_assert(sizeof(_dummy) == sizeof(value_type));

            public:

            OptionInner() noexcept :
                    _dummy() {
                std::memset(_dummy, 0, sizeof(_dummy));
            }

            // @formatter:off
            ~OptionInner() noexcept {}
            // @formatter:on
        };

        template<typename T> KSTD_REQUIRES(!std::is_void<T>::value)
        struct StatefulOptionInner final {
            using inner_type = OptionInner<T>;
            using value_type = typename inner_type::value_type;

            private:

            friend class Option<T>;

            inner_type _inner;
            bool _is_present;

            public:

            StatefulOptionInner() noexcept :
                    _inner(),
                    _is_present(false) {
            }

            ~StatefulOptionInner() noexcept = default;
        };
    }

    template<typename T> //
    KSTD_REQUIRES(!std::is_void<T>::value)
    struct Option final {
        static constexpr bool _is_pointer = std::is_pointer<T>::value;
        static constexpr bool _is_reference = std::is_reference<T>::value;

        using self_type = Option<T>;
        using value_type = T;
        using naked_value_type = typename std::remove_all_extents<typename std::remove_reference<value_type>::type>::type;
        using inner_type = typename std::conditional<_is_reference, OptionInner<T>, StatefulOptionInner<T>>::type;
        using inner_value_type [[maybe_unused]] = typename inner_type::value_type;
        using borrowed_value_type = typename std::conditional<_is_pointer, T, naked_value_type&>::type;

        private:

        inner_type _inner;

        constexpr auto _set_value(inner_value_type value) noexcept -> void {
            if constexpr (_is_reference) {
                _inner._value = value; // Refs don't need a state flag
            }
            else {
                _inner._inner._value = value;
                _inner._is_present = true;
            }
        }

        [[nodiscard]] constexpr auto _get_value() noexcept -> inner_value_type {
            if constexpr (_is_reference) {
                return _inner._value;
            }
            else {
                return _inner._inner._value;
            }
        }

        constexpr auto _set_empty() noexcept -> void {
            if constexpr (_is_reference) {
                std::memset(_inner._dummy, 0, sizeof(void*));
            }
            else {
                _inner._is_present = false;
            }
        }

        public:

        Option() noexcept :
                _inner() {
        }

        Option(value_type value) noexcept : // NOLINT
                _inner() {
            if constexpr ((_is_pointer || _is_reference) || !std::is_trivial<T>::value) {
                if constexpr (_is_reference) {
                    _set_value(&value);
                }
                else {
                    _set_value(value);
                }
            }
            else {
                if constexpr (std::is_move_assignable<T>::value) {
                    _set_value(std::move(value));
                }
                else {
                    _set_value(value);
                }
            }
        }

        Option(const self_type& other) noexcept :
                _inner() {
            if (other) {
                release();
                _set_value(other._get_value()); // Copy value
            }
        }

        Option(self_type&& other) noexcept :
                _inner() {
            if (other) {
                release();

                if constexpr (_is_pointer || _is_reference) {
                    _set_value(other._get_value()); // Copy value
                }
                else {
                    _set_value(std::move(other._get_value())); // Move value
                }
            }
        }

        ~Option() noexcept {
            release();
        }

        constexpr auto release() noexcept -> void {
            if constexpr (!_is_pointer && !_is_reference) {
                if (is_value()) {
                    _get_value().~inner_value_type();
                }
            }

            _set_empty(); // Mark this option as empty after releasing ownership
        }

        constexpr auto operator =(const self_type& other) noexcept -> self_type& {
            if (this == &other) {
                return *this;
            }

            if (other) {
                release();
                _set_value(other._get_value()); // Copy value
            }

            return *this;
        }

        constexpr auto operator =(self_type&& other) noexcept -> self_type& {
            if (other) {
                release();

                if constexpr (_is_pointer || _is_reference) {
                    _set_value(other._get_value()); // Copy value
                }
                else {
                    _set_value(std::move(other._get_value())); // Move value
                }
            }

            return *this;
        }

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            if constexpr (_is_reference) {
                return std::bit_cast<const void*>(_inner._dummy) == nullptr;
            }
            else {
                return !_inner._is_present;
            }
        }

        [[nodiscard]] constexpr auto is_value() const noexcept -> bool {
            if constexpr (_is_reference) {
                return std::bit_cast<const void*>(_inner._dummy) != nullptr;
            }
            else {
                return _inner._is_present;
            }
        }

        [[nodiscard]] constexpr auto borrow_value() noexcept -> borrowed_value_type {
            #ifdef BUILD_DEBUG
            if (is_empty()) {
                throw std::runtime_error("Result has no value");
            }
            #endif

            if constexpr (_is_reference) {
                return *_inner._value;
            }
            else {
                return _inner._inner._value;
            }
        }

        [[nodiscard]] constexpr auto get_value() noexcept -> T {
            #ifdef BUILD_DEBUG
            if (is_empty()) {
                throw std::runtime_error("Result has no value");
            }
            #endif

            if constexpr (_is_reference) {
                return *_inner._value;
            }
            else if constexpr (_is_pointer) {
                return _inner._inner._value;
            }
            else {
                return std::move(_inner._value);
            }
        }

        [[nodiscard]] constexpr operator bool() const noexcept { // NOLINT
            return is_value();
        }

        [[nodiscard]] constexpr auto operator *() noexcept -> decltype(auto) {
            return borrow_value();
        }

        [[nodiscard]] constexpr auto operator ->() noexcept -> naked_value_type* {
            return &borrow_value();
        }
    };

    template<typename T>
    KSTD_REQUIRES(!std::is_void<T>::value)
    [[nodiscard]] constexpr auto make_empty() noexcept -> Option<T> {
        return Option<T>();
    }

    template<typename T>
    KSTD_REQUIRES(!std::is_void<T>::value)
    [[nodiscard]] constexpr auto make_value(T value) noexcept -> Option<T> {
        if constexpr (std::is_pointer<T>::value || std::is_reference<T>::value) {
            return Option<T>(value);
        }
        else {
            return Option<T>(std::move(value));
        }
    }
}