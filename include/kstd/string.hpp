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

#include "types.hpp"
#include "allocator.hpp"
#include "libc.hpp"
#include "meta.hpp"
#include "utils.hpp"

namespace kstd {
    template<typename CHAR, typename ALLOCATOR = Allocator<CHAR>>
    struct BasicHeapString;

    template<typename CHAR, usize SIZE = sizeof(BasicHeapString<CHAR, Allocator<CHAR>>) / sizeof(CHAR)>
    union BasicSmallString final {
        static constexpr usize size = SIZE;
        static constexpr usize usable_size = size - 1;

        using ValueType = CHAR;
        using Self = BasicSmallString<ValueType, size>;
        using Pointer = ValueType*;
        using ConstPointer = const ValueType*;

        private:

        ValueType _data[size];

        struct {
            [[maybe_unused]] ValueType _padding0[size - 1];
            ValueType _available : (sizeof(ValueType) << 3) - 1;
            [[maybe_unused]] bool _is_on_heap : 1;
        };

        public:

        constexpr BasicSmallString() noexcept {
            libc::zero(_data);
            _available = usable_size;
            _is_on_heap = false;
        }

        ~BasicSmallString() noexcept = default;

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            return usable_size - _available;
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
        bool _is_on_heap : 1;

        public:

        constexpr BasicHeapString() noexcept :
                _data(Allocator().allocate(1)),
                _capacity(1),
                _size(0),
                _is_on_heap(true) {
        }

        explicit constexpr BasicHeapString(const ValueType* value) noexcept :
                _data(Allocator().allocate(libc::get_string_length(value))),
                _capacity(libc::get_string_length(value)),
                _size(_capacity),
                _is_on_heap(true) {
        }

        ~BasicHeapString() noexcept = default;

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            return libc::get_string_length(_data);
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
            [[maybe_unused]] ValueType _padding0[SmallType::size - 1];
            [[maybe_unused]] ValueType _padding1 : (sizeof(ValueType) << 3) - 1;
            bool _is_on_heap : 1;
        };

        public:

        constexpr BasicString() noexcept {
            libc::zero(_small);
            _is_on_heap = false; // Make clang-tidy shut up..
        }

        ~BasicString() noexcept = default;

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            if (_is_on_heap) {
                return _heap.get_size();
            }

            return _small.get_size();
        }

        [[nodiscard]] constexpr auto get_data() noexcept -> Pointer {
            if (_is_on_heap) {
                return _heap.get_data();
            }

            return _small.get_data();
        }

        [[nodiscard]] constexpr auto get_data() const noexcept -> ConstPointer {
            if (_is_on_heap) {
                return _heap.get_data();
            }

            return _small.get_data();
        }

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return get_size() == 0;
        }
    };

    template<usize SIZE = sizeof(BasicHeapString<char, Allocator<char>>) / sizeof(char)> //
    using SmallString = BasicSmallString<char, SIZE>;

    template<usize SIZE = sizeof(BasicHeapString<wchar_t, Allocator<wchar_t>>) / sizeof(wchar_t)> //
    using WSmallString = BasicSmallString<wchar_t, SIZE>;

    template<typename ALLOCATOR = Allocator<char>> //
    using HeapString = BasicHeapString<char, ALLOCATOR>;

    template<typename ALLOCATOR = Allocator<wchar_t>> //
    using WHeapString = BasicHeapString<wchar_t, ALLOCATOR>;

    template<typename ALLOCATOR = Allocator<char>> //
    using String = BasicString<char, ALLOCATOR>;

    template<typename ALLOCATOR = Allocator<wchar_t>> //
    using WString = BasicString<wchar_t, ALLOCATOR>;
}