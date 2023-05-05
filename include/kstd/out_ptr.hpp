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
#include <cstring>
#include "kstd/concepts.hpp"

namespace kstd {
    namespace {
        #ifdef KSTD_CONCEPTS_AVAILABLE
        template<typename P> //
        concept SmartPointer = requires(P value, typename P::pointer ptr) {
            typename P::pointer;
            requires std::is_pointer_v<typename P::pointer>;
            value.reset(ptr);
            requires std::same_as<decltype(value.reset(ptr)), void>;
        };
        #endif // KSTD_CONCEPTS_AVAILABLE
    }

    /*
     * This is a freestanding implementation
     * of a mechanism identical to std::out_ptr as described here:
     * https://en.cppreference.com/w/cpp/memory/out_ptr_t/out_ptr
     */
    template<typename P> // @formatter:off
    KSTD_REQUIRES(SmartPointer<P>) // @formatter:on
    struct OutPtr final {
        using self_type = OutPtr<P>;
        using element_type = typename P::element_type;
        using smart_pointer = P;
        using pointer = element_type *;

    private:

        smart_pointer &_owner;
        pointer _new_value;

    public:

        explicit OutPtr(smart_pointer &owner) noexcept:
                _owner(owner),
                _new_value(pointer()) {
        }

        ~OutPtr() noexcept {
            if (_new_value == nullptr) {
                return;
            }

            _owner.reset(_new_value);
        }

        constexpr OutPtr(const self_type &other) noexcept = delete;

        constexpr OutPtr(self_type &&other) noexcept :
                _owner(other._owner),
                _new_value(other._new_value)
        {}

        constexpr auto operator=(const self_type &other) noexcept -> self_type& = delete;

        constexpr auto operator=(self_type &&other) noexcept -> self_type& {
            _owner = other._owner;
            _new_value = other._new_value;
            return *this;
        }

        [[nodiscard]] constexpr operator pointer *() noexcept { // NOLINT
            return &_new_value;
        }
    };

    /*
     * A simple free-deleter for smart pointers with ownership of a
     * C-style malloc'ed object.
     */
    template<typename T> //
    struct FreeDeleter final {
        inline auto operator()(T *ptr) const noexcept -> void {
            ::free(ptr);
        }
    };

    template<typename P>
    KSTD_REQUIRES(SmartPointer<P>)
    [[nodiscard]] constexpr auto make_out(P& pointer) noexcept -> OutPtr<P> {
        return OutPtr<P>(pointer);
    }
}