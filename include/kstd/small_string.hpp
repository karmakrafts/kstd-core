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

#include <type_traits>
#include <cstring>
#include <memory>
#include <array>
#include "kstd/concepts.hpp"
#include "types.hpp"
#include "string_concepts.hpp"
#include "string_fwd.hpp"

namespace kstd {
    template<typename CHAR, usize SIZE> //
    KSTD_REQUIRES(concepts::Char<CHAR>)
    union BasicSmallString final {
        static_assert(SIZE <= 128, "Small string must be 128 characters or less");

        static constexpr usize capacity = SIZE;
        static constexpr usize usable_capacity = capacity - 1;
        static constexpr usize char_size = sizeof(CHAR);

        using self_type = BasicSmallString<CHAR, SIZE>;
        using value_type = CHAR;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using size_type = usize;
        using iterator = pointer;
        using const_iterator = const_pointer;

    private:

        CHAR _data[capacity];

        struct {
            CHAR _padding0[usable_capacity];
            usize _size : (char_size << 3) - 1;
            bool _is_large : 1; // We can't overlap with the large string bit
        };

    public:

        BasicSmallString() noexcept :
                _is_large(false) { // Initialize the large bit to 0 for small-string, makes clang-tidy stfu
            std::memset(_data, 0, capacity * char_size);
            _size = capacity; // We start out with n - 1 characters available
        }

        BasicSmallString(const self_type& other) noexcept :
                _is_large(false) { // Initialize the large bit to 0 for small-string, makes clang-tidy stfu
            std::memcpy(_data, other._data, capacity * char_size);
        }

        BasicSmallString(self_type&& other) noexcept :
                _is_large(false) { // Initialize the large bit to 0 for small-string, makes clang-tidy stfu
            std::memcpy(_data, other._data, capacity * char_size);
        }

        constexpr auto operator =(const self_type& other) noexcept -> self_type& {
            if (&other != this) {
                std::memcpy(_data, other._data, capacity * char_size);
            }

            return *this;
        }

        constexpr auto operator =(self_type&& other) noexcept -> self_type& {
            std::memcpy(_data, other._data, capacity * char_size);
            return *this;
        }

        [[nodiscard]] constexpr auto get_data() noexcept -> pointer {
            return _data;
        }

        [[nodiscard]] constexpr auto get_c_str() const noexcept -> const_pointer {
            return _data;
        }

        constexpr auto resize(size_type new_size) noexcept -> void {
            _size = usable_capacity - new_size;
        }

        [[nodiscard]] constexpr auto get_size() const noexcept -> size_type {
            return usable_capacity - _size;
        }

        [[nodiscard]] constexpr auto get_capacity() const noexcept -> size_type {
            return capacity;
        }

        [[nodiscard]] constexpr auto get_capacity_in_bytes() const noexcept -> size_type {
            return capacity * char_size;
        }

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return get_size() == 0;
        }

        [[nodiscard]] constexpr auto begin() noexcept -> iterator {
            return get_data();
        }

        [[nodiscard]] constexpr auto end() noexcept -> iterator {
            return get_data() + get_size(); // Pointer to last char
        }

        [[nodiscard]] constexpr auto cbegin() noexcept -> const_iterator {
            return get_c_str();
        }

        [[nodiscard]] constexpr auto cend() noexcept -> const_iterator {
            return get_c_str() + get_size(); // Pointer to last char
        }

        constexpr auto clear() noexcept -> void {
            std::memset(_data, 0, sizeof(self_type)); // Zero out self
            _size = usable_capacity; // Reset available char count
        }

        [[nodiscard]] constexpr auto operator [](size_type index) noexcept -> value_type& {
            #ifdef BUILD_DEBUG
            if (index > _size) {
                throw std::runtime_error("String index out of bounds");
            }
            #endif
            return _data[index];
        }
    };

    using SmallString = BasicSmallString<char>;
    using SmallWString = BasicSmallString<wchar_t>;

    #ifdef KSTD_SIZED_CHAR_TYPES

    using SmallString8 = BasicSmallString<char8_t>;
    using SmallString16 = BasicSmallString<char16_t>;
    using SmallString32 = BasicSmallString<char32_t>;

    #endif // KSTD_SIZED_CHAR_TYPES
}