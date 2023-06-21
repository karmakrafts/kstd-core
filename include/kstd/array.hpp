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
#include "libc.hpp"
#include "types.hpp"

namespace kstd {
    template<typename T, usize SIZE>
    class Array final {
        public:
        static constexpr usize size = SIZE;

        using ElementType [[maybe_unused]] = T;
        using Self [[maybe_unused]] = Array<T, SIZE>;
        using Pointer [[maybe_unused]] = T*;
        using ConstPointer [[maybe_unused]] = const T*;

        private:
        ElementType _data[size];// NOLINT

        public:
        KSTD_DEFAULT_MOVE_COPY(Array)

        constexpr Array() noexcept {                                            // NOLINT
            libc::memset(reinterpret_cast<Pointer>(_data), 0, sizeof(T) * size);// NOLINT
        }

        ~Array() noexcept = default;

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            return size;
        }

        [[nodiscard]] constexpr auto get_data() noexcept -> Pointer {
            return static_cast<Pointer>(_data);// NOLINT
        }

        [[nodiscard]] constexpr auto get_data() const noexcept -> ConstPointer {
            return static_cast<ConstPointer>(_data);// NOLINT
        }
    };
}// namespace kstd