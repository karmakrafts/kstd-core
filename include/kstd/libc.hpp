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
 * @since 16/05/2023
 */

#pragma once

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cwchar>

#include "meta.hpp"
#include "types.hpp"
#include "math.hpp"

namespace kstd::libc {
    namespace iob {
        // @formatter:off
        #if defined(PLATFORM_LINUX)
        inline auto out = stdout;
        inline auto err = stderr;
        inline auto in = stdin;
        #elif defined(PLATFORM_WINDOWS)
        inline auto out = ::__acrt_iob_func(1);
        inline auto err = ::__acrt_iob_func(2);
        inline auto in = ::__acrt_iob_func(0);
        #else
            #ifdef __DARWIN_UNIX03
            inline auto out = ::__stdoutp;
            inline auto err = ::__stderrp;
            inline auto in = ::__stdinp;
            #else
            inline auto out = &::__sF[1];
            inline auto err = &::__sF[2];
            inline auto in = &::__sF[0];
            #endif
        #endif
        // @formatter:on
    }

    using std::exit;

    using std::malloc;
    using std::realloc;
    using std::calloc;
    using std::free;

    using std::memset;
    using std::memcpy;
    using std::memmove;
    using std::memchr;
    using std::memcmp;

    using std::strlen;
    using std::strcmp;
    using std::strncmp;
    using std::strcat;
    using std::strcpy;
    using std::strncpy;
    using std::strchr;
    using std::strrchr;
    using std::strstr;
    using std::strtok;
    using std::strxfrm;

    using std::wcslen;
    using std::wcscmp;
    using std::wcsncmp;
    using std::wcscat;
    using std::wcscpy;
    using std::wcsncpy;
    using std::wcschr;
    using std::wcsrchr;
    using std::wcsstr;
    using std::wcstok;
    using std::wcsxfrm;

    using std::scanf;
    using std::printf;
    using std::fprintf;
    using std::sprintf;

    using std::wscanf;
    using std::wprintf;
    using std::fwprintf;
    using std::swprintf;

    template<typename T>
    [[nodiscard]] constexpr auto get_string_length(const T* str) noexcept -> usize {
        if constexpr (meta::is_same<T, char>) {
            return strlen(str);
        }
        else if constexpr (meta::is_same<T, wchar_t>) {
            return wcslen(str);
        }
        else {
            auto* ptr = str;

            while (*ptr != static_cast<T>('\0')) {
                ++ptr;
            }

            return static_cast<usize>(ptr - str); // Use pointer difference
        }
    }

    template<typename T>
    constexpr auto copy_string(T* dst, const T* src) noexcept -> void {
        if constexpr (meta::is_same<T, char>) {
            return strcpy(dst, src);
        }
        else if constexpr (meta::is_same<T, wchar_t>) {
            return wcscpy(dst, src);
        }
        else {
            const auto dst_length = get_string_length(dst);
            const auto src_length = get_string_length(src);
            const auto length = min(dst_length, src_length);
            memcpy(dst, src, (length + 1) * sizeof(T));
        }
    }

    template<typename T>
    constexpr auto compare_string(const T* a, const T* b) noexcept -> i32 {
        if constexpr (meta::is_same<T, char>) {
            return strcmp(a, b);
        }
        else if constexpr (meta::is_same<T, wchar_t>) {
            return wcscmp(a, b);
        }
        else {
            const auto a_length = get_string_length(a);
            const auto b_length = get_string_length(b);

            if (a_length != b_length) {
                return static_cast<i32>(b_length - a_length); // Narrowing conversion -.-
            }

            return memcmp(a, b, a_length);
        }
    }

    template<typename T>
    constexpr auto compare_string(const T* a, const T* b, usize count) noexcept -> i32 {
        if constexpr (meta::is_same<T, char>) {
            return strncmp(a, b, count);
        }
        else if constexpr (meta::is_same<T, wchar_t>) {
            return wcsncmp(a, b, count);
        }
        else {
            const auto a_length = get_string_length(a);
            const auto b_length = get_string_length(b);

            if (a_length != b_length) {
                return static_cast<i32>(b_length - a_length); // Narrowing conversion -.-
            }

            return memcmp(a, b, a_length);
        }
    }

    template<typename T>
    constexpr auto concat_string(T* dst, const T* src) noexcept -> T* {
        if constexpr (meta::is_same<T, char>) {
            return strcat(dst, src);
        }
        else if constexpr (meta::is_same<T, wchar_t>) {
            return wcscat(dst, src);
        }
        else {
            memcpy(dst + get_string_length(dst), src, get_string_length(src) + 1);
            return dst;
        }
    }

    template<typename T>
    constexpr auto zero(T* ptr) noexcept -> void {
        memset(ptr, 0, sizeof(T));
    }

    template<typename T>
    constexpr auto zero(T& ptr) noexcept -> void {
        memset(&ptr, 0, sizeof(T));
    }
}