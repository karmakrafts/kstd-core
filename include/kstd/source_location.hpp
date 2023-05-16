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

#include "string_slice.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace kstd {
    class SourceLocation final {
        StringSlice _file;
        StringSlice _function;
        usize _line;

        public:

        constexpr SourceLocation(StringSlice file, StringSlice function, usize line) noexcept :
                _file(move(file)),
                _function(move(function)),
                _line(line) {
        }

        ~SourceLocation() noexcept = default;

        [[nodiscard]] constexpr auto get_file() const noexcept -> const StringSlice& {
            return _file;
        }

        [[nodiscard]] constexpr auto get_function() const noexcept -> const StringSlice& {
            return _function;
        }

        [[nodiscard]] constexpr auto get_line() const noexcept -> usize {
            return _line;
        }
    };

    // @formatter:off
    [[nodiscard]] constexpr auto current_location(StringSlice file = __builtin_FILE(),
                                                  StringSlice function = __builtin_FUNCTION(),
                                                  usize line = __builtin_LINE()) noexcept -> SourceLocation {
        return {move(file), move(function), line};
    }
    // @formatter:on
}