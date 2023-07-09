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

#include "defaults.hpp"

namespace kstd {
    /*
     * This is a freestanding implementation
     * of a mechanism identical to std::out_ptr as described here:
     * https://en.cppreference.com/w/cpp/memory/out_ptr_t/out_ptr
     */
    template<typename P>// @formatter:off
    struct OutPtr final {
        using OwnerType = P;
        using ElementType = typename OwnerType::element_type;
        using Self = OutPtr<OwnerType>;

        private:
        OwnerType& _owner;// NOLINT
        ElementType* _new_value;

        public:
        KSTD_NO_MOVE_COPY(OutPtr, Self, constexpr)

        explicit constexpr OutPtr(OwnerType& owner) noexcept :
                _owner(owner),
                _new_value() {
        }

        ~OutPtr() noexcept {
            if(_new_value == nullptr) {
                return;
            }

            _owner.reset(_new_value);
        }

        [[nodiscard]] constexpr operator ElementType**() noexcept {// NOLINT
            return &_new_value;
        }
    };

    template<typename P>
    [[nodiscard]] constexpr auto make_out(P& pointer) noexcept -> OutPtr<P> {
        return OutPtr<P>(pointer);
    }
}// namespace kstd