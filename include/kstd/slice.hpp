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
 * @since 09/08/2023
 */

#pragma once

#include "defaults.hpp"
#include "libc.hpp"
#include "types.hpp"

#ifndef KSTD_DISABLE_STD_SPAN_SUPPORT
#include "language.hpp"
#ifdef KSTD_CPP_20
#define KSTD_STD_SPAN_SUPPORT
#include <span>
#endif
#endif

namespace kstd {
    template<typename T>
    struct Slice final {
        using ValueType = T;
        using Self = Slice<ValueType>;
        using Pointer = ValueType*;
        using ConstPointer = std::conditional_t<std::is_const_v<ValueType>, ValueType*, const ValueType*>;
        using Iterator = Pointer;
        using ConstIterator = ConstPointer;
        using ReverseIterator = std::reverse_iterator<Iterator>;
        using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

        private:
        Pointer _data;
        usize _size;

        public:
        KSTD_DEFAULT_MOVE_COPY(Slice, Self, constexpr)

        constexpr Slice(Pointer data, const usize size) noexcept :
                _data {data},
                _size {size} {
        }

        template<typename I>
        constexpr Slice(I begin, I end) noexcept :
                _data {&(*begin)},
                _size {static_cast<usize>(end - begin) * sizeof(ValueType)} {
        }

        ~Slice() noexcept = default;

        constexpr auto zero() const noexcept -> void {
            libc::memset(_data, 0, _size);
        }

        constexpr auto copy_to(Self& destination) const noexcept -> usize {
            const auto size = std::min(_size, destination._size);
            libc::memcpy(destination._data, _data, size);
            return size;
        }

        constexpr auto copy_to(Pointer data) const noexcept -> void {
            libc::memcpy(data, _data, _size);
        }

        template<typename I>
        constexpr auto copy_to(I begin, I end) const noexcept -> usize {
            return copy_to({begin, end});
        }

        [[nodiscard]] constexpr auto begin() noexcept -> Iterator {
            return _data;
        }

        [[nodiscard]] constexpr auto end() noexcept -> Iterator {
            return reinterpret_cast<Iterator>(reinterpret_cast<u8*>(_data) + _size);// NOLINT
        }

        [[nodiscard]] constexpr auto rbegin() noexcept -> ReverseIterator {
            return ReverseIterator {begin()};
        }

        [[nodiscard]] constexpr auto rend() noexcept -> ReverseIterator {
            return ReverseIterator {end()};
        }

        [[nodiscard]] constexpr auto cbegin() const noexcept -> ConstIterator {
            return _data;
        }

        [[nodiscard]] constexpr auto cend() const noexcept -> ConstIterator {
            return reinterpret_cast<T*>(reinterpret_cast<const u8*>(_data) + _size);// NOLINT
        }

        [[nodiscard]] constexpr auto crbegin() noexcept -> ConstReverseIterator {
            return ConstReverseIterator {cbegin()};
        }

        [[nodiscard]] constexpr auto crend() noexcept -> ConstReverseIterator {
            return ConstReverseIterator {cend()};
        }

        [[nodiscard]] constexpr auto get_data() noexcept -> Pointer {
            return _data;
        }

        [[nodiscard]] constexpr auto get_data() const noexcept -> ConstPointer {
            return _data;
        }

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            return _size;
        }

        [[nodiscard]] constexpr auto get_count() const noexcept -> usize {
            return _size / sizeof(ValueType);
        }

        [[nodiscard]] constexpr operator Pointer() noexcept {
            return _data;
        }

        [[nodiscard]] constexpr operator ConstPointer() const noexcept {
            return _data;
        }

        [[nodiscard]] constexpr auto operator*() noexcept -> Pointer {
            return _data;
        }

        [[nodiscard]] constexpr auto operator*() const noexcept -> ConstPointer {
            return _data;
        }

        [[nodiscard]] constexpr auto operator->() noexcept -> Pointer {
            return _data;
        }

        [[nodiscard]] constexpr auto operator->() const noexcept -> ConstPointer {
            return _data;
        }

        [[nodiscard]] constexpr auto operator[](const usize index) noexcept -> ValueType& {
            return _data[index];
        }

        [[nodiscard]] constexpr auto operator[](const usize index) const noexcept -> const ValueType& {
            return _data[index];
        }

#ifdef KSTD_STD_SPAN_SUPPORT
        [[nodiscard]] constexpr auto clone_into() const noexcept -> std::span<ValueType> {
            return {_data, _size};
        }
#endif// KSTD_STD_SPAN_SUPPORT
    };

    template<typename T>
    Slice(T*, usize) -> Slice<T>;

    // clang-format off
    template<typename I>
    Slice(I, I) -> Slice<std::conditional_t<std::is_pointer_v<I>,
        std::remove_pointer_t<I>,
        std::conditional_t<std::is_const_v<std::remove_pointer_t<typename I::pointer>>,
            const typename I::value_type,
            typename I::value_type>>>;
    // clang-format on

#ifdef KSTD_STD_SPAN_SUPPORT
    template<typename T>
    [[nodiscard]] constexpr auto clone_into(const std::span<T>& value) noexcept -> Slice<T> {
        return {value.data(), value.size_bytes()};
    }
#endif// KSTD_STD_SPAN_SUPPORT
}// namespace kstd