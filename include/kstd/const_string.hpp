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
 * @since 21/06/2023
 */

#pragma once

#include "defaults.hpp"
#include "small_string.hpp"
#include "string_slice.hpp"
#include "types.hpp"

namespace kstd {
    template<typename T, usize LENGTH>
    struct BasicConstString {
        using ValueType = T;

        static constexpr usize length = LENGTH;
        static constexpr usize capacity = length + 1;

        ValueType data[capacity] = {};// NOLINT

        KSTD_DEFAULT_MOVE_COPY(BasicConstString)

        constexpr BasicConstString(const ValueType (&data_)[length]) noexcept {// NOLINT
            for(usize i = 0; i < length; ++i) {
                data[i] = data_[i];
            }
        }

        ~BasicConstString() noexcept = default;

        [[nodiscard]] constexpr auto to_slice() const noexcept -> BasicStringSlice<ValueType> {
            return BasicStringSlice<ValueType>(static_cast<const ValueType*>(data), length);
        }

        [[nodiscard]] constexpr auto get_length() const noexcept -> usize {
            return length;
        }

        template<usize OTHER_LENGTH>
        [[nodiscard]] constexpr auto operator==(const BasicConstString<ValueType, OTHER_LENGTH>& other) const noexcept
                -> bool {
            if constexpr(length != OTHER_LENGTH) {
                return false;
            }
            for(usize i = 0; i < length; ++i) {
                if(other.data[i] == data[i]) {
                    continue;
                }
                return false;
            }
            return true;
        }

        template<usize OTHER_LENGTH>
        [[nodiscard]] constexpr auto operator!=(const BasicConstString<ValueType, OTHER_LENGTH>& other) const noexcept
                -> bool {
            return !(*this == other);// NOLINT
        }
    };

    template<usize LENGTH>
    struct ConstString final : public BasicConstString<char, LENGTH> {
        constexpr ConstString(const char (&data_)[LENGTH]) noexcept :// NOLINT
                BasicConstString<char, LENGTH>(data_) {
        }
    };

    template<usize LENGTH>
    struct ConstWString final : public BasicConstString<wchar_t, LENGTH> {
        constexpr ConstWString(const wchar_t (&data_)[LENGTH]) noexcept :// NOLINT
                BasicConstString<wchar_t, LENGTH>(data_) {
        }
    };
}// namespace kstd