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

#include <functional>
#include <type_traits>

#include "assert.hpp"
#include "box.hpp"
#include "defaults.hpp"
#include "libc.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "void.hpp"

namespace kstd {
    template<typename E>
    struct Error final {
        using ErrorType = E;
        using Self = Error<ErrorType>;

        private:
        ErrorType _error;

        public:
        KSTD_DEFAULT_MOVE_COPY(Error, Self, constexpr)

        explicit constexpr Error(ErrorType error) noexcept :
                _error(std::forward<ErrorType>(error)) {
        }

        ~Error() noexcept = default;

        [[nodiscard]] constexpr auto get() noexcept -> ErrorType& {
            return _error;
        }

        [[nodiscard]] constexpr auto get() const noexcept -> const ErrorType& {
            return _error;
        }
    };

    template<typename T, typename E = std::string_view>
    struct Result final {
        static_assert(!std::is_same_v<std::remove_all_extents_t<T>, Void>, "Type cannot be Void");

        using Self = Result<T, E>;
        using ValueType = std::conditional_t<std::is_void_v<T>, u8, T>;
        using ErrorType = Error<E>;
        using BoxedValueType = Box<ValueType>;
        using BorrowedValueType = typename BoxedValueType::BorrowedValueType;
        using ConstBorrowedValueType = typename BoxedValueType::ConstBorrowedValueType;
        using Pointer = typename BoxedValueType::Pointer;
        using ConstPointer = typename BoxedValueType::ConstBorrowedValueType;

        private:
        std::variant<BoxedValueType, ErrorType, Void> _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Result, Self, constexpr)

        constexpr Result() noexcept :
                _value(Void()) {
        }

        constexpr Result(ValueType value) noexcept :// NOLINT
                _value(BoxedValueType(std::forward<ValueType>(value))) {
        }

        constexpr Result(ErrorType error) noexcept :// NOLINT
                _value(std::move(error)) {
        }

        ~Result() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return std::holds_alternative<Void>(_value);
        }

        [[nodiscard]] constexpr auto is_ok() const noexcept -> bool {
            if constexpr(std::is_void_v<T>) {
                return is_empty();
            }
            else {
                return std::holds_alternative<BoxedValueType>(_value);
            }
        }

        [[nodiscard]] constexpr auto is_error() const noexcept -> bool {
            return std::holds_alternative<ErrorType>(_value);
        }

        [[nodiscard]] constexpr auto get() noexcept -> BorrowedValueType {
            assert_true(is_ok());

            if constexpr(!std::is_void_v<T>) {
                return std::get<BoxedValueType>(_value).get();
            }
        }

        [[nodiscard]] constexpr auto get() const noexcept -> ConstBorrowedValueType {
            assert_true(is_ok());

            if constexpr(!std::is_void_v<T>) {
                return std::get<BoxedValueType>(_value).get();
            }
        }

        [[nodiscard]] constexpr auto get_or(ValueType default_value) const noexcept -> ValueType {
            if(!is_ok()) {
                return default_value;
            }

            return get();
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> E& {
            assert_true(is_error());
            return std::get<ErrorType>(_value).get();
        }

        [[nodiscard]] constexpr auto get_error() const noexcept -> const E& {
            assert_true(is_error());
            return std::get<ErrorType>(_value).get();
        }

        template<typename TT>
        [[nodiscard]] constexpr auto forward_error() const noexcept -> Result<TT, E> {
            assert_true(is_error());
            return {std::get<ErrorType>(_value)};
        }

        template<typename R, typename F>
        [[nodiscard]] constexpr auto map(F&& function) const noexcept -> Result<R, E> {
            static_assert(std::is_convertible_v<F, std::function<R(ValueType)>>, "Function signature does not match");
            if(is_ok()) {
                return function(get());
            }
            if(is_error()) {
                return forward_error<R>();
            }
            return {};
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return is_ok();
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> BorrowedValueType {
            return get();
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> ConstBorrowedValueType {
            return get();
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> Pointer {
            return &get();
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> ConstPointer {
            return &get();
        }
    };

    template<typename E>
    struct Result<void, E> final {
        using Self = Result<void, E>;
        using ValueType = void;
        using ErrorType = Error<E>;

        private:
        std::variant<ErrorType, Void> _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Result, Self, constexpr)

        constexpr Result() noexcept :
                _value(Void()) {
        }

        constexpr Result(ErrorType error) noexcept :// NOLINT
                _value(std::move(error)) {
        }

        ~Result() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return std::holds_alternative<Void>(_value);
        }

        [[nodiscard]] constexpr auto is_ok() const noexcept -> bool {
            return is_empty();
        }

        [[nodiscard]] constexpr auto is_error() const noexcept -> bool {
            return std::holds_alternative<ErrorType>(_value);
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> E& {
            assert_true(is_error());
            return std::get<ErrorType>(_value).get();
        }

        [[nodiscard]] constexpr auto get_error() const noexcept -> const E& {
            assert_true(is_error());
            return std::get<ErrorType>(_value).get();
        }

        template<typename TT>
        [[nodiscard]] constexpr auto forward_error() const noexcept -> Result<TT, E> {
            assert_true(is_error());
            return {std::get<ErrorType>(_value)};
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return is_ok();
        }
    };

    template<typename T, typename E = std::string_view>
    Result(T) -> Result<T, E>;

#ifdef BUILD_DEBUG
    static_assert(std::is_same_v<typename Result<void>::ValueType, void>);
    static_assert(std::is_same_v<typename Result<void>::ErrorType, Error<std::string_view>>);
#endif

    template<typename R, typename F>
    [[nodiscard]] constexpr auto try_to(F&& function) noexcept -> Result<R> {
        static_assert(std::is_convertible_v<F, std::function<R()>>, "Function return type does not match");
        try {
            if constexpr(std::is_void_v<R>) {
                function();
            }
            else {
                return function();
            }
        }
        catch(const std::exception& error) {
            return Error(std::string_view(error.what()));
        }
    }
}// namespace kstd