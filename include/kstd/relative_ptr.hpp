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
        using Self = RelativePtr<T, OFFSET>;
        using ElementType = T;
        using OffsetType = OFFSET;
        using Pointer = ElementType*;
        using ConstPointer = const ElementType*;
        using Reference = ElementType&;
        using ConstReference = const ElementType&;
        using SizeType = usize;

        private:

        OffsetType _offset;

        public:

        constexpr RelativePtr() noexcept :
                _offset(0) {
        }

        constexpr RelativePtr(const Self& other) noexcept = delete;

        constexpr RelativePtr(Self&& other) noexcept = delete;

        ~RelativePtr() noexcept = default;

        constexpr auto operator =(const Self& other) noexcept -> Self& = delete;

        constexpr auto operator =(Self&& other) noexcept -> Self& = delete;

        constexpr auto operator =(Pointer value) noexcept -> Self& {
            set(value);
            return *this;
        }

        constexpr auto set(T* value) noexcept -> void {
            _offset = static_cast<OffsetType>(reinterpret_cast<u8*>(value) - reinterpret_cast<u8*>(this));
        }

        [[nodiscard]] constexpr auto get() noexcept -> Pointer {
            return reinterpret_cast<T*>(reinterpret_cast<u8*>(this) + _offset);
        }

        [[nodiscard]] constexpr auto get() const noexcept -> ConstPointer {
            return reinterpret_cast<T*>(reinterpret_cast<u8*>(this) + _offset);
        }

        [[nodiscard]] constexpr operator Pointer() noexcept { // NOLINT
            return get();
        }

        [[nodiscard]] constexpr operator ConstPointer() const noexcept { // NOLINT
            return get();
        }

        [[nodiscard]] constexpr operator bool() const noexcept { // NOLINT
            return _offset != 0;
        }

        [[nodiscard]] constexpr auto operator *() noexcept -> Reference {
            return *get();
        }

        [[nodiscard]] constexpr auto operator *() const noexcept -> ConstReference {
            return *get();
        }

        [[nodiscard]] constexpr auto operator ->() noexcept -> Pointer {
            return get();
        }

        [[nodiscard]] constexpr auto operator ->() const noexcept -> ConstPointer {
            return get();
        }

        [[nodiscard]] constexpr auto operator [](SizeType index) noexcept -> Reference {
            return get()[index];
        }

        [[nodiscard]] constexpr auto operator [](SizeType index) const noexcept -> ConstReference {
            return get()[index];
        }
    };
}