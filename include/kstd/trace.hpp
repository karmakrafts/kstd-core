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
 * @since 31/08/2023
 */

#pragma once

#include <stack>

#include "defaults.hpp"
#include "source_location.hpp"

#define KSTD_TRACE [[maybe_unused]] const kstd::StackTraceGuard __trace_guard = {}

namespace kstd {
    namespace {
        thread_local std::vector<SourceLocation> g_stack_trace;// NOLINT
    }

    struct StackTraceGuard final {
        KSTD_NO_MOVE_COPY(StackTraceGuard, StackTraceGuard, inline)

        inline StackTraceGuard(SourceLocation location = SourceLocation::current()) noexcept {
            g_stack_trace.push_back(std::move(location));
        }

        ~StackTraceGuard() noexcept {
            g_stack_trace.pop_back();
        }
    };

    [[nodiscard]] inline auto get_stack_trace() noexcept -> const std::vector<SourceLocation>& {
        return g_stack_trace;
    }
}// namespace kstd