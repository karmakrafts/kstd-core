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

// C89/99 says these should be macros, but we don't want that in C++..
#undef stderr
#undef stdout
#undef stdin

#if defined(PLATFORM_LINUX)
extern FILE* stdout; // NOLINT
extern FILE* stderr; // NOLINT
extern FILE* stdin; // NOLINT
#elif defined(PLATFORM_APPLE)
extern FILE* __stdoutp;
extern FILE* __stderrp;
extern FILE* __stdinp;
#else
extern _iobuf* stdout;
extern _iobuf* stderr;
extern _iobuf* stdin;
#endif

namespace kstd::libc {
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

    using std::wscanf;
    using std::wprintf;
    using std::fwprintf;

    #if defined(PLATFORM_LINUX) || defined(PLATFORM_WINDOWS)
    using ::stdout;
    using ::stderr;
    using ::stdin;
    #else
    using stdout = ::__stdoutp;
    using stderr = ::__stderrp;
    using stdin = ::__stdinp;
    #endif
}