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
 * @since 25/08/2023
 */

#pragma once

// NOLINTBEGIN
#define KSTD_EXPAND(...) __VA_ARGS__
#define KSTD_LITERAL(e) e
#define KSTD_UNPAREN(a) KSTD_LITERAL(KSTD_EXPAND a)
#define KSTD_TEMPLATE(t) template<KSTD_UNPAREN(t)>
// NOLINTEND

#define KSTD_DEFAULT_DELETER(n, f)                                                                                     \
    struct n final {                                                                                                   \
        constexpr auto operator()(auto* memory) noexcept {                                                             \
            f(memory);                                                                                                 \
        }                                                                                                              \
    };
