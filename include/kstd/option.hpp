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
        static constexpr bool is_pointer = meta::is_ptr<T>;
        static constexpr bool is_reference = meta::is_ref<T>;

        using ValueType = T;
        using Self [[maybe_unused]] = Option<ValueType>;
        using BoxType = Box<ValueType>;
        using BorrowedValueType = typename BoxType::BorrowedValueType;
        using ConstBorrowedValueType = typename BoxType::ConstBorrowedValueType;
        using Pointer = typename BoxType::Pointer;
        using ConstPointer = typename BoxType::ConstPointer;

        private:
        BoxType _inner;
        bool _is_present;

        public:
        KSTD_DEFAULT_MOVE_COPY(Option)

        constexpr Option() noexcept :
                _inner(),
                _is_present(false) {
        }

        constexpr Option(ValueType value) noexcept :// NOLINT
                _inner(utils::move_or_copy(value)),
                _is_present(true) {
        }

        ~Option() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return !_is_present;
        }

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool {
            return _is_present;
        }

        [[nodiscard]] constexpr auto borrow() noexcept -> BorrowedValueType {
            assert_false(is_empty());
            return _inner.borrow();
        }

        [[nodiscard]] constexpr auto borrow() const noexcept -> ConstBorrowedValueType {
            assert_false(is_empty());
            return _inner.borrow();
        }

        [[nodiscard]] constexpr auto get() noexcept -> decltype(auto) {
            assert_false(is_empty());
            return _inner.get();
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