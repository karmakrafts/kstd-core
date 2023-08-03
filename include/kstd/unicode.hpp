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
 * @since 30/07/2023
 */

#pragma once

#include <iterator>
#include <string>

#include "libc.hpp"
#include "types.hpp"

/**
 * Implementation based on Boost Nowide
 * https://github.com/boostorg/nowide
 * made constexpr-ready and prettified some of the code.
 */
namespace kstd::unicode {
    using CodePoint = char32_t;

    constexpr CodePoint illegal = 0xFFFF'FFFFU;
    constexpr CodePoint incomplete = 0xFFFF'FFFEU;
    constexpr CodePoint replacement = 0x0000'FFFD;

    constexpr auto is_valid_codepoint(CodePoint value) noexcept -> bool {
        if(value > 0x10FFFF) {
            return false;
        }
        if(0xD800 <= value && value <= 0xDFFF) {
            return false;
        }
        return true;
    }

    template<typename CHAR, usize SIZE = sizeof(CHAR)>
    struct UTFTraits;

    template<typename CHAR>
    struct UTFTraits<CHAR, 1> {
        using CharType = CHAR;
        static constexpr i32 max_width = 4;

        [[nodiscard]] static constexpr auto get_trail_length(CharType value) noexcept -> i32 {
            const auto uc_value = static_cast<u8>(value);
            if(uc_value < 128) {
                return 0;
            }
            if(uc_value < 194) {
                return -1;
            }
            if(uc_value < 224) {
                return 1;
            }
            if(uc_value < 240) {
                return 2;
            }
            if(uc_value <= 244) {
                return 3;
            }
            return -1;
        }

        [[nodiscard]] static constexpr auto get_width(CodePoint value) noexcept -> i32 {
            if(value <= 0x7F) {
                return 1;
            }
            if(value <= 0x7FF) {
                return 2;
            }
            if(value <= 0xFFFF) {
                return 3;
            }
            return 4;
        }

        [[nodiscard]] static constexpr auto is_trail(CharType value) noexcept -> bool {
            return (static_cast<u8>(value) & 0xC0) == 0x80;
        }

        [[nodiscard]] static constexpr auto is_lead(CharType value) noexcept -> bool {
            return !is_trail(value);
        }

        template<typename ITERATOR>
        [[nodiscard]] static constexpr auto decode(ITERATOR& current, ITERATOR end) noexcept -> CodePoint {
            if(current == end) {
                return incomplete;
            }
            const auto lead = static_cast<u8>(*current++);
            const auto trail_size = get_trail_length(lead);
            if(trail_size < 0) {
                return illegal;
            }
            if(trail_size == 0) {
                return lead;
            }
            auto code_point = lead & ((1 << (6 - trail_size)) - 1);
            u8 temp = 0;
            for(int32_t remaining_size = trail_size; remaining_size > 0; --remaining_size) {
                if(current == end) {
                    return incomplete;
                }
                temp = static_cast<u8>(*current++);
                if(!is_trail(temp)) {
                    return illegal;
                }
                code_point = (code_point << 6) | (temp & 0x3F);
            }
            if(!is_valid_codepoint(code_point) || get_width(code_point) != trail_size + 1) {
                current -= trail_size;
                return illegal;
            }
            return code_point;
        }

        template<typename ITERATOR>
        [[nodiscard]] static constexpr auto decode_valid(ITERATOR& current) noexcept -> CodePoint {
            const auto lead = static_cast<u8>(*current++);
            if(lead < 192) {
                return lead;
            }
            i32 trail_size = 0;
            if(lead < 224) {
                trail_size = 1;
            }
            else if(lead < 240) {
                trail_size = 2;
            }
            else {
                trail_size = 3;
            }
            auto code_point = lead & ((1 << (6 - trail_size)) - 1);
            for(int32_t remaining_size = trail_size; remaining_size > 0; --remaining_size) {
                code_point = (code_point << 6) | (static_cast<u8>(*current++) & 0x3F);
            }
            return code_point;
        }

        template<typename ITERATOR>
        static constexpr auto encode(CodePoint value, ITERATOR& out) noexcept -> void {
            if(value <= 0x7F) {
                *out++ = static_cast<CharType>(value);
            }
            else if(value <= 0x7FF) {
                *out++ = static_cast<CharType>((value >> 6) | 0xC0);
                *out++ = static_cast<CharType>((value & 0x3F) | 0x80);
            }
            else if(value <= 0xFFFF) {
                *out++ = static_cast<CharType>((value >> 12) | 0xE0);
                *out++ = static_cast<CharType>(((value >> 6) & 0x3F) | 0x80);
                *out++ = static_cast<CharType>((value & 0x3F) | 0x80);
            }
            else {
                *out++ = static_cast<CharType>((value >> 18) | 0xF0);
                *out++ = static_cast<CharType>(((value >> 12) & 0x3F) | 0x80);
                *out++ = static_cast<CharType>(((value >> 6) & 0x3F) | 0x80);
                *out++ = static_cast<CharType>((value & 0x3F) | 0x80);
            }
        }
    };

    template<typename CHAR>
    struct UTFTraits<CHAR, 2> {
        using CharType = CHAR;
        static constexpr i32 max_width = 2;

        private:
        [[nodiscard]] static constexpr auto is_single_codepoint(u16 value) noexcept -> bool {
            return value <= 0xD7FF || value >= 0xE000;
        }

        [[nodiscard]] static constexpr auto is_first_surrogate(u16 value) noexcept -> bool {
            return 0xD800 <= value && value <= 0xDBFF;
        }

        [[nodiscard]] static constexpr auto is_second_surrogate(u16 value) noexcept -> bool {
            return 0xDC00 <= value && value <= 0xDFFF;
        }

        [[nodiscard]] static constexpr auto combine_surrogate(u16 surr1, u16 value2) noexcept -> CodePoint {
            return ((CodePoint(surr1 & 0x3FF) << 10) | (value2 & 0x3FF)) + 0x10000;
        }

        public:
        [[nodiscard]] static constexpr auto get_trail_length(CharType value) noexcept -> i32 {
            if(is_first_surrogate(value)) {
                return 1;
            }
            if(is_second_surrogate(value)) {
                return -1;
            }
            return 0;
        }

        [[nodiscard]] static constexpr auto get_width(CodePoint value) noexcept -> i32 {
            return value >= 0x10000 ? 2 : 1;
        }

        [[nodiscard]] static constexpr auto is_trail(CharType value) noexcept -> bool {
            return is_second_surrogate(value);
        }

        [[nodiscard]] static constexpr auto is_lead(CharType value) noexcept -> bool {
            return !is_second_surrogate(value);
        }

        template<typename ITERATOR>
        [[nodiscard]] static constexpr auto decode(ITERATOR& current, ITERATOR end) noexcept -> CodePoint {
            if(current == end) {
                return incomplete;
            }
            const auto surr1 = static_cast<u16>(*current++);
            if(is_single_codepoint(surr1)) {
                return surr1;
            }
            if(surr1 >= 0xDC00) {
                return illegal;
            }
            if(current == end) {
                return incomplete;
            }
            const auto surr2 = static_cast<u16>(*current++);
            if(!is_second_surrogate(surr2)) {
                return illegal;
            }
            return combine_surrogate(surr1, surr2);
        }

        template<typename ITERATOR>
        [[nodiscard]] static constexpr auto decode_valid(ITERATOR& current) noexcept -> CodePoint {
            const auto surr1 = static_cast<u16>(*current++);
            if(is_single_codepoint(surr1)) {
                return surr1;
            }
            const auto surr2 = static_cast<u16>(*current++);
            return combine_surrogate(surr1, surr2);
        }

        template<typename ITERATOR>
        static constexpr auto encode(CodePoint value, ITERATOR& out) noexcept -> void {
            if(value <= 0xFFFF) {
                *out++ = static_cast<CharType>(value);
            }
            else {
                value -= 0x10000;
                *out++ = static_cast<CharType>(0xD800 | (value >> 10));
                *out++ = static_cast<CharType>(0xDC00 | (value & 0x3FF));
            }
        }
    };

    template<typename CHAR>
    struct UTFTraits<CHAR, 4> {
        using CharType = CHAR;
        static constexpr i32 max_width = 1;

        [[nodiscard]] static constexpr auto get_trail_length(CharType value) noexcept -> i32 {
            if(is_valid_codepoint(value)) {
                return 0;
            }
            return -1;
        }

        [[nodiscard]] static constexpr auto get_width([[maybe_unused]] CodePoint value) noexcept -> i32 {
            return 1;
        }

        [[nodiscard]] static constexpr auto is_trail([[maybe_unused]] CodePoint value) noexcept -> bool {
            return false;
        }

        [[nodiscard]] static constexpr auto is_lead([[maybe_unused]] CodePoint value) noexcept -> bool {
            return true;
        }

        template<typename ITERATOR>
        [[nodiscard]] static constexpr auto decode(ITERATOR& current, ITERATOR end) noexcept -> CodePoint {
            if(current == end) {
                return incomplete;
            }
            const auto code_point = static_cast<CodePoint>(*current++);
            if(!is_valid_codepoint(code_point)) {
                return illegal;
            }
            return code_point;
        }

        template<typename ITERATOR>
        [[nodiscard]] static constexpr auto decode_valid(ITERATOR& current) noexcept -> CodePoint {
            return static_cast<CodePoint>(*current++);
        }

        template<typename ITERATOR>
        static constexpr auto encode(CodePoint value, ITERATOR& out) noexcept -> void {
            *out++ = static_cast<CharType>(value);
        }
    };

    template<typename CHAR_OUT, typename CHAR_IN>
    inline auto convert_buffer(const CHAR_IN* value, CHAR_OUT* out, usize* out_count) noexcept -> void {
        if(value == nullptr) {
            return;
        }
        const auto length = libc::get_string_length(value);
        if(length == 0) {
            return;
        }
        const auto* end = &value[length];
        if(out_count != nullptr) {
            const auto* current = value;
            while(current != end) {// NOLINT
                auto code_point = UTFTraits<CHAR_IN>::decode(current, end);
                if(code_point == illegal || code_point == incomplete) {
                    code_point = replacement;
                }
                *out_count += UTFTraits<CHAR_OUT>::get_width(code_point);// NOLINT
            }
        }
        if(out != nullptr) {
            const auto* current = value;
            while(current != end) {// NOLINT
                auto code_point = UTFTraits<CHAR_IN>::decode(current, end);
                if(code_point == illegal || code_point == incomplete) {
                    code_point = replacement;
                }
                UTFTraits<CHAR_OUT>::encode(code_point, out);// NOLINT
            }
        }
    }

    template<typename CHAR_OUT, typename... OUT_ARGS, typename CHAR_IN, typename... IN_ARGS>
    [[nodiscard]] inline auto convert(const std::basic_string<CHAR_IN, IN_ARGS...>& value) noexcept
            -> std::basic_string<CHAR_OUT, OUT_ARGS...> {
        const auto* value_address = value.c_str();
        usize count = 0;
        convert_buffer<CHAR_OUT, CHAR_IN>(value_address, nullptr, &count);
        std::basic_string<CHAR_OUT, OUT_ARGS...> result(count, ' ');
        convert_buffer<CHAR_OUT, CHAR_IN>(value_address, result.data(), nullptr);
        return result;
    }
}// namespace kstd::unicode