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
#include <cstring>
#include "kstd/concepts.hpp"
#include "meta.hpp"

namespace kstd {
    namespace {
        #ifdef KSTD_CONCEPTS_AVAILABLE
        template<typename P> //
        concept SmartPointer = requires(P value, typename P::pointer ptr) {
            typename P::pointer;
            requires meta::is_ptr<typename P::pointer>;
            value.reset(ptr);
            requires meta::is_same<decltype(value.reset(ptr)), void>;
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
        using Self = OutPtr<P>;
        using ElementType = typename P::element_type;
        using SmartPointer = P;
        using Pointer = ElementType*;

        private:

        SmartPointer& _owner;
        Pointer _new_value;

        public:

        explicit constexpr OutPtr(SmartPointer& owner) noexcept :
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

        [[nodiscard]] constexpr operator Pointer*() noexcept { // NOLINT
            return &_new_value;
        }
    };

    /*
     * A simple free-deleter for smart pointers with ownership of a
     * C-style malloc'ed object.
     */
    template<typename T> //
    struct FreeDeleter final {
        inline auto operator ()(T* ptr) const noexcept -> void {
            ::free(ptr);
        }
    };

    template<typename P>
    KSTD_REQUIRES(SmartPointer<P>)
    [[nodiscard]] constexpr auto make_out(P& pointer) noexcept -> decltype(auto) {
        return OutPtr<P>(pointer);
    }
}