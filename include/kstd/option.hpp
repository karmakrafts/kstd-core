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
 * @since 06/05/2023
 */

#pragma once

#include <functional>
#include <type_traits>

#include "assert.hpp"
#include "box.hpp"
#include "defaults.hpp"
#include "libc.hpp"
#include "non_zero.hpp"
#include "types.hpp"
#include "utils.hpp"

#ifndef KSTD_DISABLE_STD_OPTIONAL_SUPPORT
#define KSTD_STD_OPTIONAL_SUPPORT
#include <optional>
#endif// KSTD_DISABLE_STD_OPTIONAL_SUPPORT

namespace kstd {
    template<typename T>
    struct Option final {
        static_assert(!std::is_same_v<std::decay_t<T>, Void>, "Type cannot be Void");

        using ValueType = T;
        using Self = Option<ValueType>;
        using BoxType = Box<std::remove_const_t<ValueType>>;
        using Reference = typename BoxType::Reference;
        using ConstReference = typename BoxType::ConstReference;
        using Pointer = typename BoxType::Pointer;
        using ConstPointer = typename BoxType::ConstPointer;

        friend struct std::hash<Self>;

        private:
        BoxType _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Option, Self, constexpr)

        constexpr Option() noexcept :
                _value {} {
        }

        constexpr Option(ValueType value) noexcept :
                _value {std::forward<ValueType>(value)} {
        }

        ~Option() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _value.is_empty();
        }

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool {
            return !_value.is_empty();
        }

        [[nodiscard]] constexpr auto get() noexcept -> Reference {
            return _value.get();
        }

        [[nodiscard]] constexpr auto get() const noexcept -> ConstReference {
            return _value.get();
        }

        [[nodiscard]] constexpr auto get_or(ValueType default_value) noexcept -> ValueType {
            if(has_value()) {
                return get();
            }
            return default_value;
        }

        template<typename F, typename R = std::invoke_result_t<F, Reference>>
        [[nodiscard]] constexpr auto map(F&& function) const noexcept -> Option<R> {
            static_assert(std::is_invocable_r_v<R, F, Reference>, "Function signature does not match");
            if(is_empty()) {
                return {};
            }
            return function(get());
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return has_value();
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

#ifdef KSTD_STD_OPTIONAL_SUPPORT
        [[nodiscard]] constexpr auto clone_into_optional() noexcept
                -> std::optional<std::remove_reference_t<ValueType>> {
            if(is_empty()) {
                return std::nullopt;
            }
            return std::make_optional<std::remove_reference_t<ValueType>>(get());
        }

        [[nodiscard]] constexpr auto into_optional() noexcept -> std::optional<std::remove_reference_t<ValueType>> {
            if(is_empty()) {
                return std::nullopt;
            }
            if constexpr(std::is_reference_v<ValueType>) {
                return std::make_optional<std::remove_reference_t<ValueType>>(get());
            }
            else {
                return std::make_optional<ValueType>(std::move(get()));
            }
        }
#endif// KSTD_STD_OPTIONAL_SUPPORT
    };

#ifdef KSTD_STD_OPTIONAL_SUPPORT
    template<typename T>
    [[nodiscard]] constexpr auto clone_into_option(const std::optional<T>& value) noexcept -> Option<T> {
        if(!value) {
            return {};
        }
        return *value;
    }

    template<typename T>
    [[nodiscard]] constexpr auto into_option(std::optional<T>& value) noexcept -> Option<T> {
        if(!value) {
            return {};
        }
        return {std::move(*value)};
    }
#endif// KSTD_STD_OPTIONAL_SUPPORT
}// namespace kstd

KSTD_DEFAULT_HASH_T(KSTD_TEMPLATE((typename T)), (kstd::Option<T>), value._value)