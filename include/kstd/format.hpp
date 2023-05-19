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
 * @since 19/05/2023
 */

#pragma once

#include "meta.hpp"
#include "string.hpp"
#include "string_slice.hpp"

namespace kstd {
    template<typename ALLOCATOR = Allocator<char>, typename... ARGS>
    [[nodiscard]] constexpr auto format(StringSlice format, ARGS&& ... args) noexcept -> String<ALLOCATOR> {
        String<ALLOCATOR> result;

        return result;
    }
}