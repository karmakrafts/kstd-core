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
 * @since 05/05/2023
 */

#pragma once

#include "allocator.hpp"
#include "array.hpp"
#include "libc.hpp"
#include "meta.hpp"
#include "small_string.hpp"
#include "string_fwd.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace kstd {
    template<typename CHAR, typename ALLOCATOR>
    struct BasicHeapString final {
        using ValueType = CHAR;
        using Allocator = ALLOCATOR;
        using Self = BasicHeapString<ValueType, Allocator>;
        using Pointer = ValueType*;
        using ConstPointer = const ValueType*;

        private:
        Pointer _data;
        usize _capacity;
        usize _size : (sizeof(usize) << 3) - 1;
        [[maybe_unused]] bool _is_on_heap : 1;

        public:
        constexpr BasicHeapString() noexcept :
                _data(Allocator().allocate_zero(1)),
                _capacity(1),
                _size(0),
                _is_on_heap(true) {
        }

        constexpr BasicHeapString(const ValueType* data) noexcept :// NOLINT
                _capacity(libc::get_string_length(data) + 1),
                _data(Allocator().allocate_zero(_capacity)),
                _size(_capacity - 1),
                _is_on_heap(true) {
            libc::copy_string(_data, data, get_size_in_bytes());
        }

        constexpr BasicHeapString(const Self& other) noexcept :
                _capacity(other.get_capacity()),
                _data(Allocator().allocate_zero(_capacity)),
                _size(_capacity - 1),
                _is_on_heap(true) {
            libc::copy_string(_data, other._data, other.get_size_in_bytes());
        }

        constexpr BasicHeapString(Self&& other) noexcept :
                _capacity(other.get_capacity()),
                _data(Allocator().allocate_zero(_capacity)),
                _size(_capacity - 1),
                _is_on_heap(true) {
            libc::copy_string(_data, other._data, other.get_size_in_bytes());
        }

        ~BasicHeapString() noexcept {
            Allocator().deallocate(_data, _capacity);
        }

        constexpr auto operator=(const Self& other) noexcept -> Self& {
            reserve(other.get_size());
            return *this;
        }

        constexpr auto reserve(usize count) noexcept -> void {
        }

        [[nodiscard]] constexpr auto get_capacity() const noexcept -> usize {
            return _capacity;
        }

        [[nodiscard]] constexpr auto get_capacity_in_bytes() const noexcept -> usize {
            return _capacity * sizeof(ValueType);
        }

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            return _size;
        }

        [[nodiscard]] constexpr auto get_size_in_bytes() const noexcept -> usize {
            return _size * sizeof(ValueType);
        }

        [[nodiscard]] constexpr auto get_data() noexcept -> Pointer {
            return _data;
        }

        [[nodiscard]] constexpr auto get_data() const noexcept -> ConstPointer {
            return _data;
        }

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return get_size() == 0;
        }
    };

    using HeapString = BasicHeapString<char>;
    using WHeapString = BasicHeapString<wchar_t>;

    template<typename CHAR, typename ALLOCATOR = Allocator<CHAR>>
    union BasicString final {
        using ValueType = CHAR;
        using Allocator = ALLOCATOR;
        using SmallType = BasicSmallString<ValueType>;
        using HeapType = BasicHeapString<ValueType, Allocator>;
        using Self = BasicString<ValueType, Allocator>;
        using Pointer = ValueType*;
        using ConstPointer = const ValueType*;

        private:
        SmallType _small;
        HeapType _heap;

        struct {
            [[maybe_unused]] Array<ValueType, SmallType::capacity - 1> _padding0;
            [[maybe_unused]] ValueType _padding1 : (sizeof(ValueType) << 3) - 1;
            bool _is_on_heap : 1;
        };

        public:
        constexpr BasicString() noexcept :
                _small() {
            _is_on_heap = false;// Make clang-tidy shut up..
        }

        constexpr BasicString(const Self& other) noexcept {// NOLINT: we DO initialize them, conditionally..
            if(other._is_on_heap) { new(&_heap) HeapType(other._heap); }
            else { new(&_small) SmallType(other._small); }
        }

        constexpr BasicString(Self&& other) noexcept {// NOLINT: we DO initialize them, conditionally..
            if(other._is_on_heap) { new(&_heap) HeapType(other._heap); }
            else { new(&_small) SmallType(other._small); }
        }

        ~BasicString() noexcept {
            if(_is_on_heap) {
                _heap.~HeapType();
                return;
            }

            _small.~SmallType();
        }

        [[nodiscard]] constexpr auto get_capacity() const noexcept -> usize {
            if(_is_on_heap) { return _heap.get_capacity(); }

            return _small.get_capacity();
        }

        [[nodiscard]] constexpr auto get_capacity_in_bytes() const noexcept -> usize {
            if(_is_on_heap) { return _heap.get_capacity_in_bytes(); }

            return _small.get_capacity_in_bytes();
        }

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            if(_is_on_heap) { return _heap.get_size(); }

            return _small.get_size();
        }

        [[nodiscard]] constexpr auto get_size_in_bytes() const noexcept -> usize {
            if(_is_on_heap) { return _heap.get_size_in_bytes(); }

            return _small.get_size_in_bytes();
        }

        [[nodiscard]] constexpr auto get_data() noexcept -> Pointer {
            if(_is_on_heap) { return _heap.get_data(); }

            return _small.get_data();
        }

        [[nodiscard]] constexpr auto get_data() const noexcept -> ConstPointer {
            if(_is_on_heap) { return _heap.get_data(); }

            return _small.get_data();
        }

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return get_size() == 0;
        }
    };

    using String = BasicString<char>;
    using WString = BasicString<wchar_t>;
}// namespace kstd