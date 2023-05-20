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
            utils::fill_array<CHAR, SIZE>(_data, 0);
            _available = usable_size;
            _is_on_heap = false;
        }

        constexpr BasicSmallString(const ValueType* data) noexcept { // NOLINT
            utils::fill_array<CHAR, SIZE>(_data, 0);
            const auto length = libc::get_string_length(data);
            _available = usable_size - length;
            _is_on_heap = false;
            libc::copy_string(_data, data, length);
        }

        ~BasicSmallString() noexcept = default;

        [[nodiscard]] constexpr auto get_capacity() const noexcept -> usize {
            return usable_size;
        }

        [[nodiscard]] constexpr auto get_capacity_in_bytes() const noexcept -> usize {
            return usable_size * sizeof(ValueType);
        }

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            return usable_size - _available;
        }

        [[nodiscard]] constexpr auto get_size_in_bytes() const noexcept -> usize {
            return (usable_size - _available) * sizeof(ValueType);
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

        constexpr BasicHeapString(const ValueType* data) noexcept : // NOLINT
                _capacity(libc::get_string_length(data) + 1),
                _data(Allocator().allocate(_capacity)),
                _size(_capacity - 1),
                _is_on_heap(true) {
            libc::copy_string(_data, data, _size);
        }

        ~BasicHeapString() noexcept = default;

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

        [[nodiscard]] constexpr auto get_capacity() const noexcept -> usize {
            if (_is_on_heap) {
                return _heap.get_capacity();
            }

            return _small.get_capacity();
        }

        [[nodiscard]] constexpr auto get_capacity_in_bytes() const noexcept -> usize {
            if (_is_on_heap) {
                return _heap.get_capacity_in_bytes();
            }

            return _small.get_capacity_in_bytes();
        }

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            if (_is_on_heap) {
                return _heap.get_size();
            }

            return _small.get_size();
        }

        [[nodiscard]] constexpr auto get_size_in_bytes() const noexcept -> usize {
            if (_is_on_heap) {
                return _heap.get_size_in_bytes();
            }

            return _small.get_size_in_bytes();
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

    template<usize SIZE> //
    using SmallString = BasicSmallString<char, SIZE>;

    template<usize SIZE> //
    using WSmallString = BasicSmallString<wchar_t, SIZE>;

    using HeapString = BasicHeapString<char>;
    using WHeapString = BasicHeapString<wchar_t>;

    using String = BasicString<char>;
    using WString = BasicString<wchar_t>;
}