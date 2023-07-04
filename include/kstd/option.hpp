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

#include "assert.hpp"
#include "box.hpp"
#include "defaults.hpp"
#include "libc.hpp"
#include "meta.hpp"
#include "non_zero.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "void.hpp"

namespace kstd {
    template<typename T>
    struct Option {
        static constexpr bool is_pointer = meta::is_ptr<T>;
        static constexpr bool is_reference = meta::is_ref<T>;

        using ValueType = T;
        using Self [[maybe_unused]] = Option<ValueType>;
        using BoxedValueType = Box<ValueType>;

        private:
        std::variant<BoxedValueType, Void> _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Option)

        constexpr Option() noexcept :
                _value(Void()) {
        }

        constexpr Option(ValueType value) noexcept :// NOLINT
                _value(utils::move_or_copy(value)) {
        }

        ~Option() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return std::holds_alternative<Void>(_value);
        }

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool {
            return std::holds_alternative<BoxedValueType>(_value);
        }

        [[nodiscard]] constexpr auto borrow() noexcept -> decltype(auto) {
            assert_false(is_empty());
            return std::get<BoxedValueType>(_value).borrow();
        }

        [[nodiscard]] constexpr auto borrow() const noexcept -> decltype(auto) {
            assert_false(is_empty());
            return std::get<BoxedValueType>(_value).borrow();
        }

        [[nodiscard]] constexpr auto get() noexcept -> decltype(auto) {
            assert_false(is_empty());
            auto value = std::get<BoxedValueType>(_value).get();
            _value = Void();
            return value;
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return has_value();
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> decltype(auto) {
            return get();
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> decltype(auto) {
            return get();
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> decltype(auto) {
            return &borrow();
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> decltype(auto) {
            return &borrow();
        }
    };

    template<typename T>
    [[nodiscard]] constexpr auto make_empty() noexcept -> Option<T> {
        return Option<T>();
    }

    template<typename T>
    [[nodiscard]] constexpr auto make_value(T value) noexcept -> Option<T> {
        return Option<T>(utils::move_or_copy(value));
    }
}// namespace kstd