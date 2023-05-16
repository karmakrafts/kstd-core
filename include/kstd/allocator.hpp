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
 * @since 12/05/2023
 */

#pragma once

#include "libc.hpp"
#include "types.hpp"
#include "meta.hpp"
#include "utils.hpp"
#include "assert.hpp"

/*
 * Allows for the easy customization of the default memory allocator used by the kstd library.
 * Simple define your own versions of these macros before including any kstd headers, and they'll
 * automatically use your custom memory allocator implementation.
 */
#ifndef KSTD_MEMORY_ALLOC_FN
    #define KSTD_MEMORY_ALLOC_FN(s) kstd::libc::malloc(s)
#endif

#ifndef KSTD_MEMORY_FREE_FN
    #define KSTD_MEMORY_FREE_FN(p) kstd::libc::free(p)
#endif

namespace kstd {
    template<typename T, typename IMPL>
    struct BasicAllocator {
        using ValueType = T;

        [[nodiscard]] constexpr auto get_self() noexcept -> IMPL& {
            return static_cast<IMPL&>(*this);
        }

        template<typename... ARGS>
        [[nodiscard]] constexpr auto construct(ARGS&& ... args) noexcept -> T* {
            auto* memory = get_self().allocate(1);
            assert_true(memory != nullptr, "Could not allocate memory");
            new(memory) T(forward<ARGS>(args)...);
            return memory;
        }

        constexpr auto destroy(T* object) noexcept -> void {
            if (object == nullptr) {
                return;
            }

            object->~T();
            get_self().deallocate(object, 1);
        }
    };

    template<typename T>
    struct Allocator final : BasicAllocator<T, Allocator<T>> {
        using ValueType = T;

        [[nodiscard]] constexpr auto allocate(usize count) noexcept -> T* {
            return reinterpret_cast<T*>(KSTD_MEMORY_ALLOC_FN(sizeof(ValueType) * count));
        }

        constexpr auto deallocate(T* ptr, [[maybe_unused]] usize count) noexcept -> void {
            return KSTD_MEMORY_FREE_FN(ptr);
        }
    };
}