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
#include "kstd/concepts.hpp"
#include "kstd/types.hpp"

namespace kstd {
    template<typename T> //
    KSTD_REQUIRES(!std::is_void<T>::value)
    struct Option;

    namespace {
        template<typename T>
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
    }

    template<typename T> //
    KSTD_REQUIRES(!std::is_void<T>::value)
    struct Option final {
        using self_type = Option<T>;
        using value_type = T;
        using naked_value_type = typename std::remove_all_extents<typename std::remove_reference<value_type>::type>::type;
        using inner_type = OptionInner<T>;
        using inner_value_type = typename inner_type::value_type;

        private:

        static constexpr bool _is_pointer = std::is_pointer<T>::value;
        static constexpr bool _is_reference = std::is_reference<T>::value;

        inner_type _inner;

        public:

        using borrowed_value_type = typename std::conditional<_is_pointer, T, naked_value_type&>::type;

        Option() noexcept :
                _inner() {
        }

        Option(value_type value) noexcept : // NOLINT
                _inner() {
            if constexpr ((_is_pointer || _is_reference) || !std::is_trivial<T>::value) {
                if constexpr (_is_reference) {
                    _inner._value = &value;
                }
                else {
                    _inner._value = value;
                }
            }
            else {
                if constexpr (std::is_move_assignable<T>::value) {
                    _inner._value = std::move(value);
                }
                else {
                    _inner._value = value;
                }
            }
        }

        ~Option() noexcept {
            if (is_value()) {
                if constexpr (!_is_pointer && !_is_reference) {
                    _inner._value.~inner_value_type();
                }
            }
        }

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            constexpr auto size = sizeof(_inner._dummy);
            u8 dummy[size];
            std::memset(dummy, 0, size);
            return std::memcmp(_inner._dummy, dummy, size) == 0;
        }

        [[nodiscard]] constexpr auto is_value() const noexcept -> bool {
            return !is_empty();
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
                return _inner._value;
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
                return _inner._value;
            }
            else {
                return std::move(_inner._value);
            }
        }

        [[nodiscard]] constexpr operator bool() const noexcept { // NOLINT
            return is_value();
        }

        [[nodiscard]] constexpr auto operator *() noexcept -> decltype(auto) {
            return get_value();
        }
    };

    template<typename T>
    //
    KSTD_REQUIRES(!std::is_void<T>::value)
    [[nodiscard]] constexpr auto make_empty() noexcept -> decltype(auto) {
        return Option<T>();
    }

    template<typename T>
    //
    KSTD_REQUIRES(!std::is_void<T>::value)
    [[nodiscard]] constexpr auto make_value(T value) noexcept -> decltype(auto) {
        if constexpr (std::is_pointer<T>::value || std::is_reference<T>::value) {
            return Option<T>(value);
        }
        else {
            return Option<T>(std::move(value));
        }
    }
}