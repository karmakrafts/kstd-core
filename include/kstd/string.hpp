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
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either expressor implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @author Alexander Hinze
 * @since 22/04/2023
 */

#pragma once

#include <cstdint>
#include <type_traits>
#include <concepts>
#include <cstring>
#include <memory>
#include <array>
#include "kstd/concepts.hpp"
#include "types.hpp"

namespace kstd {
    namespace {
        #ifdef KSTD_CONCEPTS_AVAILABLE
        template<typename T> concept Char8 = std::integral<T> && sizeof(T) == 1;
        template<typename T> concept Char16 = std::integral<T> && sizeof(T) == 2;
        template<typename T> concept Char32 = std::integral<T> && sizeof(T) == 4;
        template<typename T> concept Char = Char8<T> || Char16<T> || Char32<T>;
        #endif // KSTD_CONCEPTS_AVAILABLE
    }

    template<typename CHAR, usize SIZE = 24 / sizeof(CHAR)> //
    KSTD_REQUIRES(Char<CHAR>)
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

    namespace {
        template<typename CHAR> //
        KSTD_REQUIRES(Char<CHAR>)
        struct StringInner final {
            CHAR* data;
            usize capacity;
            usize size : (sizeof(usize) << 3) - 1;
            bool is_large : 1;
        };
    }

    template<typename CHAR> //
    KSTD_REQUIRES(Char<CHAR>)
    struct BasicStringSlice;

    template<typename CHAR, concepts::Allocator ALLOCATOR = std::allocator<CHAR>> //
    KSTD_REQUIRES(Char<CHAR>)
    union BasicString final {
        using value_type = CHAR;
        using self_type = BasicString<value_type, ALLOCATOR>;
        using inner_type = StringInner<value_type>;
        using small_type = BasicSmallString<value_type, sizeof(inner_type) / sizeof(value_type)>;
        using view_type = BasicStringSlice<CHAR>;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using size_type = usize;
        using iterator = pointer;
        using const_iterator = const_pointer;

        static_assert(sizeof(small_type) == sizeof(inner_type)); // Make sure size is calculated right

    private:

        inner_type _inner;
        small_type _small;

        [[nodiscard]] constexpr auto is_small() const noexcept -> bool {
            return !_inner.is_large;
        }

    public:

        BasicString() noexcept :
                _small() {
        }

        // This allows for implicit assignment of string slices to owning strings
        BasicString(view_type view) noexcept : // NOLINT
                _small() {
            *this = std::move(view.to_owning());
        }

        BasicString(const self_type& other) noexcept :
                _small() {
        }

        BasicString(self_type&& other) noexcept :
                _small() {
        }

        constexpr auto operator =(const self_type& other) noexcept -> self_type& {
            return *this;
        }

        constexpr auto operator =(self_type&& other) noexcept -> self_type& {
            return *this;
        }

        [[nodiscard]] constexpr auto get_data() noexcept -> pointer {
            if (is_small()) {
                return _small.get_data();
            }

            return _inner.data;
        }

        [[nodiscard]] constexpr auto get_c_str() const noexcept -> const_pointer {
            if (is_small()) {
                return _small.get_c_str();
            }

            return _inner.data;
        }

        [[nodiscard]] constexpr auto get_size() const noexcept -> size_type {
            if (is_small()) {
                return _small.get_size();
            }

            return _inner.size;
        }

        [[nodiscard]] constexpr auto get_capacity() const noexcept -> size_type {
            if (is_small()) {
                return small_type::capacity;
            }

            return _inner.capacity;
        }

        [[nodiscard]] constexpr auto get_capacity_in_bytes() const noexcept -> size_type {
            if (is_small()) {
                return small_type::capacity * sizeof(CHAR);
            }

            return _inner.capacity * sizeof(CHAR);
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
            if (is_small()) {
                _small.clear();
                return;
            }

            std::memset(_inner.data, 0, get_capacity_in_bytes());
        }

        [[nodiscard]] constexpr auto get_allocator() const noexcept -> ALLOCATOR {
            return {};
        }

        constexpr auto reserve(size_type new_capacity, bool collapse = true) noexcept -> void {

        }

        [[nodiscard]] constexpr auto compact() noexcept -> self_type& {
            reserve(get_capacity());
            return *this;
        }

        [[nodiscard]] constexpr auto operator [](size_type index) noexcept -> value_type& {
            #ifdef BUILD_DEBUG
            if (index > get_size()) {
                throw std::runtime_error("String index out of bounds");
            }
            #endif
            return *(get_data() + index);
        }
    };

    using String = BasicString<char>;
    using WString = BasicString<wchar_t>;
    using String8 = BasicString<char8_t>;
    using String16 = BasicString<char16_t>;
    using String32 = BasicString<char32_t>;

    template<typename CHAR> //
    KSTD_REQUIRES(Char<CHAR>)
    struct BasicStringSlice final {
        using self_type = BasicStringSlice<CHAR>;
        using value_type = CHAR;
        using const_pointer = const value_type*;
        using size_type = usize;
        using const_iterator = const_pointer;

    private:

        const_pointer _data;
        size_type _size;

    public:

        constexpr BasicStringSlice() noexcept :
                _data(nullptr),
                _size(0) {
        }

        explicit constexpr BasicStringSlice(const_pointer data, size_type size) noexcept :
                _data(data),
                _size(size) {
        }

        constexpr BasicStringSlice(const self_type& other) noexcept = default;

        constexpr BasicStringSlice(self_type&& other) noexcept = default;

        constexpr auto operator =(const self_type& other) noexcept -> self_type& = default;

        constexpr auto operator =(self_type&& other) noexcept -> self_type& = default;

        template<concepts::Allocator ALLOCATOR = std::allocator<CHAR>>
        [[nodiscard]] constexpr auto to_owning() const noexcept -> BasicString<CHAR, ALLOCATOR> {
            BasicString<CHAR, ALLOCATOR> result;
            result.reserve(_size + 1);
            std::memcpy(result.get_data(), _data, _size);
            result[_size] = static_cast<CHAR>(0); // Insert null-terminator
            return result;
        }

        [[nodiscard]] constexpr auto get_data() const noexcept -> const_pointer {
            return _data;
        }

        [[nodiscard]] constexpr auto get_size() const noexcept -> size_type {
            return _size;
        }

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _size == 0;
        }

        [[nodiscard]] constexpr auto cbegin() noexcept -> const_iterator {
            return get_data();
        }

        [[nodiscard]] constexpr auto cend() noexcept -> const_iterator {
            return get_data() + get_size(); // Pointer to last char
        }

        [[nodiscard]] constexpr auto sub_slice(size_type begin, size_type end) const noexcept -> self_type {
            #ifdef BUILD_DEBUG
            if (begin >= _size || end >= _size || end > begin) {
                throw std::runtime_error("Invalid begin or end index for string slice");
            }
            #endif
            return self_type(_data + begin, _data + end);
        }

        [[nodiscard]] constexpr auto operator [](size_type index) const noexcept -> value_type {
            #ifdef BUILD_DEBUG
            if (index >= _size) {
                throw std::runtime_error("Index out of bounds");
            }
            #endif

            return _data[index];
        }

        [[nodiscard]] constexpr auto operator ==(const self_type& other) const noexcept -> bool {
            if (_size != other._size) {
                return false;
            }

            for (size_type i = 0; i < _size; ++i) {
                if (_data[i] == other[i]) {
                    continue;
                }

                return false;
            }

            return true;
        }

        [[nodiscard]] constexpr auto operator !=(const self_type& other) const noexcept -> bool {
            return !(*this == other); // NOLINT
        }
    };

    using StringSlice = BasicStringSlice<char>;
    using WStringSlice = BasicStringSlice<wchar_t>;
    using String8Slice = BasicStringSlice<char8_t>;
    using String16Slice = BasicStringSlice<char16_t>;
    using String32Slice = BasicStringSlice<char32_t>;

    namespace string_literals {
        [[nodiscard]] constexpr auto operator ""_str(const char* data, usize size) noexcept -> StringSlice {
            return StringSlice(data, size);
        }

        [[nodiscard]] constexpr auto operator ""_str(const wchar_t* data, usize size) noexcept -> WStringSlice {
            return WStringSlice(data, size);
        }

        [[nodiscard]] constexpr auto operator ""_str(const char8_t* data, usize size) noexcept -> String8Slice {
            return String8Slice(data, size);
        }

        [[nodiscard]] constexpr auto operator ""_str(const char16_t* data, usize size) noexcept -> String16Slice {
            return String16Slice(data, size);
        }

        [[nodiscard]] constexpr auto operator ""_str(const char32_t* data, usize size) noexcept -> String32Slice {
            return String32Slice(data, size);
        }
    }
}