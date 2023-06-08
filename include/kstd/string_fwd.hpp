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
 * @since 21/05/2023
 */

#pragma once

#include "allocator.hpp"

namespace kstd {
    template<typename CHAR>
    struct BasicStringSlice;

    template<typename CHAR, typename ALLOCATOR = Allocator<CHAR>>
    struct BasicHeapString;

    template<typename CHAR, usize SIZE = sizeof(BasicHeapString<CHAR, Allocator<CHAR>>) / sizeof(CHAR)>
    struct BasicSmallString;
}// namespace kstd