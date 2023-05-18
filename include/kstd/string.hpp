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

#include "string_fwd.hpp"
#include "types.hpp"
#include "allocator.hpp"
#include "libc.hpp"
#include "meta.hpp"

namespace kstd {
    template<typename CHAR, template<typename> typename ALLOCATOR>
    struct BasicString final {
        using ValueType = CHAR;
        using Allocator = ALLOCATOR<ValueType>;
        using Pointer = ValueType*;
        using ConstPointer = const ValueType*;

        private:

        Pointer _data;

        public:

        ~BasicString() noexcept = default;

        [[nodiscard]] constexpr auto get_length() const noexcept -> usize {
            return libc::get_string_length(_data);
        }

        [[nodiscard]] constexpr auto get_c_str() const noexcept -> ConstPointer {
            return _data;
        }

        [[nodiscard]] constexpr auto get_data() const noexcept -> Pointer {
            return _data;
        }

        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool {
            return get_length() == 0;
        }
    };

    template<template<typename> typename ALLOCATOR> //
    using String = BasicString<char, ALLOCATOR>;

    template<template<typename> typename ALLOCATOR> //
    using WString = BasicString<wchar_t, ALLOCATOR>;
}