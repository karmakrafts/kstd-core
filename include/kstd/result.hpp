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
 * @since 26/04/2023
 */

#pragma once

#include <type_traits>
#include <string_view>
#include "types.hpp"

namespace kstd {
    template<typename E> requires std::is_standard_layout_v<E>
    class Error final {
        E _error;

        public:

        explicit constexpr Error(E error) noexcept:
                _error(std::move(error)) {
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> E& {
            return _error;
        }
    };

    namespace {
        enum class ResultType : u8 {
            OK,
            ERROR,
            EMPTY
        };

        template<usize ALIGN>
        struct alignas(ALIGN) DummyStruct final {
            u8 value;
        };

        template<typename T, typename E> requires std::is_standard_layout_v<E> && std::is_move_assignable_v<E>
        union ResultInner {
            using value_type = std::conditional_t<std::is_void_v<T>, u8, std::conditional_t<std::is_reference_v<T>, std::remove_reference_t<T>*, T>>;

            value_type _value;
            E _error;
            DummyStruct<std::max(sizeof(value_type), sizeof(E))> _dummy;

            constexpr ResultInner() noexcept:
                    _dummy({0}) {
            }

            constexpr ResultInner(value_type value) noexcept:
                    _value(value) {
            }

            constexpr ResultInner(Error<E> error) noexcept:
                    _error(std::move(error)) {
            }

            // @formatter:off
            constexpr ~ResultInner() noexcept {}
            // @formatter:on
        };
    }

    template<typename T, typename E = std::string_view> requires std::is_standard_layout_v<E> && std::is_move_assignable_v<E>
    struct Result final {
        // If the ok-type of the result is a void, we substitute an unsigned byte
        using value_type = std::conditional_t<std::is_void_v<T>, u8, T>;

        private:

        static constexpr bool _is_pointer = std::is_pointer_v<T>;
        static constexpr bool _is_reference = std::is_reference_v<T>;
        static constexpr bool _is_void = std::is_void_v<T>;

        ResultInner<T, E> _inner;
        ResultType _type;

        public:

        using inner_value_type = typename decltype(_inner)::value_type;

        constexpr Result(value_type value) noexcept: // NOLINT
                _inner(inner_value_type()),
                _type(ResultType::OK) {
            if constexpr ((_is_pointer || _is_reference) || !std::is_trivial_v<T>) {
                if constexpr (_is_reference) {
                    _inner._value = &value;
                }
                else {
                    _inner._value = value;
                }
            }
            else {
                new(&_inner._value) T(); // Initialize union field before copying
                _inner._value = std::move(value);
            }
        }

        constexpr Result(Error<E> error) noexcept: // NOLINT
                _inner(std::move(error.get_error())),
                _type(ResultType::ERROR) {
        }

        constexpr Result() noexcept:
                _inner(ResultInner<T, E>()),
                _type(ResultType::EMPTY) {
        }

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _type == ResultType::EMPTY;
        }

        [[nodiscard]] constexpr auto is_ok() const noexcept -> bool {
            if constexpr (_is_void) {
                return _type == ResultType::EMPTY;
            }
            else {
                return _type == ResultType::OK;
            }
        }

        [[nodiscard]] constexpr auto is_error() const noexcept -> bool {
            return _type == ResultType::ERROR;
        }

        [[nodiscard]] constexpr auto borrow_value() noexcept -> decltype(auto) {
            if constexpr (!_is_void) {
                if constexpr (_is_reference) {
                    return *_inner._value;
                }
                else {
                    return _inner._value;
                }
            }
        }

        [[nodiscard]] constexpr auto get_value() noexcept -> decltype(auto) {
            if constexpr (!_is_void) {
                _type = ResultType::EMPTY;

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
        }

        [[nodiscard]] constexpr auto borrow_error() noexcept -> E& {
            return _inner._error;
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> E {
            _type = ResultType::EMPTY;
            return std::move(_inner._error);
        }
    };
}