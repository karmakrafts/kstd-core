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
#include <cstring>
#include "types.hpp"

namespace kstd {
    template<typename E> //
    requires std::is_standard_layout_v<E>
    class Error final {
        E _error;

    public:

        explicit constexpr Error(E error) noexcept:
                _error(std::move(error)) {
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> E & {
            return _error;
        }
    };

    namespace {
        enum class ResultType : u8 {
            OK,
            ERROR,
            EMPTY
        };

        template<typename T, typename E> //
        requires std::is_standard_layout_v<E> && std::is_move_assignable_v<E>
        union ResultInner {
            using value_type = std::conditional_t<std::is_void_v<T>, u8, std::conditional_t<std::is_reference_v<T>, std::remove_reference_t<T> *, T>>;

            value_type _value;
            E _error;

            constexpr ResultInner() noexcept:
                    _value() {
            }

            // @formatter:off
            constexpr ~ResultInner() noexcept {}
            // @formatter:on
        };
    }

    template<typename T, typename E = std::string_view> //
    requires std::is_standard_layout_v<E> && std::is_move_assignable_v<E>
    struct Result final {
        using self_type = Result<T, E>;
        using value_type = std::conditional_t<std::is_void_v<T>, u8, T>;
        using naked_value_type = std::remove_all_extents_t<std::remove_reference_t<value_type>>;

    private:

        static constexpr bool _is_pointer = std::is_pointer_v<T>;
        static constexpr bool _is_reference = std::is_reference_v<T>;
        static constexpr bool _is_void = std::is_void_v<T>;

        ResultInner<T, E> _inner;
        ResultType _type;

    public:

        constexpr Result() noexcept:
                _inner(),
                _type(ResultType::EMPTY) {
        }

        constexpr Result(value_type value) noexcept: // NOLINT
                _inner(),
                _type(ResultType::OK) {
            if constexpr ((_is_pointer || _is_reference) || !std::is_trivial_v<T>) {
                if constexpr (_is_reference) {
                    _inner._value = &value;
                } else {
                    _inner._value = value;
                }
            } else {
                if constexpr (std::is_move_assignable_v<T>) {
                    _inner._value = std::move(value);
                } else {
                    _inner._value = value;
                }
            }
        }

        constexpr Result(Error<E> error) noexcept: // NOLINT
                _inner(),
                _type(ResultType::ERROR) {
            _inner._error = std::move(error.get_error());
        }

        constexpr auto operator=(const self_type &other) noexcept -> self_type & {
            _type = other._type;

            if (other._type == ResultType::OK) {
                _inner._value = other._inner._value;
            } else if (other._type == ResultType::ERROR) {
                _inner._error = other._inner._error;
            }

            return *this;
        }

        constexpr auto operator=(self_type &&other) noexcept -> self_type & {
            _type = other._type;

            if (other._type == ResultType::OK) {
                if constexpr (_is_pointer || _is_reference) {
                    _inner._value = other._inner._value;
                } else {
                    _inner._value = std::move(other._inner._value);
                }
            } else if (other._type == ResultType::ERROR) {
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
            } else {
                return _type == ResultType::OK;
            }
        }

        [[nodiscard]] constexpr auto is_error() const noexcept -> bool {
            return _type == ResultType::ERROR;
        }

        [[nodiscard]] constexpr auto borrow_value() noexcept -> std::conditional_t<_is_pointer, T, naked_value_type &> {
#ifdef BUILD_DEBUG
            if (is_empty()) {
                throw std::runtime_error("Result has no value");
            }

            if (is_error()) {
                throw std::runtime_error("Result is an error");
            }
#endif

            if constexpr (!_is_void) {
                if constexpr (_is_reference) {
                    return *_inner._value;
                } else {
                    return _inner._value;
                }
            }
        }

        [[nodiscard]] constexpr auto get_value() noexcept -> T {
#ifdef BUILD_DEBUG
            if (is_empty()) {
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
                } else if constexpr (_is_pointer) {
                    return _inner._value;
                } else {
                    return std::move(_inner._value);
                }
            }
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> E {
#ifdef BUILD_DEBUG
            if (is_empty()) {
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
            if (is_empty()) {
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

        [[nodiscard]] constexpr auto operator->() noexcept -> naked_value_type * {
            return &borrow_value();
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> T {
            return get_value();
        }
    };
}