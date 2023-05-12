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
#include "allocator.hpp"
#include <cstring>

#ifdef KSTD_STD_STRING_VIEW_SUPPORT
    #include <string_view>
#endif

namespace kstd {
    template<typename CHAR> //
    KSTD_REQUIRES(concepts::Char<CHAR>)
    struct BasicStringSlice final {
        using Self = BasicStringSlice<CHAR>;
        using ValueType = CHAR;
        using ConstPointer = const ValueType*;
        using SizeType = usize;
        using ConstIterator = ConstPointer;

        #ifdef KSTD_STD_STRING_VIEW_SUPPORT
        using std_type = std::basic_string_view<value_type>;
        #endif

        private:

        ConstPointer _data;
        SizeType _size;

        public:

        constexpr BasicStringSlice() noexcept :
                _data(nullptr),
                _size(0) {
        }

        constexpr BasicStringSlice(ConstPointer data, SizeType size) noexcept :
                _data(data),
                _size(size) {
        }

        #ifdef KSTD_STD_STRING_VIEW_SUPPORT
        constexpr BasicStringSlice(std_type view) noexcept : // NOLINT
                _data(view.data()),
                _size(view.size()) {
        }
        #endif

        constexpr BasicStringSlice(const Self& other) noexcept = default;

        constexpr BasicStringSlice(Self&& other) noexcept = default;

        ~BasicStringSlice() noexcept = default;

        constexpr auto operator =(const Self& other) noexcept -> Self& = default;

        constexpr auto operator =(Self&& other) noexcept -> Self& = default;

        #ifdef KSTD_STD_STRING_VIEW_SUPPORT
        [[nodiscard]] constexpr auto to_view() const noexcept -> std_type {
            return {_data, static_cast<typename std_type::size_type>(_size)};
        }

        [[nodiscard]] constexpr operator std_type() const noexcept { // NOLINT: allow implicit conversion to std::basic_string_view<>
            return to_view();
        }
        #endif

        template<typename ALLOCATOR = DefaultAllocator<ValueType>>
        KSTD_REQUIRES(concepts::Allocator<ALLOCATOR>)
        [[nodiscard]] constexpr auto to_owning() const noexcept -> BasicString<ValueType, ALLOCATOR> {
            BasicString<ValueType, ALLOCATOR> result;
            result.reserve(_size + 1);
            std::memcpy(result.get_data(), _data, _size);
            result[_size] = static_cast<ValueType>(0); // Insert null-terminator
            return result;
        }

        [[nodiscard]] constexpr auto get_data() const noexcept -> ConstPointer {
            return _data;
        }

        [[nodiscard]] constexpr auto get_size() const noexcept -> SizeType {
            return _size;
        }

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _size == 0;
        }

        [[nodiscard]] constexpr auto cbegin() noexcept -> ConstIterator {
            return get_data();
        }

        [[nodiscard]] constexpr auto cend() noexcept -> ConstIterator {
            return get_data() + get_size(); // Pointer to last char
        }

        [[nodiscard]] constexpr auto sub_slice(SizeType begin, SizeType end) const noexcept -> Self {
            #ifdef BUILD_DEBUG
            if (begin >= _size || end >= _size || end > begin) {
                throw std::runtime_error("Invalid begin or end index for string slice");
            }
            #endif
            return Self(_data + begin, _data + end);
        }

        [[nodiscard]] constexpr auto operator [](SizeType index) const noexcept -> ValueType {
            #ifdef BUILD_DEBUG
            if (index >= _size) {
                throw std::runtime_error("Index out of bounds");
            }
            #endif

            return _data[index];
        }

        [[nodiscard]] constexpr auto operator ==(const Self& other) const noexcept -> bool {
            if (_size != other._size) {
                return false;
            }

            for (SizeType i = 0; i < _size; ++i) {
                if (_data[i] == other[i]) {
                    continue;
                }

                return false;
            }

            return true;
        }

        [[nodiscard]] constexpr auto operator !=(const Self& other) const noexcept -> bool {
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