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
#include <stdexcept>
#include <type_traits>

#include "assert.hpp"
#include "box.hpp"
#include "defaults.hpp"
#include "libc.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "void.hpp"

#ifndef KSTD_DISABLE_STD_EXPECTED_SUPPORT
#include "language.hpp"
#ifdef KSTD_CPP_23
#define KSTD_STD_EXPECTED_SUPPORT
#include <expected>
#endif// KSTD_CPP_23
#endif// KSTD_DISABLE_STD_EXPECTED_SUPPORT

namespace kstd {
    template<typename E>
    struct Error final {
        using ValueType = E;
        using Self = Error<ValueType>;

        private:
        ValueType _error;

        public:
        KSTD_DEFAULT_MOVE_COPY(Error, Self, constexpr)

        explicit constexpr Error(ValueType error) noexcept :
                _error {std::forward<ValueType>(error)} {
        }

        ~Error() noexcept = default;

        [[nodiscard]] constexpr auto get() noexcept -> ValueType& {
            return _error;
        }

        [[nodiscard]] constexpr auto get() const noexcept -> const ValueType& {
            return _error;
        }
    };

    template<typename T, typename E = std::string>
    struct Result final {
        static_assert(!std::is_same_v<std::remove_all_extents_t<T>, Void>, "Type cannot be Void");

        using ValueType = T;
        using ErrorType = E;
        using WrappedErrorType = Error<ErrorType>;
        using Self = Result<ValueType, ErrorType>;
        using BoxType = Box<ValueType>;
        using Reference = typename BoxType::Reference;
        using ConstReference = typename BoxType::ConstReference;
        using Pointer = typename BoxType::Pointer;
        using ConstPointer = typename BoxType::ConstPointer;

        private:
        std::variant<BoxType, WrappedErrorType, Void> _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Result, Self, constexpr)

        constexpr Result() noexcept :
                _value {Void()} {
        }

        constexpr Result(ValueType value) noexcept :// NOLINT
                _value {BoxType(std::forward<ValueType>(value))} {
        }

        constexpr Result(WrappedErrorType error) noexcept :// NOLINT
                _value {std::move(error)} {
        }

        ~Result() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return std::holds_alternative<Void>(_value);
        }

        [[nodiscard]] constexpr auto is_ok() const noexcept -> bool {
            return std::holds_alternative<BoxType>(_value);
        }

        [[nodiscard]] constexpr auto is_error() const noexcept -> bool {
            return std::holds_alternative<WrappedErrorType>(_value);
        }

        [[nodiscard]] constexpr auto get() noexcept -> Reference {
            assert_true(is_ok());
            return std::get<BoxType>(_value).get();
        }

        [[nodiscard]] constexpr auto get() const noexcept -> ConstReference {
            assert_true(is_ok());
            return std::get<BoxType>(_value).get();
        }

        [[nodiscard]] constexpr auto
        get_or(std::remove_reference_t<std::remove_cv_t<ValueType>> default_value) const noexcept
                -> std::remove_reference_t<std::remove_cv_t<ValueType>> {
            if(!is_ok()) {
                return default_value;
            }

            return get();
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> ErrorType& {
            assert_true(is_error());
            return std::get<WrappedErrorType>(_value).get();
        }

        [[nodiscard]] constexpr auto get_error() const noexcept -> const ErrorType& {
            assert_true(is_error());
            return std::get<WrappedErrorType>(_value).get();
        }

        template<typename TT>
        [[nodiscard]] constexpr auto forward() const noexcept -> Result<TT, ErrorType> {
            if(is_empty()) {
                return {};
            }
            assert_true(is_error());
            return {std::move(std::get<WrappedErrorType>(_value))};
        }

        template<typename F, typename R = std::invoke_result_t<F, Reference>>
        [[nodiscard]] constexpr auto map(F&& function) const noexcept -> Result<R, ErrorType> {
            static_assert(std::is_convertible_v<F, std::function<R(ValueType)>>, "Function signature does not match");
            if(is_ok()) {
                return std::forward<F>(function)(get());
            }
            if(is_error()) {
                return forward<R>();
            }
            return {};
        }

        inline auto throw_if_error() const -> void {
            if(is_error()) {
                if constexpr(std::is_same_v<ErrorType, std::string>) {
                    throw std::runtime_error {get_error()};
                }
                else if constexpr(std::is_same_v<ErrorType, std::string_view>) {
                    throw std::runtime_error {std::string {get_error()}};
                }
                else {
                    throw std::runtime_error {std::to_string(get_error())};
                }
            }
        }

        [[nodiscard]] inline auto get_or_throw() -> Reference {
            throw_if_error();
            return std::get<BoxType>(_value).get();
        }

        [[nodiscard]] inline auto get_or_throw() const -> ConstReference {
            throw_if_error();
            return std::get<BoxType>(_value).get();
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return is_ok();
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> Reference {
            return get();
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> ConstReference {
            return get();
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> Pointer {
            return &get();
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> ConstPointer {
            return &get();
        }

#ifdef KSTD_STD_EXPECTED_SUPPORT
        [[nodiscard]] constexpr auto clone_into() noexcept -> std::expected<ValueType, ErrorType> {
            if(is_empty()) {
                return {std::unexpected {ErrorType {}}};// Empty default error value
            }
            if(is_error()) {
                return {std::unexpected {get_error()}};
            }
            return {get()};
        }

        [[nodiscard]] constexpr auto into() noexcept -> std::expected<ValueType, ErrorType> {
            if(is_empty()) {
                return {std::unexpected {ErrorType {}}};// Empty default error value
            }
            if(is_error()) {
                return {std::unexpected {get_error()}};
            }
            if constexpr(std::is_reference_v<ValueType>) {
                return {get()};
            }
            else {
                return {std::move(get())};
            }
        }
#endif// KSTD_STD_EXPECTED_SUPPORT
    };

    template<typename E>
    struct Result<void, E> final {
        using ValueType = void;
        using ErrorType = E;
        using WrappedErrorType = Error<ErrorType>;
        using Self = Result<ValueType, ErrorType>;
        using Reference = ValueType;
        using ConstReference = ValueType;
        using Pointer = ValueType;
        using ConstPointer = ValueType;

        private:
        std::variant<WrappedErrorType, Void> _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Result, Self, constexpr)

        constexpr Result() noexcept :
                _value {Void()} {
        }

        constexpr Result(WrappedErrorType error) noexcept :// NOLINT
                _value {std::move(error)} {
        }

        ~Result() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return std::holds_alternative<Void>(_value);
        }

        [[nodiscard]] constexpr auto is_ok() const noexcept -> bool {
            return is_empty();
        }

        [[nodiscard]] constexpr auto is_error() const noexcept -> bool {
            return std::holds_alternative<WrappedErrorType>(_value);
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> E& {
            assert_true(is_error());
            return std::get<WrappedErrorType>(_value).get();
        }

        [[nodiscard]] constexpr auto get_error() const noexcept -> const E& {
            assert_true(is_error());
            return std::get<WrappedErrorType>(_value).get();
        }

        template<typename TT>
        [[nodiscard]] constexpr auto forward() const noexcept -> Result<TT, E> {
            if(is_empty()) {
                return {};
            }
            assert_true(is_error());
            return {std::move(std::get<WrappedErrorType>(_value))};
        }

        inline auto throw_if_error() const -> void {
            if(is_error()) {
                if constexpr(std::is_same_v<ErrorType, std::string>) {
                    throw std::runtime_error {get_error()};
                }
                else if constexpr(std::is_same_v<ErrorType, std::string_view>) {
                    throw std::runtime_error {std::string {get_error()}};
                }
                else {
                    throw std::runtime_error {std::to_string(get_error())};
                }
            }
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return is_ok();
        }
    };

    template<typename T, typename E = std::string>
    Result(T) -> Result<T, E>;

#ifdef BUILD_DEBUG
    static_assert(std::is_same_v<typename Result<void>::ValueType, void>);
    static_assert(std::is_same_v<typename Result<void>::WrappedErrorType, Error<std::string>>);
#endif

    template<typename F, typename R = std::invoke_result_t<F>>
    [[nodiscard]] constexpr auto try_to(F&& function) noexcept -> Result<R> {
        static_assert(std::is_convertible_v<F, std::function<R()>>, "Function return type does not match");
        try {
            if constexpr(std::is_void_v<R>) {
                std::forward<F>(function)();
            }
            else {
                return std::forward<F>(function)();
            }
        }
        catch(const std::exception& error) {
            return Error {std::string {error.what()}};
        }
    }

#ifdef KSTD_STD_EXPECTED_SUPPORT
    template<typename T, typename E>
    [[nodiscard]] constexpr auto clone_into(const std::expected<T, E>& value) noexcept -> Result<T, E> {
        if(!value) {
            return Error {value.error()};
        }
        return *value;
    }

    template<typename T, typename E>
    [[nodiscard]] constexpr auto into(std::expected<T, E>& value) noexcept -> Result<T, E> {
        if(!value) {
            return Error {value.error()};
        }
        return {std::move(*value)};
    }
#endif// KSTD_STD_EXPECTED_SUPPORT
}// namespace kstd