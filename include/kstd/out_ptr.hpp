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

#include "meta.hpp"

namespace kstd {
    /*
     * This is a freestanding implementation
     * of a mechanism identical to std::out_ptr as described here:
     * https://en.cppreference.com/w/cpp/memory/out_ptr_t/out_ptr
     */
    template<typename P> // @formatter:off
    struct OutPtr final {
        using OwnerType = P;
        using ValueType = typename OwnerType::ValueType;
        using Self = OutPtr<OwnerType>;

        private:

        OwnerType& _owner;
        ValueType* _new_value;

        public:

        explicit constexpr OutPtr(OwnerType& owner) noexcept :
                _owner(owner),
                _new_value() {
        }

        ~OutPtr() noexcept {
            if (_new_value == nullptr) {
                return;
            }

            _owner.reset(_new_value);
        }

        constexpr OutPtr(const Self& other) noexcept = delete;

        constexpr OutPtr(Self&& other) noexcept :
                _owner(other._owner),
                _new_value(other._new_value) {
        }

        constexpr auto operator =(const Self& other) noexcept -> Self& = delete;

        constexpr auto operator =(Self&& other) noexcept -> Self& {
            _owner = other._owner;
            _new_value = other._new_value;
            return *this;
        }

        [[nodiscard]] constexpr operator ValueType**() noexcept { // NOLINT
            return &_new_value;
        }
    };

    template<typename P>
    [[nodiscard]] constexpr auto make_out(P& pointer) noexcept -> decltype(auto) {
        return OutPtr<P>(pointer);
    }
}