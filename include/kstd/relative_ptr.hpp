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
 * @since 02/05/2023
 */

#pragma once

#include "types.hpp"
#include "kstd/concepts.hpp"

namespace kstd {
    template<typename T, typename OFFSET = u32> //
    KSTD_REQUIRES(!std::is_void<T>::value && std::integral<OFFSET>)
    struct RelativePtr final {
        using self_type = RelativePtr<T, OFFSET>;
        using element_type = T;
        using offset_type = OFFSET;
        using pointer = element_type*;
        using const_pointer = const element_type*;
        using reference = element_type&;
        using const_reference = const element_type&;
        using size_type = usize;

        private:

        offset_type _offset;

        public:

        RelativePtr() noexcept :
                _offset(0) {
        }

        ~RelativePtr() noexcept = default;

        RelativePtr(const self_type& other) noexcept = delete;

        RelativePtr(self_type&& other) noexcept = delete;

        constexpr auto operator =(const self_type& other) noexcept -> self_type& = delete;

        constexpr auto operator =(self_type&& other) noexcept -> self_type& = delete;

        constexpr auto operator =(pointer value) noexcept -> self_type& {
            set(value);
            return *this;
        }

        constexpr auto set(T* value) noexcept -> void {
            _offset = static_cast<offset_type>(reinterpret_cast<u8*>(value) - reinterpret_cast<u8*>(this));
        }

        [[nodiscard]] constexpr auto get() noexcept -> pointer {
            return reinterpret_cast<T*>(reinterpret_cast<u8*>(this) + _offset);
        }

        [[nodiscard]] constexpr auto get() const noexcept -> const_pointer {
            return reinterpret_cast<T*>(reinterpret_cast<u8*>(this) + _offset);
        }

        [[nodiscard]] constexpr operator pointer() noexcept { // NOLINT
            return get();
        }

        [[nodiscard]] constexpr operator const_pointer() const noexcept { // NOLINT
            return get();
        }

        [[nodiscard]] constexpr operator bool() const noexcept { // NOLINT
            return _offset != 0;
        }

        [[nodiscard]] constexpr auto operator *() noexcept -> reference {
            return *get();
        }

        [[nodiscard]] constexpr auto operator *() const noexcept -> const_reference {
            return *get();
        }

        [[nodiscard]] constexpr auto operator ->() noexcept -> pointer {
            return get();
        }

        [[nodiscard]] constexpr auto operator ->() const noexcept -> const_pointer {
            return get();
        }

        [[nodiscard]] constexpr auto operator [](size_type index) noexcept -> reference {
            return get()[index];
        }

        [[nodiscard]] constexpr auto operator [](size_type index) const noexcept -> const_reference {
            return get()[index];
        }
    };
}