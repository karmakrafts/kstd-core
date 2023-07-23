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
        // clang-format off
        using value_type    = E;
        using self          = Error<value_type>;
        // clang-format on

        private:
        value_type _error;

        public:
        KSTD_DEFAULT_MOVE_COPY(Error, self, constexpr)

        explicit constexpr Error(value_type error) noexcept :
                _error {std::forward<value_type>(error)} {
        }

        ~Error() noexcept = default;

        [[nodiscard]] constexpr auto get() noexcept -> value_type& {
            return _error;
        }

        [[nodiscard]] constexpr auto get() const noexcept -> const value_type& {
            return _error;
        }
    };

    template<typename T, typename E = std::string_view>
    struct Result final {
        static_assert(!std::is_same_v<std::remove_all_extents_t<T>, Void>, "Type cannot be Void");

        // clang-format off
        using value_type            = T;
        using error_type            = E;
        using wrapped_error_type    = Error<error_type>;
        using nv_value_type         = std::conditional_t<std::is_void_v<value_type>, u8, value_type>;
        using self                  = Result<value_type, error_type>;
        using boxed_value_type      = Box<nv_value_type>;
        using reference             = typename boxed_value_type::reference;
        using const_reference       = typename boxed_value_type::const_reference;
        using pointer               = typename boxed_value_type::pointer;
        using const_pointer         = typename boxed_value_type::const_pointer;
        // clang-format on

        private:
        std::variant<boxed_value_type, wrapped_error_type, Void> _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Result, self, constexpr)

        constexpr Result() noexcept :
                _value {Void()} {
        }

        constexpr Result(nv_value_type value) noexcept :// NOLINT
                _value {boxed_value_type(std::forward<nv_value_type>(value))} {
        }

        constexpr Result(wrapped_error_type error) noexcept :// NOLINT
                _value {std::move(error)} {
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
                return std::holds_alternative<boxed_value_type>(_value);
            }
        }

        [[nodiscard]] constexpr auto is_error() const noexcept -> bool {
            return std::holds_alternative<wrapped_error_type>(_value);
        }

        [[nodiscard]] constexpr auto get() noexcept -> reference {
            assert_true(is_ok());

            if constexpr(!std::is_void_v<T>) {
                return std::get<boxed_value_type>(_value).get();
            }
        }

        [[nodiscard]] constexpr auto get() const noexcept -> const_reference {
            assert_true(is_ok());

            if constexpr(!std::is_void_v<T>) {
                return std::get<boxed_value_type>(_value).get();
            }
        }

        [[nodiscard]] constexpr auto
        get_or(std::remove_reference_t<std::remove_cv_t<nv_value_type>> default_value) const noexcept
                -> std::remove_reference_t<std::remove_cv_t<nv_value_type>> {
            if(!is_ok()) {
                return default_value;
            }

            return get();
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> E& {
            assert_true(is_error());
            return std::get<wrapped_error_type>(_value).get();
        }

        [[nodiscard]] constexpr auto get_error() const noexcept -> const E& {
            assert_true(is_error());
            return std::get<wrapped_error_type>(_value).get();
        }

        template<typename TT>
        [[nodiscard]] constexpr auto forward() const noexcept -> Result<TT, E> {
            if(is_empty()) {
                return {};
            }
            assert_true(is_error());
            return std::get<wrapped_error_type>(_value);
        }

        template<typename F, typename R = std::invoke_result_t<F, reference>>
        [[nodiscard]] constexpr auto map(F&& function) const noexcept -> Result<R, E> {
            static_assert(std::is_convertible_v<F, std::function<R(nv_value_type)>>,
                          "Function signature does not match");
            if(is_ok()) {
                return function(get());
            }
            if(is_error()) {
                return forward<R>();
            }
            return {};
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return is_ok();
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> reference {
            return get();
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> const_reference {
            return get();
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> pointer {
            return &get();
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> const_pointer {
            return &get();
        }
    };

    template<typename E>
    struct Result<void, E> final {
        // clang-format off
        using value_type            = void;
        using error_type            = E;
        using wrapped_error_type    = Error<error_type>;
        using self                  = Result<value_type, error_type>;
        using reference             = value_type;
        using const_reference       = value_type;
        using pointer               = value_type;
        using const_pointer         = value_type;
        // clang-format on

        private:
        std::variant<wrapped_error_type, Void> _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Result, self, constexpr)

        constexpr Result() noexcept :
                _value {Void()} {
        }

        constexpr Result(wrapped_error_type error) noexcept :// NOLINT
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
            return std::holds_alternative<wrapped_error_type>(_value);
        }

        [[nodiscard]] constexpr auto get_error() noexcept -> E& {
            assert_true(is_error());
            return std::get<wrapped_error_type>(_value).get();
        }

        [[nodiscard]] constexpr auto get_error() const noexcept -> const E& {
            assert_true(is_error());
            return std::get<wrapped_error_type>(_value).get();
        }

        template<typename TT>
        [[nodiscard]] constexpr auto forward() const noexcept -> Result<TT, E> {
            if(is_empty()) {
                return {};
            }
            assert_true(is_error());
            return std::get<wrapped_error_type>(_value);
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return is_ok();
        }
    };

    template<typename T, typename E = std::string_view>
    Result(T) -> Result<T, E>;

#ifdef BUILD_DEBUG
    static_assert(std::is_same_v<typename Result<void>::value_type, void>);
    static_assert(std::is_same_v<typename Result<void>::wrapped_error_type, Error<std::string_view>>);
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