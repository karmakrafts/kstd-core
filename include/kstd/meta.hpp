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
 * @since 08/05/2023
 */

#pragma once

#include "types.hpp"
#include "kstd/concepts.hpp"

namespace kstd {
    template<typename... TYPES>
    struct Pack final {
        [[maybe_unused]] static constexpr usize num_types = sizeof...(TYPES);
    };

    template<usize INDEX, typename PACK>
    struct nth_pack_element;

    template<usize INDEX, typename HEAD, typename... TAIL>
    struct nth_pack_element<INDEX, Pack<HEAD, TAIL...>> : nth_pack_element<INDEX - 1, Pack<TAIL...>> {};

    template<typename HEAD, typename... TAIL>
    struct nth_pack_element<0, Pack<HEAD, TAIL...>> {
        using type = HEAD;
    };

    template<usize INDEX, typename PACK> using nth_pack_element_t = typename nth_pack_element<INDEX, PACK>::type;

    template<template<typename> typename TRANSFORM, typename... TYPES> //
    KSTD_REQUIRES(requires { typename TRANSFORM<TYPES...>::type; })
    struct transform_pack {
        using type = Pack<typename TRANSFORM<TYPES>::type...>;
    };

    template<template<typename> typename TRANSFORM, typename... TYPES> //
            KSTD_REQUIRES(requires { typename TRANSFORM<TYPES...>::type; })
    using transform_pack_t = typename transform_pack<TRANSFORM, TYPES...>::type;
}