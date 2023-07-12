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

#include <type_traits>

namespace kstd {
    template<typename... TYPES>
    struct Pack final {
        [[maybe_unused]] static constexpr usize size = sizeof...(TYPES);

        [[nodiscard, maybe_unused]] constexpr auto get_size() const noexcept -> usize {
            return size;
        }
    };

    // PackElement

    template<usize INDEX, typename PACK>
    struct PackElementImpl;

    template<usize INDEX, typename HEAD, typename... TAIL>
    struct PackElementImpl<INDEX, Pack<HEAD, TAIL...>> : public PackElementImpl<INDEX - 1, Pack<TAIL...>> {};

    template<typename HEAD, typename... TAIL>
    struct PackElementImpl<0, Pack<HEAD, TAIL...>> {
        using type = HEAD;
    };

    template<usize INDEX, typename PACK>//
    using pack_element = typename PackElementImpl<INDEX, PACK>::type;

#ifdef BUILD_DEBUG
    static_assert(std::is_same_v<pack_element<0, Pack<i32, f32, u32>>, i32>, "Type should be i32");
    static_assert(std::is_same_v<pack_element<1, Pack<i32, f32, u32>>, f32>, "Type should be f32");
    static_assert(std::is_same_v<pack_element<2, Pack<i32, f32, u32>>, u32>, "Type should be u32");
#endif

    // LeftTrimPack

    template<usize COUNT, typename PACK, typename = Pack<>>
    struct LeftTrimPackImpl {
        using type = Pack<>;
    };

    template<usize COUNT, typename HEAD, typename... TAIL, typename... SKIPPED>
    struct LeftTrimPackImpl<COUNT, Pack<HEAD, TAIL...>, Pack<SKIPPED...>>
            : public LeftTrimPackImpl<COUNT - 1, Pack<TAIL...>, Pack<SKIPPED..., HEAD>> {};

    template<typename HEAD, typename... TAIL, typename... SKIPPED>
    struct LeftTrimPackImpl<0, Pack<HEAD, TAIL...>, Pack<SKIPPED...>> {
        using type = Pack<HEAD, TAIL...>;
    };

    template<typename... SKIPPED>
    struct LeftTrimPackImpl<0, Pack<>, Pack<SKIPPED...>> {
        using type = Pack<>;
    };

    template<usize COUNT, typename PACK>//
    using left_trim_pack = typename LeftTrimPackImpl<COUNT, PACK>::type;

#ifdef BUILD_DEBUG
    static_assert(std::is_same_v<left_trim_pack<0, Pack<>>, Pack<>>, "Pack type should be Pack<>");
    static_assert(std::is_same_v<left_trim_pack<1, Pack<>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(std::is_same_v<left_trim_pack<0, Pack<i32, f32, u64>>, Pack<i32, f32, u64>>,
                  "Pack type should be Pack<i32, f32, u64>");
    static_assert(std::is_same_v<left_trim_pack<1, Pack<i32, f32, u64>>, Pack<f32, u64>>,
                  "Pack type should be Pack<f32, u64>");
    static_assert(std::is_same_v<left_trim_pack<2, Pack<i32, f32, u64>>, Pack<u64>>, "Pack type should be Pack<u64>");
    static_assert(std::is_same_v<left_trim_pack<3, Pack<i32, f32, u64>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(left_trim_pack<0, Pack<i32, f32, u64>>().get_size() == 3, "Pack size should be 3");
    static_assert(left_trim_pack<1, Pack<i32, f32, u64>>().get_size() == 2, "Pack size should be 2");
    static_assert(left_trim_pack<2, Pack<i32, f32, u64>>().get_size() == 1, "Pack size should be 1");
    static_assert(left_trim_pack<3, Pack<i32, f32, u64>>().get_size() == 0, "Pack size should be 0");
#endif

    // RightTrimPack

    template<usize COUNT, typename PACK, typename = Pack<>>
    struct RightTrimPackImpl {
        using type = Pack<>;
    };

    template<usize COUNT, typename HEAD, typename... TAIL, typename... ACCUMULATED>
    struct RightTrimPackImpl<COUNT, Pack<HEAD, TAIL...>, Pack<ACCUMULATED...>>
            : public RightTrimPackImpl<COUNT - 1, Pack<TAIL...>, Pack<ACCUMULATED..., HEAD>> {};

    template<typename HEAD, typename... TAIL, typename... ACCUMULATED>
    struct RightTrimPackImpl<0, Pack<HEAD, TAIL...>, Pack<ACCUMULATED...>> {
        using type = Pack<ACCUMULATED...>;
    };

    template<typename... ACCUMULATED>
    struct RightTrimPackImpl<0, Pack<>, Pack<ACCUMULATED...>> {
        using type = Pack<ACCUMULATED...>;
    };

    template<usize COUNT, typename PACK>//
    using right_trim_pack = typename RightTrimPackImpl<COUNT, PACK>::type;

#ifdef BUILD_DEBUG
    static_assert(std::is_same_v<right_trim_pack<0, Pack<>>, Pack<>>, "Pack type should be Pack<>");
    static_assert(std::is_same_v<right_trim_pack<1, Pack<>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(std::is_same_v<right_trim_pack<3, Pack<i32, f32, u64>>, Pack<i32, f32, u64>>,
                  "Pack type should be Pack<i32, f32, u64>");
    static_assert(std::is_same_v<right_trim_pack<2, Pack<i32, f32, u64>>, Pack<i32, f32>>,
                  "Pack type should be Pack<i32, f32>");
    static_assert(std::is_same_v<right_trim_pack<1, Pack<i32, f32, u64>>, Pack<i32>>, "Pack type should be Pack<i32>");
    static_assert(std::is_same_v<right_trim_pack<0, Pack<i32, f32, u64>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(right_trim_pack<0, Pack<i32, f32, u64>>().get_size() == 0, "Pack size should be 0");
    static_assert(right_trim_pack<1, Pack<i32, f32, u64>>().get_size() == 1, "Pack size should be 1");

    static_assert(right_trim_pack<2, Pack<i32, f32, u64>>().get_size() == 2, "Pack size should be 2");
    static_assert(right_trim_pack<3, Pack<i32, f32, u64>>().get_size() == 3, "Pack size should be 3");
#endif

    // SlicePack

    template<usize BEGIN, usize END, typename PACK>
    struct SlicePackImpl;

    template<usize BEGIN, usize END, typename... TYPES>
    struct SlicePackImpl<BEGIN, END, Pack<TYPES...>> {
        using type = right_trim_pack<END + 1 - BEGIN, left_trim_pack<BEGIN, Pack<TYPES...>>>;
    };

    template<usize BEGIN, usize END>
    struct SlicePackImpl<BEGIN, END, Pack<>> {
        using type = Pack<>;
    };

    template<usize BEGIN, usize END, typename PACK>//
    using slice_pack = typename SlicePackImpl<BEGIN, END, PACK>::type;

#ifdef BUILD_DEBUG
    static_assert(std::is_same_v<slice_pack<0, 0, Pack<>>, Pack<>>, "Pack type should be Pack<>");
    static_assert(std::is_same_v<slice_pack<0, 1, Pack<>>, Pack<>>, "Pack type should be Pack<>");
    static_assert(std::is_same_v<slice_pack<1, 2, Pack<>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(std::is_same_v<slice_pack<0, 3, Pack<i32, f32, u64, u32>>, Pack<i32, f32, u64, u32>>,
                  "Pack type should be Pack<i32, f32, u64, u32>");
    static_assert(std::is_same_v<slice_pack<0, 2, Pack<i32, f32, u64, u32>>, Pack<i32, f32, u64>>,
                  "Pack type should be Pack<i32, f32, u64>");
    static_assert(std::is_same_v<slice_pack<0, 1, Pack<i32, f32, u64, u32>>, Pack<i32, f32>>,
                  "Pack type should be Pack<i32, f32>");
    static_assert(std::is_same_v<slice_pack<0, 0, Pack<i32, f32, u64, u32>>, Pack<i32>>,
                  "Pack type should be Pack<i32>");

    static_assert(std::is_same_v<slice_pack<0, 3, Pack<i32, f32, u64, u32>>, Pack<i32, f32, u64, u32>>,
                  "Pack type should be Pack<i32, f32, u64, u32>");
    static_assert(std::is_same_v<slice_pack<1, 3, Pack<i32, f32, u64, u32>>, Pack<f32, u64, u32>>,
                  "Pack type should be Pack<f32, u64, u32>");
    static_assert(std::is_same_v<slice_pack<2, 3, Pack<i32, f32, u64, u32>>, Pack<u64, u32>>,
                  "Pack type should be Pack<u64, u32>");
    static_assert(std::is_same_v<slice_pack<3, 3, Pack<i32, f32, u64, u32>>, Pack<u32>>,
                  "Pack type should be Pack<u32>");

    static_assert(slice_pack<0, 3, Pack<i32, f32, u64, u32>>().get_size() == 4, "Pack size should be 4");
    static_assert(slice_pack<0, 2, Pack<i32, f32, u64, u32>>().get_size() == 3, "Pack size should be 3");
    static_assert(slice_pack<0, 1, Pack<i32, f32, u64, u32>>().get_size() == 2, "Pack size should be 2");
    static_assert(slice_pack<0, 0, Pack<i32, f32, u64, u32>>().get_size() == 1, "Pack size should be 1");
#endif

    // ConcatPacks

    template<typename PACK_A, typename PACK_B>
    struct ConcatPacksImpl;

    template<typename... TYPES_A, typename... TYPES_B>
    struct ConcatPacksImpl<Pack<TYPES_A...>, Pack<TYPES_B...>> {
        using type = Pack<TYPES_A..., TYPES_B...>;
    };

    template<typename PACK_A, typename PACK_B>//
    using concat_packs = typename ConcatPacksImpl<PACK_A, PACK_B>::type;

#ifdef BUILD_DEBUG
    static_assert(std::is_same_v<concat_packs<Pack<>, Pack<>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(std::is_same_v<concat_packs<Pack<>, Pack<i32, f32>>, Pack<i32, f32>>,
                  "Pack type should be Pack<i32, f32>");
    static_assert(std::is_same_v<concat_packs<Pack<i32, f32>, Pack<>>, Pack<i32, f32>>,
                  "Pack type should be Pack<i32, f32>");
    static_assert(std::is_same_v<concat_packs<Pack<i32, f32>, Pack<i32, f32>>, Pack<i32, f32, i32, f32>>,
                  "Pack type should be Pack<i32, f32, i32, f32>");
#endif

    // TransformPack

    template<template<typename> typename TRANSFORM, typename PACK>
    struct TransformPackImpl;

    template<template<typename> typename TRANSFORM, typename... TYPES>//
    struct TransformPackImpl<TRANSFORM, Pack<TYPES...>> {
        using type = Pack<typename TRANSFORM<TYPES>::type...>;
    };

    template<template<typename> typename TRANSFORM, typename... TYPES>//
    using transform_pack = typename TransformPackImpl<TRANSFORM, TYPES...>::type;
}// namespace kstd