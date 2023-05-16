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

#include <cstring>
#include "types.hpp"
#include "utils.hpp"
#include "meta.hpp"
#include "box.hpp"

namespace kstd {
    template<typename T> //
    struct Option final {
        static constexpr bool is_pointer = meta::is_ptr<T>;
        static constexpr bool is_reference = meta::is_ref<T>;

        using Self = Option<T>;
        using ValueType = T;
        using InnerType = Box<ValueType>;

        private:

        InnerType _inner;
        bool _is_present;

        public:

        constexpr Option() noexcept :
                _inner(),
                _is_present(false) {
        }

        constexpr Option(ValueType value) noexcept : // NOLINT
                _inner(move_or_copy(value)),
                _is_present(true) {
        }

        constexpr Option(const Self& other) noexcept :
                _inner(),
                _is_present(other._is_present) {
            if (other) {
                drop();
                _inner = other._inner.borrow();
            }
        }

        constexpr Option(Self&& other) noexcept :
                _inner(),
                _is_present(other._is_present) {
            if (other) {
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
            _inner.~InnerType();
            _is_present = false; // Mark this option as empty after releasing ownership
        }

        constexpr auto operator =(const Self& other) noexcept -> Self& {
            if (this == &other) {
                return *this;
            }

            if (other) {
                drop();
                _inner = other._inner;
                _is_present = true;
            }

            return *this;
        }

        constexpr auto operator =(Self&& other) noexcept -> Self& {
            if (other) {
                drop();
                _inner = move(other._inner);
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

        [[nodiscard]] constexpr auto borrow() noexcept -> decltype(auto) {
            #ifdef BUILD_DEBUG
            if (is_empty()) {
                throw std::runtime_error("Result has no value");
            }
            #endif

            return _inner.borrow();
        }

        [[nodiscard]] constexpr auto get() noexcept -> decltype(auto) {
            #ifdef BUILD_DEBUG
            if (is_empty()) {
                throw std::runtime_error("Result has no value");
            }
            #endif

            return _inner.get();
        }

        [[nodiscard]] constexpr operator bool() const noexcept { // NOLINT
            return has_value();
        }

        [[nodiscard]] constexpr auto operator *() noexcept -> decltype(auto) {
            return borrow();
        }

        [[nodiscard]] constexpr auto operator ->() noexcept -> decltype(auto) {
            return &borrow();
        }
    };

    template<typename T>
    [[nodiscard]] constexpr auto make_empty() noexcept -> decltype(auto) {
        return Option<T>();
    }

    template<typename T>
    [[nodiscard]] constexpr auto make_value(T value) noexcept -> decltype(auto) {
        return Option<T>(move_or_copy(value));
    }
}