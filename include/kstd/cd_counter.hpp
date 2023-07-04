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
 * @since 04/07/2023
 */

#pragma once

#include <kstd/assert.hpp>
#include <kstd/defaults.hpp>
#include <kstd/types.hpp>

namespace kstd {
    class CDCounter;

    class CDCounterStats final {
        friend class CDCounter;

        atomic_usize _constructions;
        atomic_usize _destructions;

        public:
        KSTD_NO_MOVE_COPY(CDCounterStats)

        inline CDCounterStats() noexcept :
                _constructions(0),
                _destructions(0) {
        }

        ~CDCounterStats() noexcept = default;

        [[nodiscard]] inline auto get_constructions() const noexcept -> usize {
            return _constructions;
        }

        [[nodiscard]] inline auto get_destructions() const noexcept -> usize {
            return _destructions;
        }

        [[nodiscard]] inline auto get_spills() const noexcept -> usize {
            return _constructions - _destructions;
        }
    };

    class CDCounter final {
        CDCounterStats* _stats;

        public:
        inline CDCounter(CDCounterStats& stats) noexcept :
                _stats(&stats) {
        }

        inline CDCounter(const CDCounter& other) noexcept :
                _stats(other._stats) {
            ++(_stats->_constructions);
        }

        inline CDCounter(CDCounter&& other) noexcept :
                _stats(other._stats) {
            ++(_stats->_constructions);
        }

        ~CDCounter() noexcept {
            ++(_stats->_destructions);
        }

        inline auto operator=(const CDCounter&) noexcept -> CDCounter& = default;

        inline auto operator=(CDCounter&&) noexcept -> CDCounter& = default;

        [[nodiscard]] inline auto get_stats() const noexcept -> const CDCounterStats& {
            return *_stats;
        }
    };
}// namespace kstd