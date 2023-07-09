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
 * @since 07/06/2023
 */

#pragma once

#include "defaults.hpp"

// clang-format off
// NOLINTBEGIN
#define KSTD_DEFAULT_MOVE(n, t, ...) __VA_ARGS__ n(t&&) noexcept = default; \
__VA_ARGS__ auto operator =(t&&) noexcept -> t& = default;

#define KSTD_DEFAULT_COPY(n, t, ...) __VA_ARGS__ n(const t&) noexcept = default; \
__VA_ARGS__ auto operator =(const t&) noexcept -> t& = default;

#define KSTD_DEFAULT_MOVE_COPY(n, t, ...) KSTD_DEFAULT_MOVE(n, t, __VA_ARGS__) \
KSTD_DEFAULT_COPY(n, t, __VA_ARGS__)

#define KSTD_NO_MOVE(n, t, ...) __VA_ARGS__ n(t&&) noexcept = delete; \
__VA_ARGS__ auto operator =(t&&) noexcept -> t& = delete;

#define KSTD_NO_COPY(n, t, ...) __VA_ARGS__ n(const t&) noexcept = delete; \
__VA_ARGS__ auto operator =(const t&) noexcept -> t& = delete;

#define KSTD_NO_MOVE_COPY(n, t, ...) KSTD_NO_MOVE(n, t, __VA_ARGS__) \
KSTD_NO_COPY(n, t, __VA_ARGS__)
// NOLINTEND
// clang-format on