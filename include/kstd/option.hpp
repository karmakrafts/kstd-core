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

namespace kstd {
    template<typename T>
    struct Option {
        using ValueType = T;
        using Self [[maybe_unused]] = Option<ValueType>;
        using BoxedValueType = Box<ValueType>;
        using BorrowedValueType = typename BoxedValueType::BorrowedValueType;
        using ConstBorrowedValueType = typename BoxedValueType::ConstBorrowedValueType;
        using Pointer = typename BoxedValueType::Pointer;
        using ConstPointer = typename BoxedValueType::ConstPointer;

        private:
        BoxedValueType _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Option)

        constexpr Option() noexcept :
                _value() {
        }

        constexpr Option(ValueType value) noexcept :// NOLINT
                _value(utils::move_or_copy(value)) {
        }

        ~Option() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _value.is_empty();
        }

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool {
            return !_value.is_empty();
        }

        [[nodiscard]] constexpr auto borrow() noexcept -> BorrowedValueType {
            return _value.borrow();
        }

        [[nodiscard]] constexpr auto borrow() const noexcept -> ConstBorrowedValueType {
            return _value.borrow();
        }

        [[nodiscard]] constexpr auto get() noexcept -> decltype(auto) {
            return _value.get();
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return has_value();
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> BorrowedValueType {
            return borrow();
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> ConstBorrowedValueType {
            return borrow();
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> Pointer {
            return &borrow();
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> ConstPointer {
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