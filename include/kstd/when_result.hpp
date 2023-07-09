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
 * @since 09/07/2023
 */

#pragma once

#include "defaults.hpp"
#include "result.hpp"
#include "types.hpp"
#include <functional>
#include <type_traits>

namespace kstd {
    template<typename T, typename E, typename R>
    struct ResultWhenScope final {
        using ValueType = T;
        using ErrorType = E;
        using ReturnType = R;
        using BoxedReturnType = Box<ReturnType>;
        using Self = ResultWhenScope<ValueType, ErrorType, ReturnType>;

        private:
        Result<ValueType, ErrorType>* _address;
        BoxedReturnType _return_value;

        public:
        KSTD_NO_MOVE_COPY(ResultWhenScope, Self, constexpr)

        explicit constexpr ResultWhenScope(Result<ValueType, ErrorType>* address) noexcept :
                _address(address) {
        }

        ~ResultWhenScope() noexcept = default;

        template<typename F>
        [[nodiscard]] constexpr auto some(F&& function) noexcept -> Self& {
            if(_address->is_ok()) {
                if constexpr(std::is_void_v<ValueType>) {
                    _return_value = function();
                }
                else {
                    _return_value = function(_address->get());
                }
            }
            return *this;
        }

        template<typename F>
        [[nodiscard]] constexpr auto none(F&& function) noexcept -> Self& {
            if(_address->is_empty()) {
                _return_value = function();
            }
            return *this;
        }

        template<typename F>
        [[nodiscard]] constexpr auto error(F&& function) noexcept -> Self& {
            if(_address->is_error()) {
                _return_value = function(_address->get_error());
            }
            return *this;
        }

        // This allows the when-scope to be evaluated into an r-value reference
        [[nodiscard]] constexpr operator ReturnType&&() noexcept {// NOLINT
            return std::move(_return_value.get());
        }
    };

    template<typename T, typename E>
    struct ResultWhenScope<T, E, void> final {
        using ValueType = T;
        using ErrorType = E;
        using ReturnType = void;
        using Self = ResultWhenScope<ValueType, ErrorType, void>;

        private:
        Result<ValueType, ErrorType>* _address;

        public:
        KSTD_NO_MOVE_COPY(ResultWhenScope, Self, constexpr)

        explicit constexpr ResultWhenScope(Result<ValueType, ErrorType>* address) noexcept :
                _address(address) {
        }

        ~ResultWhenScope() noexcept = default;

        template<typename F>
        [[nodiscard]] constexpr auto some(F&& function) noexcept -> Self& {
            if(_address->is_ok()) {
                if constexpr(std::is_void_v<ValueType>) {
                    function();
                }
                else {
                    function(_address->get());
                }
            }
            return *this;
        }

        template<typename F>
        [[nodiscard]] constexpr auto none(F&& function) noexcept -> Self& {
            if(_address->is_empty()) {
                function();
            }
            return *this;
        }

        template<typename F>
        [[nodiscard]] constexpr auto error(F&& function) noexcept -> Self& {
            if(_address->is_error()) {
                function(_address->get_error());
            }
            return *this;
        }
    };

    template<typename R, typename T, typename E = std::string_view>
    [[nodiscard]] constexpr auto when(Result<T, E>& value) noexcept -> ResultWhenScope<T, E, R> {
        return ResultWhenScope<T, E, R>(&value);
    }
}// namespace kstd