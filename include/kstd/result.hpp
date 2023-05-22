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

#include "libc.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "string_slice.hpp"
#include "box.hpp"
#include "meta.hpp"

namespace kstd {
    template<typename E> //
    class Error final {
        E _error;

        public:

        explicit Error(E error) noexcept :
                _error(utils::move_or_copy(error)) {
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> E& {
            return _error;
        }
    };

    template<typename T, typename E = StringSlice> //
    struct Result;

    namespace {
        enum class ResultType : u8 {
            OK, ERROR, EMPTY
        };

        template<typename T, typename E> //
        union ResultInner {
            using ValueType = meta::substitute_void<T, u8>;

            private:

            friend class Result<T, E>;

            Box<ValueType> _value;
            E _error;

            ResultInner() noexcept :
                    _value() {
            }

            // @formatter:off
            ~ResultInner() noexcept {} // This is handled in Result itself
            // @formatter:on
        };
    }

    template<typename T, typename E> //
    struct Result final {
        static constexpr bool is_pointer = meta::is_ptr<T>;
        static constexpr bool is_reference = meta::is_ref<T>;
        static constexpr bool is_void = meta::is_void<T>;

        using Self = Result<T, E>;
        using ValueType = meta::conditional<is_void, u8, T>;
        using ErrorType = E;
        using NakedValueType = meta::naked_type<ValueType>;
        using InnerType = ResultInner<T, E>;
        using InnerValueType = typename InnerType::ValueType;
        using BorrowedValueType = meta::conditional<meta::is_ptr<ValueType>, NakedValueType*, NakedValueType&>;
        using ConstBorrowedValueType = meta::conditional<meta::is_ptr<ValueType>, const NakedValueType*, const NakedValueType&>;

        private:

        InnerType _inner;
        ResultType _type;

        public:

        constexpr Result() noexcept :
                _inner(),
                _type(ResultType::EMPTY) {
        }

        constexpr Result(ValueType value) noexcept : // NOLINT
                _inner(),
                _type(ResultType::OK) {
            _inner._value = value;
        }

        constexpr Result(Error<E> error) noexcept : // NOLINT
                _inner(),
                _type(ResultType::ERROR) {
            _inner._error = utils::move_or_copy(error.get_error());
        }

        constexpr Result(const Self& other) noexcept :
                _inner(),
                _type(other._type) {
            if (other.is_ok()) {
                _inner._value = other._inner._value;
            }
            else if (other.is_error()) {
                _inner._error = other._inner._error;
            }
        }

        constexpr Result(Self&& other) noexcept :
                _inner(),
                _type(other._type) {
            if (other.is_ok()) {
                _inner._value = move_or_copy(*other._inner._value);
            }
            else if (other.is_error()) {
                _inner._error = move_or_copy(*other._inner._error);
            }
        }

        ~Result() noexcept {
            drop();
        }

        constexpr auto drop() noexcept -> void {
            if constexpr (!is_void && meta::is_destructible<InnerValueType> && !is_pointer && !is_reference) {
                if (is_ok()) {
                    _inner._value.drop(); // Free currently held value
                    return;
                }
            }

            if (is_error()) {
                _inner._error.~ErrorType();
            }
        }

        constexpr auto operator =(const Self& other) noexcept -> Self& {
            if (this == &other) {
                return *this;
            }

            if (other.is_ok()) {
                drop();
                _inner._value = other._inner._value;
            }
            else if (other.is_error()) {
                drop();
                _inner._error = other._inner._error;
            }

            _type = other._type;
            return *this;
        }

        constexpr auto operator =(Self&& other) noexcept -> Self& {
            if (other.is_ok()) {
                drop();

                if constexpr (is_pointer || is_reference) {
                    _inner._value = other._inner._value;
                }
                else {
                    _inner._value = utils::move_or_copy(other._inner._value);
                }
            }
            else if (other.is_error()) {
                drop();
                _inner._error = utils::move_or_copy(other._inner._error);
            }

            _type = other._type;
            return *this;
        }

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _type == ResultType::EMPTY;
        }

        [[nodiscard]] constexpr auto is_ok() const noexcept -> bool {
            if constexpr (is_void) {
                return is_empty();
            }
            else {
                return _type == ResultType::OK;
            }
        }

        [[nodiscard]] constexpr auto is_error() const noexcept -> bool {
            return _type == ResultType::ERROR;
        }

        [[nodiscard]] constexpr auto borrow_value() noexcept -> BorrowedValueType {
            assert_true(!is_error());

            if constexpr (!is_void) {
                return _inner._value.borrow();
            }
        }

        [[nodiscard]] constexpr auto unwrap() noexcept -> decltype(auto) {
            assert_true(!is_error());
            _type = ResultType::EMPTY;

            if constexpr (!is_void) {
                return _inner._value.get();
            }
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> E {
            assert_true(is_error());
            return _inner._error;
        }

        template<typename TT>
        [[nodiscard]] constexpr auto forward_error() const noexcept -> Result<TT, E> {
            assert_true(is_error());
            return Result<TT, E>(Error<E>(move_or_copy(_inner._error)));
        }

        [[nodiscard]] constexpr operator bool() const noexcept { // NOLINT
            return !is_empty();
        }

        [[nodiscard]] constexpr auto operator ->() noexcept -> NakedValueType* {
            return &borrow_value();
        }

        [[nodiscard]] constexpr auto operator *() noexcept -> decltype(auto) {
            return borrow_value();
        }
    };

    template<typename T, typename E = StringSlice>
    [[nodiscard]] constexpr auto make_ok(T value) noexcept -> Result<T, E> {
        return Result<T, E>(utils::move_or_copy(value));
    }

    template<typename T, typename E>
    [[nodiscard]] constexpr auto make_error(E error) noexcept -> Result<T, E> {
        return Result<T, E>(Error<E>(utils::move_or_copy(error)));
    }
}