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

#include "string_concepts.hpp"
#include "kstd/concepts.hpp"
#include "types.hpp"

#if __cplusplus >= 202002L
    #define KSTD_SIZED_CHAR_TYPES // Check support for sized char types
#endif

namespace kstd {
    template<typename CHAR> //
    KSTD_REQUIRES(concepts::Char<CHAR>)
    struct BasicStringSlice;

    template<typename CHAR, usize SIZE = 24 / sizeof(CHAR)> //
    KSTD_REQUIRES(concepts::Char<CHAR>)
    union BasicSmallString;

    template<typename CHAR, typename ALLOCATOR = std::allocator<CHAR>> //
    KSTD_REQUIRES(concepts::Char<CHAR> && concepts::Allocator<ALLOCATOR>)
    union BasicString;
}