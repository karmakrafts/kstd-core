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
 * @since 01/05/2023
 */

#pragma once

#include <exception>
#include <concepts>
#include <type_traits>

namespace kstd {
    /*
     * This is a freestanding implementation
     * of a mechanism identical to std::out_ptr as described here:
     * https://en.cppreference.com/w/cpp/memory/out_ptr_t/out_ptr
     */
    template<typename P> requires requires(P value, typename P::pointer ptr) {
        typename P::pointer;
        requires std::is_pointer_v<typename P::pointer>;
        value.reset(ptr);
        requires std::same_as<decltype(value.reset(ptr)), void>;
    }
    struct OutPtr final {
        using self_type = OutPtr<P>;
        using element_type = typename P::element_type;
        using smart_pointer = P;
        using pointer = element_type*;

        private:

        smart_pointer& _owner;
        pointer _new_value;

        public:

        explicit constexpr OutPtr(smart_pointer& owner) noexcept:
                _owner(owner),
                _new_value(pointer()) {
        }

        constexpr ~OutPtr() noexcept {
            #ifdef BUILD_DEBUG
            if (_new_value == nullptr) {
                throw std::runtime_error("Invalid value");
            }
            #endif

            _owner.reset(_new_value);
        }

        constexpr OutPtr(const self_type& other) noexcept = delete;

        constexpr OutPtr(self_type&& other) noexcept = delete;

        constexpr auto operator =(const self_type& other) noexcept = delete;

        constexpr auto operator =(self_type&& other) noexcept = delete;

        [[nodiscard]] constexpr operator pointer*() noexcept { // NOLINT
            return &_new_value;
        }
    };
}