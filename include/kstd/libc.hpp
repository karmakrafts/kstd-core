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

#include <algorithm>
#include <memory>
#include <type_traits>

#include "stdio.h" // NOLINT
#include "stdlib.h"// NOLINT
#include "string.h"// NOLINT
#include "wchar.h" // NOLINT

#include "macros.hpp"
#include "types.hpp"

namespace kstd::libc {
    namespace iob {
        // NOLINTBEGIN
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
        // NOLINTEND
    }// namespace iob

    using File = ::FILE;

    using ::exit;

    using ::calloc;
    using ::free;
    using ::malloc;
    using ::realloc;

    using ::memchr;
    using ::memcmp;
    using ::memcpy;
    using ::memmove;
    using ::memset;

    using ::strcat;
    using ::strchr;
    using ::strcmp;
    using ::strcpy;
    using ::strlen;
    using ::strncmp;
    using ::strncpy;
    using ::strrchr;
    using ::strstr;
    using ::strtok;
    using ::strxfrm;

    using ::wcscat;
    using ::wcschr;
    using ::wcscmp;
    using ::wcscpy;
    using ::wcslen;
    using ::wcsncmp;
    using ::wcsncpy;
    using ::wcsrchr;
    using ::wcsstr;
    using ::wcstok;
    using ::wcsxfrm;

    template<typename... ARGS>
    constexpr auto scanf(const char* fmt, ARGS&&... args) noexcept -> i32 {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 2 parameters!");
        return ::scanf(fmt, std::forward<ARGS>(args)...);// NOLINT
    }

    template<typename... ARGS>
    constexpr auto fprintf(File* file, const char* format, ARGS&&... args) noexcept -> i32 {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 3 parameters!");
        return ::fprintf(file, format, std::forward<ARGS>(args)...);// NOLINT
    }

    template<typename... ARGS>
    constexpr auto printf(const char* format, ARGS&&... args) noexcept -> i32 {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 2 parameters!");
        return ::printf(format, std::forward<ARGS>(args)...);// NOLINT
    }

    template<typename... ARGS>
    constexpr auto sprintf(char* buffer, const char* format, ARGS&&... args) noexcept -> i32 {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 3 parameters!");
        return ::sprintf(buffer, format, std::forward<ARGS>(args)...);// NOLINT
    }

    template<typename... ARGS>
    constexpr auto swprintf(const wchar_t* buffer, const wchar_t* format, ARGS&&... args) noexcept -> i32 {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 3 parameters!");
        return ::swprintf(buffer, format, std::forward<ARGS>(args)...);// NOLINT
    }

    template<typename... ARGS>
    constexpr auto fwprintf(File* file, const wchar_t* format, ARGS&&... args) noexcept -> i32 {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 3 parameters!");
        return ::fwprintf(file, format, std::forward<ARGS>(args)...);// NOLINT
    }

    template<typename... ARGS>
    constexpr auto wprintf(const wchar_t* format, ARGS&&... args) noexcept -> i32 {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 2 parameters!");
        return ::wprintf(format, std::forward<ARGS>(args)...);// NOLINT
    }

    template<typename... ARGS>
    constexpr auto wscanf(wchar_t* buffer, const wchar_t* format, ARGS&&... args) noexcept -> i32 {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 3 parameters!");
        return ::wscanf(buffer, format, std::forward<ARGS>(args)...);// NOLINT
    }

    // Strings
    template<typename T>
    [[nodiscard]] constexpr auto get_string_length(const T* str) noexcept -> usize {
        if constexpr(std::is_same_v<T, char>) {
            return strlen(str);
        }
        else if constexpr(std::is_same_v<T, wchar_t>) {
            return wcslen(str);
        }
        else {
            auto* ptr = str;
            while(*ptr != static_cast<T>('\0')) {
                ++ptr;
            }
            return static_cast<usize>(ptr - str);// Use pointer difference
        }
    }

    template<typename T>
    constexpr auto copy_string(T* dst, const T* src) noexcept -> void {
        if constexpr(std::is_same_v<T, char>) {
            strcpy(dst, src);
        }
        else if constexpr(std::is_same_v<T, wchar_t>) {
            wcscpy(dst, src);
        }
        else {
            const auto dst_length = get_string_length(dst);
            const auto src_length = get_string_length(src);
            const auto length = std::min(dst_length, src_length);
            memcpy(dst, src, (length + 1) * sizeof(T));
        }
    }

    template<typename T>
    constexpr auto compare_string(const T* lhs, const T* rhs) noexcept -> i32 {
        if constexpr(std::is_same_v<T, char>) {
            return strcmp(lhs, rhs);
        }
        else if constexpr(std::is_same_v<T, wchar_t>) {
            return wcscmp(lhs, rhs);
        }
        else {
            const auto a_length = get_string_length(lhs);
            const auto b_length = get_string_length(rhs);

            if(a_length != b_length) {
                return static_cast<i32>(b_length - a_length);// Narrowing conversion -.-
            }

            return memcmp(lhs, rhs, a_length);
        }
    }

    template<typename T>
    constexpr auto concat_string(T* dst, const T* src) noexcept -> T* {
        if constexpr(std::is_same_v<T, char>) {
            return strcat(dst, src);
        }
        else if constexpr(std::is_same_v<T, wchar_t>) {
            return wcscat(dst, src);
        }
        else {
            memcpy(dst + get_string_length(dst), src, get_string_length(src) + 1);
            return dst;
        }
    }

    KSTD_DEFAULT_DELETER(FreeDeleter, free)
}// namespace kstd::libc