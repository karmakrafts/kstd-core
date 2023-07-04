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

#include "assert.hpp"
#include "box.hpp"
#include "defaults.hpp"
#include "libc.hpp"
#include "meta.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "void.hpp"

namespace kstd {
    template<typename E>
    class Error final {
        E _error;

        public:
        using ErrorType [[maybe_unused]] = E;

        KSTD_DEFAULT_MOVE_COPY(Error)

        explicit Error(E error) noexcept :
                _error(utils::move_or_copy(error)) {
        }

        ~Error() noexcept = default;

        [[nodiscard]] constexpr auto get_error() noexcept -> E& {
            return _error;
        }
    };

    template<typename T, typename E = std::string_view>
    struct Result final {
        using Self [[maybe_unused]] = Result<T, E>;
        using ValueType = meta::If<meta::is_void<T>, u8, T>;
        using BoxedValueType = Box<ValueType>;

        private:
        std::variant<BoxedValueType, E, Void> _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Result)

        constexpr Result() noexcept :
                _value(Void()) {
        }

        explicit constexpr Result(ValueType value) noexcept :// NOLINT
                _value(BoxedValueType(utils::move_or_copy(value))) {
        }

        explicit constexpr Result(Error<E> error) noexcept :// NOLINT
                _value(utils::move(error.get_error())) {
        }

        ~Result() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return std::holds_alternative<Void>(_value);
        }

        [[nodiscard]] constexpr auto is_ok() const noexcept -> bool {
            if constexpr(meta::is_void<T>) {
                return is_empty();
            }
            else {
                return std::holds_alternative<BoxedValueType>(_value);
            }
        }

        [[nodiscard]] constexpr auto is_error() const noexcept -> bool {
            return std::holds_alternative<E>(_value);
        }

        [[nodiscard]] constexpr auto borrow() noexcept -> decltype(auto) {
            assert_true(is_ok());

            if constexpr(!meta::is_void<T>) {
                return std::get<BoxedValueType>(_value).borrow();
            }
        }

        [[nodiscard]] constexpr auto borrow() const noexcept -> decltype(auto) {
            assert_true(is_ok());

            if constexpr(!meta::is_void<T>) {
                return std::get<BoxedValueType>(_value).borrow();
            }
        }

        [[nodiscard]] constexpr auto unwrap() noexcept -> decltype(auto) {
            assert_true(is_ok());

            if constexpr(!meta::is_void<T>) {
                auto value = std::get<BoxedValueType>(_value).get();
                _value = Void();
                return value;
            }
            else {
                _value = Void();
            }
        }

        [[nodiscard]] constexpr auto unwrap_or(ValueType default_value) noexcept -> ValueType {
            if(!is_ok()) {
                return default_value;
            }

            auto value = std::get<BoxedValueType>(_value).get();
            _value = Void();
            return value;
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> decltype(auto) {
            assert_true(is_error());
            return std::get<E>(_value);
        }

        template<typename TT>
        [[nodiscard]] constexpr auto forward_error() const noexcept -> Result<TT, E> {
            assert_true(is_error());
            return Result<TT, E>(Error<E>(utils::move(std::get<E>(_value))));
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return is_ok();
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> decltype(auto) {
            return &borrow();
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> decltype(auto) {
            return &borrow();
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> decltype(auto) {
            return unwrap();
        }
    };

    template<typename T, typename E = std::string_view>
    [[nodiscard]] constexpr auto make_ok(T value) noexcept -> Result<T, E> {
        return Result<T, E>(utils::move_or_copy(value));
    }

    template<typename T, typename E = std::string_view>
    [[nodiscard]] constexpr auto make_error(E error) noexcept -> Result<T, E> {
        return Result<T, E>(Error<E>(utils::move(error)));
    }
}// namespace kstd