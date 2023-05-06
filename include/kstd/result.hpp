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
#include <cstring>
#include "types.hpp"
#include "kstd/concepts.hpp"
#include "large_string.hpp"

namespace kstd {
    template<typename E> //
    KSTD_REQUIRES(std::is_standard_layout<E>::value)
    class Error final {
        E _error;

        public:

        explicit Error(E error) noexcept :
                _error(std::move(error)) {
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> E& {
            return _error;
        }
    };

    namespace {
        enum class ResultType : u8 {
            OK, ERROR, EMPTY
        };

        template<typename T, typename E> //
        KSTD_REQUIRES(std::is_standard_layout<E>::value&& std::is_move_assignable<E>::value)
        union ResultInner {
            using value_type = typename std::conditional<std::is_void<T>::value, u8, typename std::conditional<std::is_reference<T>::value, typename std::remove_reference<T>::type*, T>::type>::type;

            value_type _value;
            E _error;

            ResultInner() noexcept :
                    _value() {
            }

            // @formatter:off
            ~ResultInner() noexcept {}
            // @formatter:on
        };
    }

    template<typename T, typename E = StringSlice> //
    KSTD_REQUIRES(std::is_standard_layout<E>::value&& std::is_move_assignable<E>::value)
    struct Result final {
        using self_type = Result<T, E>;
        using value_type = typename std::conditional<std::is_void<T>::value, u8, T>::type;
        using error_type = E;
        using naked_value_type = typename std::remove_all_extents<typename std::remove_reference<value_type>::type>::type;

        private:

        static constexpr bool _is_pointer = std::is_pointer<T>::value;
        static constexpr bool _is_reference = std::is_reference<T>::value;
        static constexpr bool _is_void = std::is_void<T>::value;

        ResultInner<T, E> _inner;
        ResultType _type;

        public:

        Result() noexcept :
                _inner(),
                _type(ResultType::EMPTY) {
        }

        Result(value_type value) noexcept : // NOLINT
                _inner(),
                _type(ResultType::OK) {
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

        Result(Error<E> error) noexcept : // NOLINT
                _inner(),
                _type(ResultType::ERROR) {
            _inner._error = std::move(error.get_error());
        }

        constexpr auto operator =(const self_type& other) noexcept -> self_type& {
            _type = other._type;

            if (other._type == ResultType::OK) {
                _inner._value = other._inner._value;
            }
            else if (other._type == ResultType::ERROR) {
                _inner._error = other._inner._error;
            }

            return *this;
        }

        constexpr auto operator =(self_type&& other) noexcept -> self_type& {
            _type = other._type;

            if (other._type == ResultType::OK) {
                if constexpr (_is_pointer || _is_reference) {
                    _inner._value = other._inner._value;
                }
                else {
                    _inner._value = std::move(other._inner._value);
                }
            }
            else if (other._type == ResultType::ERROR) {
                _inner._error = std::move(other._inner._error);
            }

            return *this;
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

        [[nodiscard]] constexpr auto borrow_value() noexcept -> typename std::conditional<_is_pointer, T, naked_value_type&>::type {
            #ifdef BUILD_DEBUG
            if (!_is_void && is_empty()) {
                throw std::runtime_error("Result has no value");
            }

            if (is_error()) {
                throw std::runtime_error("Result is an error");
            }
            #endif

            if constexpr (!_is_void) {
                if constexpr (_is_reference) {
                    return *_inner._value;
                }
                else {
                    return _inner._value;
                }
            }
        }

        [[nodiscard]] constexpr auto get_value() noexcept -> T {
            #ifdef BUILD_DEBUG
            if (!_is_void && is_empty()) {
                throw std::runtime_error("Result has no value");
            }

            if (is_error()) {
                throw std::runtime_error("Result is an error");
            }
            #endif

            _type = ResultType::EMPTY;

            if constexpr (!_is_void) {
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

        [[nodiscard]] constexpr auto get_error() noexcept -> E {
            #ifdef BUILD_DEBUG
            if (!_is_void && is_empty()) {
                throw std::runtime_error("Result has no value");
            }

            if (!is_error()) {
                throw std::runtime_error("Result is not an error");
            }
            #endif

            return _inner._error;
        }

        template<typename TT>
        [[nodiscard]] constexpr auto forward_error() const noexcept -> Result<TT, E> {
            #ifdef BUILD_DEBUG
            if (!_is_void && is_empty()) {
                throw std::runtime_error("Result has no value");
            }

            if (!is_error()) {
                throw std::runtime_error("Result is not an error");
            }
            #endif

            return Result<TT, E>(Error<E>(std::move(_inner._error)));
        }

        [[nodiscard]] constexpr operator bool() const noexcept { // NOLINT
            return !is_empty();
        }

        [[nodiscard]] constexpr auto operator ->() noexcept -> naked_value_type* {
            return &borrow_value();
        }

        [[nodiscard]] constexpr auto operator *() noexcept -> T {
            return get_value();
        }
    };

    template<typename T, typename E = StringSlice>
    [[nodiscard]] constexpr auto make_ok(T value) noexcept -> Result<T, E> {
        if constexpr (std::is_reference<T>::value || std::is_pointer<T>::value) {
            return Result<T, E>(value);
        }
        else {
            return Result<T, E>(std::move(value));
        }
    }

    template<typename T, typename E>
    [[nodiscard]] constexpr auto make_error(E error) noexcept -> Result<T, E> {
        return Result<T, E>(Error<E>(std::move(error)));
    }
}