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
 * @since 21/05/2023
 */

#pragma once

#include "assert.hpp"
#include "libc.hpp"
#include "types.hpp"
#include "string_fwd.hpp"

namespace kstd {
    template<typename CHAR, usize SIZE>
    union BasicSmallString final {
        static constexpr usize capacity = SIZE;
        static constexpr usize usable_capacity = capacity - 1;
        [[maybe_unused]] static constexpr usize byte_capacity = capacity * sizeof(CHAR);

        using ValueType = CHAR;
        using Self = BasicSmallString<ValueType, SIZE>;
        using Pointer = ValueType*;
        using ConstPointer = const ValueType*;

        private:

        ValueType _data[capacity];

        struct {
            [[maybe_unused]] ValueType _padding0[capacity - 1];
            ValueType _available_capacity : (sizeof(ValueType) << 3) - 1;
            [[maybe_unused]] bool _is_on_heap : 1;
        };

        public:

        constexpr BasicSmallString() noexcept :
                _is_on_heap(false) {
            clear();
        }

        constexpr BasicSmallString(const ValueType* data) noexcept : // NOLINT
                _is_on_heap(false) {
            assert_true(data != nullptr);
            set(data, libc::get_string_length(data));
        }

        constexpr BasicSmallString(const Self& other) noexcept :
                _is_on_heap(false) {
            set(other._data, other.get_size());
        }

        constexpr BasicSmallString(Self&& other) noexcept :
                _is_on_heap(false) {
            set(other._data, other.get_size());
        }

        ~BasicSmallString() noexcept = default;

        constexpr auto operator =(const Self& other) noexcept -> Self& {
            if (this == &other) {
                return *this;
            }

            set(other._data, other.get_size());
            return *this;
        }

        constexpr auto operator =(Self&& other) noexcept -> Self& {
            set(other._data, other.get_size());
            return *this;
        }

        constexpr auto resize(usize size) noexcept -> void {
            _available_capacity = usable_capacity - size;
        }

        constexpr auto clear() noexcept -> void {
            if (is_empty()) {
                return;
            }

            libc::memset(_data, 0, byte_capacity);
            _available_capacity = usable_capacity;
        }

        constexpr auto set(const ValueType* data, usize size) noexcept -> void {
            assert_true(data != nullptr);
            assert_true(size <= usable_capacity);
            libc::memset(_data, 0, byte_capacity);
            resize(size);
            libc::copy_string(_data, data, size);
        }

        [[nodiscard]] constexpr auto get_capacity() const noexcept -> usize {
            return capacity;
        }

        [[nodiscard]] constexpr auto get_capacity_in_bytes() const noexcept -> usize {
            return capacity * sizeof(ValueType);
        }

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            return usable_capacity - _available_capacity;
        }

        [[nodiscard]] constexpr auto get_size_in_bytes() const noexcept -> usize {
            return (usable_capacity - _available_capacity) * sizeof(ValueType);
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

        template<usize OTHER_SIZE>
        [[nodiscard]] constexpr auto concat(const BasicSmallString<ValueType, OTHER_SIZE>& other) noexcept -> BasicSmallString<ValueType, SIZE + OTHER_SIZE> {
            BasicSmallString<ValueType, SIZE + OTHER_SIZE> result;
            const auto size = get_size_in_bytes();
            const auto other_size = other.get_size_in_bytes();
            libc::memcpy(result.get_data(), _data, size);
            libc::memcpy(result.get_data() + size, other.get_data(), other_size);
            result.resize(size + other_size);
            return result;
        }

        template<usize OTHER_SIZE>
        [[nodiscard]] constexpr auto operator +(const BasicSmallString<ValueType, OTHER_SIZE>& other) noexcept -> BasicSmallString<ValueType, SIZE + OTHER_SIZE> {
            return concat<OTHER_SIZE>(other);
        }

        template<usize OTHER_SIZE>
        [[nodiscard]] constexpr auto operator ==(const BasicSmallString<ValueType, OTHER_SIZE>& other) const noexcept -> bool {
            return libc::compare_string(_data, other._data) == 0;
        }

        [[nodiscard]] constexpr auto operator ==(const ValueType* data) const noexcept -> bool {
            assert_true(data != nullptr);
            return libc::compare_string(_data, data) == 0;
        }

        template<usize OTHER_SIZE>
        [[nodiscard]] constexpr auto operator !=(const BasicSmallString<ValueType, OTHER_SIZE>& other) const noexcept -> bool {
            return libc::compare_string(_data, other._data) != 0;
        }

        [[nodiscard]] constexpr auto operator !=(const ValueType* data) const noexcept -> bool {
            assert_true(data != nullptr);
            return libc::compare_string(_data, data) != 0;
        }
    };

    template<usize SIZE> //
    using SmallString = BasicSmallString<char, SIZE>;

    template<usize SIZE> //
    using WSmallString = BasicSmallString<wchar_t, SIZE>;
}