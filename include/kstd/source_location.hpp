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

#include <fmt/format.h>

#include "defaults.hpp"
#include "hash.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace kstd {
    class SourceLocation final {
        const char* _file;
        const char* _function;
        usize _line;

        public:
        friend struct std::hash<SourceLocation>;

        KSTD_DEFAULT_MOVE_COPY(SourceLocation, SourceLocation, constexpr)

        constexpr SourceLocation(const char* file, const char* function, usize line) noexcept :
                _file {file},
                _function {function},
                _line {line} {
        }

        ~SourceLocation() noexcept = default;

        [[nodiscard]] static constexpr auto current(const char* file = __builtin_FILE(),
                                                    const char* function = __builtin_FUNCTION(),
                                                    usize line = __builtin_LINE()) noexcept -> SourceLocation {
            return {file, function, line};
        }

        [[nodiscard]] constexpr auto get_file() const noexcept -> const char* {
            return _file;
        }

        [[nodiscard]] constexpr auto get_function() const noexcept -> const char* {
            return _function;
        }

        [[nodiscard]] constexpr auto get_line() const noexcept -> usize {
            return _line;
        }

        [[nodiscard]] inline auto to_string() const noexcept -> std::string {
            return fmt::format("{}:{} [{}]", _file, _line, _function);
        }
    };
}// namespace kstd

KSTD_DEFAULT_HASH((kstd::SourceLocation), value._file, value._function, value._line)