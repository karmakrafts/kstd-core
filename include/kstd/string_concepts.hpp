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

#include "kstd/concepts.hpp"

namespace kstd::concepts {
    #ifdef KSTD_CONCEPTS_AVAILABLE
    template<typename T> concept Char8 = std::integral<T> && sizeof(T) == 1;
    template<typename T> concept Char16 = std::integral<T> && sizeof(T) == 2;
    template<typename T> concept Char32 = std::integral<T> && sizeof(T) == 4;
    template<typename T> concept Char = Char8<T> || Char16<T> || Char32<T>;
    #endif // KSTD_CONCEPTS_AVAILABLE
}