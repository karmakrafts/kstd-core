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

#include "string_fwd.hpp"
#include "types.hpp"
#include <cstring>
#include <string_view>

namespace kstd {
    template<typename CHAR> //
    KSTD_REQUIRES(concepts::Char<CHAR>)
    struct BasicStringSlice final {
        using self_type = BasicStringSlice<CHAR>;
        using value_type = CHAR;
        using const_pointer = const value_type*;
        using size_type = usize;
        using const_iterator = const_pointer;
        using view_type = std::basic_string_view<value_type>;

        private:

        const_pointer _data;
        size_type _size;

        public:

        constexpr BasicStringSlice() noexcept :
                _data(nullptr),
                _size(0) {
        }

        constexpr BasicStringSlice(const_pointer data, size_type size) noexcept :
                _data(data),
                _size(size) {
        }

        constexpr BasicStringSlice(view_type view) noexcept : // NOLINT
                _data(view.data()),
                _size(view.size()) {
        }

        constexpr BasicStringSlice(const self_type& other) noexcept = default;

        constexpr BasicStringSlice(self_type&& other) noexcept = default;

        constexpr auto operator =(const self_type& other) noexcept -> self_type& = default;

        constexpr auto operator =(self_type&& other) noexcept -> self_type& = default;

        [[nodiscard]] constexpr auto to_view() const noexcept -> view_type {
            return {_data, static_cast<typename view_type::size_type>(_size)};
        }

        [[nodiscard]] constexpr operator view_type() const noexcept { // NOLINT: allow implicit conversion to std::basic_string_view<>
            return to_view();
        }

        template<typename ALLOCATOR = std::allocator<value_type>>
        KSTD_REQUIRES(concepts::Allocator<ALLOCATOR>)
        [[nodiscard]] constexpr auto to_owning() const noexcept -> BasicString<value_type, ALLOCATOR> {
            BasicString<value_type, ALLOCATOR> result;
            result.reserve(_size + 1);
            std::memcpy(result.get_data(), _data, _size);
            result[_size] = static_cast<value_type>(0); // Insert null-terminator
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

    #ifdef KSTD_SIZED_CHAR_TYPES

    using String8Slice = BasicStringSlice<char8_t>;
    using String16Slice = BasicStringSlice<char16_t>;
    using String32Slice = BasicStringSlice<char32_t>;

    #endif

    namespace string_literals {
        [[nodiscard]] constexpr auto operator ""_str(const char* data, usize size) noexcept -> StringSlice {
            return {data, size};
        }

        [[nodiscard]] constexpr auto operator ""_str(const wchar_t* data, usize size) noexcept -> WStringSlice {
            return {data, size};
        }

        #ifdef KSTD_SIZED_CHAR_TYPES

        [[nodiscard]] constexpr auto operator ""_str(const char8_t* data, usize size) noexcept -> String8Slice {
            return {data, size};
        }

        [[nodiscard]] constexpr auto operator ""_str(const char16_t* data, usize size) noexcept -> String16Slice {
            return {data, size};
        }

        [[nodiscard]] constexpr auto operator ""_str(const char32_t* data, usize size) noexcept -> String32Slice {
            return {data, size};
        }

        #endif // KSTD_SIZED_CHAR_TYPES
    }
}