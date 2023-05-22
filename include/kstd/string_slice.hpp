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

#include "libc.hpp"
#include "types.hpp"
#include "allocator.hpp"
#include "assert.hpp"
#include "string_fwd.hpp"

namespace kstd {
    template<typename CHAR> //
    struct BasicStringSlice final {
        using Self = BasicStringSlice<CHAR>;
        using ValueType = CHAR;
        using ConstPointer = const ValueType*;
        using SizeType = usize;

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

        constexpr BasicStringSlice(ConstPointer data) noexcept : // NOLINT
                _data(data),
                _size(libc::get_string_length(data)) {
        }

        constexpr BasicStringSlice(const Self& other) noexcept = default;

        constexpr BasicStringSlice(Self&& other) noexcept = default;

        ~BasicStringSlice() noexcept = default;

        constexpr auto operator =(const Self& other) noexcept -> Self& = default;

        constexpr auto operator =(Self&& other) noexcept -> Self& = default;

        [[nodiscard]] constexpr auto get_data() const noexcept -> ConstPointer {
            return _data;
        }

        [[nodiscard]] constexpr auto get_size() const noexcept -> SizeType {
            return _size;
        }

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return _size == 0;
        }

        [[nodiscard]] constexpr auto slice(SizeType begin, SizeType end) const noexcept -> Self {
            assert_false(begin >= _size || end >= _size || end > begin);
            return Self(_data + begin, _data + end);
        }

        [[nodiscard]] constexpr auto operator [](SizeType index) const noexcept -> ValueType {
            assert_false(index >= _size);
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

    namespace string_literals {
        [[nodiscard]] constexpr auto operator ""_str(const char* data, usize size) noexcept -> StringSlice {
            return {data, size};
        }

        [[nodiscard]] constexpr auto operator ""_str(const wchar_t* data, usize size) noexcept -> WStringSlice {
            return {data, size};
        }
    }
}