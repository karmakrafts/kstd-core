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

namespace kstd {
    template<typename E>
    class Error final {
        E _error;

        public:
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
    struct Result;

    namespace {
        enum class ResultType : u8 {
            OK,
            ERROR,
            EMPTY
        };
    }// namespace

    template<typename T, typename E>
    struct Result final {
        static constexpr bool is_pointer = meta::is_ptr<T>;
        static constexpr bool is_reference = meta::is_ref<T>;
        static constexpr bool is_void = meta::is_void<T>;

        using Self [[maybe_unused]] = Result<T, E>;
        using ValueType = meta::If<is_void, u8, T>;
        using BoxedValueType = Box<ValueType>;
        using ErrorType [[maybe_unused]] = E;
        using InnerType = std::variant<BoxedValueType, ErrorType>;

        private:
        InnerType _inner;
        ResultType _type;

        public:
        KSTD_DEFAULT_MOVE_COPY(Result)

        constexpr Result() noexcept :
                _inner(),
                _type(ResultType::EMPTY) {
        }

        constexpr Result(ValueType value) noexcept :// NOLINT
                _inner(BoxedValueType(value)),
                _type(ResultType::OK) {
        }

        constexpr Result(Error<ErrorType> error) noexcept :// NOLINT
                _inner(utils::move_or_copy(error.get_error())),
                _type(ResultType::ERROR) {
        }

        ~Result() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _type == ResultType::EMPTY;
        }

        [[nodiscard]] constexpr auto is_ok() const noexcept -> bool {
            if constexpr(is_void) {
                return is_empty();
            }
            else {
                return _type == ResultType::OK;
            }
        }

        [[nodiscard]] constexpr auto is_error() const noexcept -> bool {
            return _type == ResultType::ERROR;
        }

        [[nodiscard]] constexpr auto borrow() noexcept -> decltype(auto) {
            assert_true(!is_error());

            if constexpr(!is_void) {
                return std::get<BoxedValueType>(_inner).borrow();
            }
        }

        [[nodiscard]] constexpr auto borrow() const noexcept -> decltype(auto) {
            assert_true(!is_error());

            if constexpr(!is_void) {
                return std::get<BoxedValueType>(_inner).borrow();
            }
        }

        [[nodiscard]] constexpr auto get() noexcept -> decltype(auto) {
            assert_true(!is_error());
            _type = ResultType::EMPTY;

            if constexpr(!is_void) {
                return std::get<BoxedValueType>(_inner).get();
            }
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> decltype(auto) {
            assert_true(is_error());
            return std::get<ErrorType>(_inner);
        }

        template<typename TT>
        [[nodiscard]] constexpr auto forward_error() const noexcept -> Result<TT, ErrorType> {
            assert_true(is_error());
            return Result<TT, ErrorType>(Error<ErrorType>(utils::move_or_copy(std::get<ErrorType>(_inner))));
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return !is_empty();
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> decltype(auto) {
            return &borrow();
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> decltype(auto) {
            return &borrow();
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> decltype(auto) {
            return borrow();
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> decltype(auto) {
            return borrow();
        }
    };

    template<typename T, typename E = std::string_view>
    [[nodiscard]] constexpr auto make_ok(T value) noexcept -> Result<T, E> {
        return Result<T, E>(utils::move_or_copy(value));
    }

    template<typename T, typename E = std::string_view>
    [[nodiscard]] constexpr auto make_error(E error) noexcept -> Result<T, E> {
        return Result<T, E>(Error<E>(utils::move_or_copy(error)));
    }
}// namespace kstd