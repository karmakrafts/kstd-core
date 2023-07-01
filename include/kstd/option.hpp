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
    namespace {
        template<typename T>
        struct OptionInner {
            using ValueType = T;
            using Self = OptionInner<ValueType>;
            using BoxType = Box<ValueType>;
            using BorrowedValueType = typename BoxType::BorrowedValueType;
            using ConstBorrowedValueType = typename BoxType::ConstBorrowedValueType;
            using Pointer = typename BoxType::Pointer;
            using ConstPointer = typename BoxType::ConstPointer;

            BoxType _value;
            bool _is_present;

            KSTD_DEFAULT_MOVE_COPY(OptionInner)

            constexpr OptionInner() noexcept :
                    _value(),
                    _is_present(false) {
            }

            constexpr OptionInner(ValueType value) noexcept :// NOLINT
                    _value(utils::move_or_copy(value)),
                    _is_present(true) {
            }

            ~OptionInner() noexcept = default;

            [[nodiscard]] constexpr auto is_present() const noexcept -> bool {
                return _is_present;
            }

            [[nodiscard]] constexpr auto get() noexcept -> decltype(auto) {
                return _value.get();// Forward rvalue so we can move
            }

            [[nodiscard]] constexpr auto borrow() noexcept -> BorrowedValueType {
                return _value.borrow();
            }

            [[nodiscard]] constexpr auto borrow() const noexcept -> ConstBorrowedValueType {
                return _value.borrow();
            }
        };

        template<typename T>
        struct OptionInner<NonZero<T>> {
            using ValueType = T;
            using Self = OptionInner<NonZero<T>>;
            using BorrowedValueType = ValueType&;
            using ConstBorrowedValueType = const ValueType&;
            using Pointer = ValueType*;
            using ConstPointer = const ValueType*;

            NonZero<ValueType> _value;

            KSTD_DEFAULT_MOVE_COPY(OptionInner)

            constexpr OptionInner() noexcept :
                    _value() {
            }

            constexpr OptionInner(NonZero<ValueType> value) noexcept :// NOLINT
                    _value(value) {
            }

            ~OptionInner() noexcept = default;

            [[nodiscard]] constexpr auto is_present() const noexcept -> bool {
                return !_value.is_empty();
            }

            [[nodiscard]] constexpr auto get() noexcept -> ValueType {
                return _value;// Copy and pass as rvalue to move automatically
            }

            [[nodiscard]] constexpr auto borrow() noexcept -> BorrowedValueType {
                return _value.borrow();
            }

            [[nodiscard]] constexpr auto borrow() const noexcept -> ConstBorrowedValueType {
                return _value.borrow();
            }
        };
    }// namespace

    template<typename T>
    struct Option {
        static constexpr bool is_pointer = meta::is_ptr<T>;
        static constexpr bool is_reference = meta::is_ref<T>;

        using ValueType = T;
        using Self = Option<ValueType>;
        using InnerType = OptionInner<ValueType>;
        using BorrowedValueType = typename InnerType::BorrowedValueType;
        using ConstBorrowedValueType = typename InnerType::ConstBorrowedValueType;
        using Pointer = typename InnerType::Pointer;
        using ConstPointer = typename InnerType::ConstPointer;

        private:
        InnerType _inner;
        bool _is_present;

        public:
        constexpr Option() noexcept :
                _inner(),
                _is_present(false) {
        }

        constexpr Option(ValueType value) noexcept :// NOLINT
                _inner(utils::move_or_copy(value)),
                _is_present(true) {
        }

        constexpr Option(const Self& other) noexcept :
                _inner(),
                _is_present(other._is_present) {
            if(other) {
                drop();
                _inner = other._inner.borrow();
            }
        }

        constexpr Option(Self&& other) noexcept :
                _inner(),
                _is_present(other._is_present) {
            if(other) {
                drop();
                _inner = other._inner.get();
            }
        }

        ~Option() noexcept {
            drop();
        }

        constexpr auto reset() noexcept -> void {
            _is_present = false;
        }

        constexpr auto drop() noexcept -> void {
            if(!_is_present) {
                return;
            }

            _inner.~InnerType();
            _is_present = false;// Mark this option as empty after releasing ownership
        }

        constexpr auto operator=(const Self& other) noexcept -> Self& {
            if(this == &other) {
                return *this;
            }

            if(other) {
                drop();
                _inner = other._inner;
                _is_present = true;
            }

            return *this;
        }

        constexpr auto operator=(Self&& other) noexcept -> Self& {
            if(other) {
                drop();
                _inner = utils::move(other._inner);
                _is_present = true;
            }

            return *this;
        }

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