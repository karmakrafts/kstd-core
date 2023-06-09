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

namespace kstd {
    template<typename T>
    struct Option final {
        static_assert(!std::is_same_v<std::decay_t<T>, Void>, "Type cannot be Void");

        // clang-format off
        using value_type        = T;
        using self              = Option<value_type>;
        using boxed_value_type  = Box<value_type>;
        using reference         = typename boxed_value_type::reference;
        using const_reference   = typename boxed_value_type::const_reference;
        using pointer           = typename boxed_value_type::pointer;
        using const_pointer     = typename boxed_value_type::const_pointer;
        // clang-format on

        private:
        boxed_value_type _value;

        public:
        KSTD_DEFAULT_MOVE_COPY(Option, self, constexpr)

        constexpr Option() noexcept :
                _value() {
        }

        constexpr Option(value_type value) noexcept :// NOLINT
                _value(std::forward<value_type>(value)) {
        }

        ~Option() noexcept = default;

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _value.is_empty();
        }

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool {
            return !_value.is_empty();
        }

        [[nodiscard]] constexpr auto get() noexcept -> reference {
            return _value.get();
        }

        [[nodiscard]] constexpr auto get() const noexcept -> const_reference {
            return _value.get();
        }

        [[nodiscard]] constexpr auto get_or(value_type default_value) noexcept -> value_type {
            if(has_value()) {
                return get();
            }
            return default_value;
        }

        template<typename R, typename F>
        [[nodiscard]] constexpr auto map(F&& function) const noexcept -> Option<R> {
            static_assert(std::is_convertible_v<F, std::function<R(value_type)>>, "Function signature does not match");
            if(is_empty()) {
                return {};
            }
            return {function(get())};
        }

        [[nodiscard]] constexpr operator bool() const noexcept {// NOLINT
            return has_value();
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
}// namespace kstd