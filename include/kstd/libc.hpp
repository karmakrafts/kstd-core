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

#include <stdlib.h> // NOLINT
#include <string.h> // NOLINT
#include <wchar.h> // NOLINT

#undef stderr
#undef stdout
#undef stdin

namespace kstd::libc {
    using ::exit;

    using ::malloc;
    using ::realloc;
    using ::calloc;
    using ::free;

    using ::memset;
    using ::memcpy;
    using ::memmove;
    using ::memchr;
    using ::memcmp;

    using ::strlen;
    using ::strnlen;
    using ::strcmp;
    using ::strncmp;
    using ::strcat;
    using ::strcpy;
    using ::strncpy;
    using ::strchr;
    using ::strrchr;
    using ::strstr;
    using ::strcasecmp;
    using ::strncasecmp;
    using ::strtok;
    using ::strxfrm;

    using ::wcslen;
    using ::wcsnlen;
    using ::wcscmp;
    using ::wcsncmp;
    using ::wcscat;
    using ::wcscpy;
    using ::wcsncpy;
    using ::wcschr;
    using ::wcsrchr;
    using ::wcsstr;
    using ::wcscasecmp;
    using ::wcsncasecmp;
    using ::wcstok;
    using ::wcsxfrm;

    using ::scanf;
    using ::printf;
    using ::fprintf;

    using ::wscanf;
    using ::wprintf;
    using ::fwprintf;

    using ::stdout;
    using ::stderr;
    using ::stdin;
}