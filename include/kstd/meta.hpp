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

namespace kstd::meta {
    // Constant

    template<typename T, T VALUE>
    struct Constant {
        static constexpr T value = VALUE;
        using ValueType = T;

        [[nodiscard]] constexpr auto operator ()() const noexcept -> ValueType {
            return value;
        }
    };

    // True/False

    struct True : public Constant<bool, true> {};

    struct False : public Constant<bool, false> {};

    // uninitialized

    template<typename T>
    [[nodiscard]] constexpr auto uninitialized() noexcept -> T {
        #if defined(COMPILER_GCC) || defined(COMPILER_CLANG)
        return __declval<T>(0);
        #else
        return *reinterpret_cast<T*>(nullptr); // NOLINT
        #endif // defined(COMPILER_GCC) || defined(COMPILER_CLANG)
    }

    // is_standard_layout

    template<typename T>
    struct IsStandardLayout final {
        static constexpr bool value = __is_standard_layout(T);
    };

    template<typename T> //
    constexpr bool is_standard_layout = IsStandardLayout<T>::value;

    // is_constructible

    template<typename T, typename... ARGS>
    struct IsConstructible final {
        static constexpr bool value = __is_constructible(T, ARGS...);
    };

    template<typename T, typename... ARGS> //
    constexpr bool is_constructible = IsConstructible<T, ARGS...>::value;

    // is_default_constructible

    template<typename T>
    struct IsDefaultConstructible final {
        static constexpr bool value = __is_constructible(T);
    };

    template<typename T> //
    constexpr bool is_default_constructible = IsDefaultConstructible<T>::value;

    // is_assignable

    template<typename LHS, typename RHS>
    struct IsAssignable final {
        static constexpr bool value = __is_assignable(LHS, RHS);
    };

    template<typename LHS, typename RHS> //
    constexpr bool is_assignable = IsAssignable<LHS, RHS>::value;

    // is_destructible

    #if defined(COMPILER_GCC) || defined(COMPILER_CLANG)
    template<typename T, typename = void>
    struct IsDestructible : public False {};

    template<typename T>
    struct IsDestructible<T, decltype(uninitialized<T>().~T())> : public True {};
    #else
    template<typename T>
    struct IsDestructible final {
        static constexpr bool value = __is_destructible(T);
    };
    #endif // defined(COMPILER_GCC) || defined(COMPILER_CLANG)

    template<typename T> //
    constexpr bool is_destructible = IsDestructible<T>::value;

    namespace {
        struct Destructible final {};

        struct NonDestructible final {
            ~NonDestructible() noexcept = delete;
        };

        static_assert(is_destructible<Destructible>, "Type is destructible, trait should return true");
        static_assert(!is_destructible<NonDestructible>, "Type is non-destructible, trait should return false");
    }

    // is_same

    template<typename A, typename B>
    struct IsSame : public False {};

    template<typename T>
    struct IsSame<T, T> : public True {};

    template<typename A, typename B> //
    constexpr bool is_same = IsSame<A, B>::value;

    static_assert(is_same<i32, i32>, "Types are matching, trait should return true");
    static_assert(!is_same<i32, f32>, "Types aren't matching, trait should return false");

    // is_void

    template<typename T>
    struct IsVoid : public False {};

    template<>
    struct IsVoid<void> : public True {};

    template<typename T> //
    constexpr bool is_void = IsVoid<T>::value;

    static_assert(is_void<void>, "Type is void, trait should return true");
    static_assert(!is_void<i32>, "Type is not a void, trait should return false");

    // def_if

    template<bool CONDITION, typename T = void>
    struct DefIf;

    template<typename T>
    struct DefIf<true, T> {
        using Type = T;
    };

    template<typename T>
    struct DefIf<false, T> {};

    template<bool CONDITION, typename T = void> //
    using def_if = typename DefIf<CONDITION, T>::Type;

    // conditional

    template<bool CONDITION, typename IF_TRUE, typename IF_FALSE>
    struct Conditional;

    template<typename IF_TRUE, typename IF_FALSE>
    struct Conditional<true, IF_TRUE, IF_FALSE> {
        using Type = IF_TRUE;
    };

    template<typename IF_TRUE, typename IF_FALSE>
    struct Conditional<false, IF_TRUE, IF_FALSE> {
        using Type = IF_FALSE;
    };

    template<bool CONDITION, typename IF_TRUE, typename IF_FALSE> //
    using conditional = typename Conditional<CONDITION, IF_TRUE, IF_FALSE>::Type;

    // is_ptr

    template<typename T>
    struct IsPtr : public False {};

    template<typename T>
    struct IsPtr<T*> : public True {};

    template<typename T> constexpr bool is_ptr = IsPtr<T>::value;

    static_assert(is_ptr<i32*>, "Type is pointer, trait should return true");
    static_assert(is_ptr<const i32*>, "Type is pointer, trait should return true");
    static_assert(!is_ptr<i32>, "Type is not a pointer, trait should return false");

    // is_lvref

    template<typename T>
    struct IsLVRef : public False {};

    template<typename T>
    struct IsLVRef<T&> : public True {};

    template<typename T> //
    constexpr bool is_lvref = IsLVRef<T>::value;

    static_assert(is_lvref<i32&>, "Type is an lvalue reference, trait should return true");
    static_assert(is_lvref<const i32&>, "Type is an lvalue reference, trait should return true");
    static_assert(!is_lvref<void>, "Type is not an lvalue reference, trait should return false");
    static_assert(!is_lvref<i32&&>, "Type is not an lvalue reference, trait should return false");

    // is_rvref

    template<typename T>
    struct IsRVRef : public False {};

    template<typename T>
    struct IsRVRef<T&&> : public True {};

    template<typename T> //
    constexpr bool is_rvref = IsRVRef<T>::value;

    static_assert(!is_rvref<i32&>, "Type is not an rvalue reference, trait should return true");
    static_assert(!is_rvref<const i32&>, "Type is not an rvalue reference, trait should return true");
    static_assert(is_rvref<i32&&>, "Type is an rvalue reference, trait should return false");
    static_assert(is_rvref<const i32&&>, "Type is an rvalue reference, trait should return false");

    // is_ref

    template<typename T>
    struct IsRef final {
        static constexpr bool value = is_lvref<T> || is_rvref<T>;
    };

    template<typename T> //
    constexpr bool is_ref = IsRef<T>::value;

    // is_move_assignable

    template<typename T>
    struct IsMoveAssignable final {
        static constexpr bool value = is_assignable < T &, T&&>;
    };

    template<typename T> //
    constexpr bool is_move_assignable = IsMoveAssignable<T>::value;

    // is_move_constructible

    template<typename T>
    struct IsMoveConstructible final {
        static constexpr bool value = is_constructible<T, T&&>;
    };

    template<typename T> //
    constexpr bool is_move_constructible = IsMoveConstructible<T>::value;

    // is_movable

    template<typename T>
    struct IsMovable final {
        static constexpr bool value = is_move_assignable<T> || is_move_constructible<T>;
    };

    template<typename T> //
    constexpr bool is_movable = IsMovable<T>::value;

    // is_copy_assignable

    template<typename T>
    struct IsCopyAssignable final {
        static constexpr bool value = is_assignable < T &,
        const T&>;
    };

    template<typename T> //
    constexpr bool is_copy_assignable = IsCopyAssignable<T>::value;

    // is_copy_constructible

    template<typename T>
    struct IsCopyConstructible final {
        static constexpr bool value = is_constructible<T, const T&>;
    };

    template<typename T> //
    constexpr bool is_copy_constructible = IsCopyConstructible<T>::value;

    // is_copyable

    template<typename T>
    struct IsCopyable final {
        static constexpr bool value = is_copy_assignable<T> || is_copy_constructible<T>;
    };

    template<typename T> //
    constexpr bool is_copyable = IsCopyable<T>::value;

    // is_const

    template<typename T>
    struct IsConst : public False {};

    template<typename T>
    struct IsConst<const T> : public True {};

    template<typename T>
    struct IsConst<const T*> : public True {};

    template<typename T>
    struct IsConst<const T&> : public True {};

    template<typename T>
    struct IsConst<const T&&> : public True {};

    template<typename T> //
    constexpr bool is_const = IsConst<T>::value;

    // is_volatile

    template<typename T>
    struct IsVolatile : public False {};

    template<typename T>
    struct IsVolatile<volatile T> : public True {};

    template<typename T>
    struct IsVolatile<volatile T*> : public True {};

    template<typename T>
    struct IsVolatile<volatile T&> : public True {};

    template<typename T>
    struct IsVolatile<volatile T&&> : public True {};

    template<typename T> //
    constexpr bool is_volatile = IsVolatile<T>::value;

    // is_cv

    template<typename T>
    struct IsCV final {
        static constexpr bool value = is_const<T> || is_volatile<T>;
    };

    template<typename T> //
    constexpr bool is_cv = IsCV<T>::value;

    // remove_ref

    template<typename T>
    struct RemoveRef {
        using Type = T;
    };

    template<typename T>
    struct RemoveRef<T&> {
        using Type = T;
    };

    template<typename T>
    struct RemoveRef<const T&> {
        using Type = T;
    };

    template<typename T>
    struct RemoveRef<T&&> {
        using Type = T;
    };

    template<typename T>
    struct RemoveRef<const T&&> {
        using Type = T;
    };

    template<typename T> //
    using remove_ref = typename RemoveRef<T>::Type;

    // remove_ptr

    template<typename T>
    struct RemovePtr {
        using Type = T;
    };

    template<typename T>
    struct RemovePtr<T*> {
        using Type = T;
    };

    template<typename T>
    struct RemovePtr<const T*> {
        using Type = T;
    };

    template<typename T>
    struct RemovePtr<volatile T*> {
        using Type = T;
    };

    template<typename T>
    struct RemovePtr<const volatile T*> {
        using Type = T;
    };

    template<typename T> //
    using remove_ptr = typename RemovePtr<T>::Type;

    // remove_const

    template<typename T>
    struct RemoveConst {
        using Type = T;
    };

    template<typename T>
    struct RemoveConst<const T> {
        using Type = T;
    };

    template<typename T>
    struct RemoveConst<const T*> {
        using Type = T;
    };

    template<typename T>
    struct RemoveConst<const T&> {
        using Type = T;
    };

    template<typename T>
    struct RemoveConst<const T&&> {
        using Type = T;
    };

    template<typename T> //
    using remove_const = typename RemoveConst<T>::Type;

    // remove_volatile

    template<typename T>
    struct RemoveVolatile {
        using Type = T;
    };

    template<typename T>
    struct RemoveVolatile<const T> {
        using Type = T;
    };

    template<typename T>
    struct RemoveVolatile<const T*> {
        using Type = T;
    };

    template<typename T>
    struct RemoveVolatile<const T&> {
        using Type = T;
    };

    template<typename T>
    struct RemoveVolatile<const T&&> {
        using Type = T;
    };

    template<typename T> //
    using remove_volatile = typename RemoveVolatile<T>::Type;

    // remove_cv

    template<typename T>
    struct RemoveCV final {
        using Type = remove_const<remove_volatile<T>>;
    };

    template<typename T> //
    using remove_cv = typename RemoveCV<T>::Type;

    // naked_type

    template<typename T>
    struct NakedType final {
        using Type = remove_ref<remove_ptr<remove_cv<T>>>;
    };

    template<typename T> //
    using naked_type = typename NakedType<T>::Type;

    // Pack

    template<typename... TYPES>
    struct Pack final {
        [[maybe_unused]] static constexpr usize num_types = sizeof...(TYPES);

        [[nodiscard, maybe_unused]] constexpr auto get_size() const noexcept -> usize {
            return num_types;
        }
    };

    // pack_element

    template<usize INDEX, typename PACK>
    struct PackElement;

    template<usize INDEX, typename HEAD, typename... TAIL>
    struct PackElement<INDEX, Pack<HEAD, TAIL...>> : public PackElement<INDEX - 1, Pack<TAIL...>> {};

    template<typename HEAD, typename... TAIL>
    struct PackElement<0, Pack<HEAD, TAIL...>> {
        using Type = HEAD;
    };

    template<usize INDEX, typename PACK> //
    using pack_element = typename PackElement<INDEX, PACK>::Type;

    static_assert(is_same<pack_element<0, Pack<i32, f32, u32>>, i32>, "Type should be i32");
    static_assert(is_same<pack_element<1, Pack<i32, f32, u32>>, f32>, "Type should be f32");
    static_assert(is_same<pack_element<2, Pack<i32, f32, u32>>, u32>, "Type should be u32");

    // skip_elements

    template<usize COUNT, typename PACK, typename = Pack<>>
    struct SkipElements {
        using Type = Pack<>;
    };

    template<usize COUNT, typename HEAD, typename... TAIL, typename... SKIPPED>
    struct SkipElements<COUNT, Pack<HEAD, TAIL...>, Pack<SKIPPED...>> : public SkipElements<COUNT - 1, Pack<TAIL...>, Pack<SKIPPED..., HEAD>> {};

    template<typename HEAD, typename... TAIL, typename... SKIPPED>
    struct SkipElements<0, Pack<HEAD, TAIL...>, Pack<SKIPPED...>> {
        using Type = Pack<HEAD, TAIL...>;
    };

    template<typename... SKIPPED>
    struct SkipElements<0, Pack<>, Pack<SKIPPED...>> {
        using Type = Pack<>;
    };

    template<usize COUNT, typename PACK> //
    using skip_elements = typename SkipElements<COUNT, PACK>::Type;

    static_assert(is_same<skip_elements<0, Pack<>>, Pack<>>, "Pack type should be Pack<>");
    static_assert(is_same<skip_elements<1, Pack<>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(is_same<skip_elements<0, Pack<i32, f32, u64>>, Pack<i32, f32, u64>>, "Pack type should be Pack<i32, f32, u64>");
    static_assert(is_same<skip_elements<1, Pack<i32, f32, u64>>, Pack<f32, u64>>, "Pack type should be Pack<f32, u64>");
    static_assert(is_same<skip_elements<2, Pack<i32, f32, u64>>, Pack<u64>>, "Pack type should be Pack<u64>");
    static_assert(is_same<skip_elements<3, Pack<i32, f32, u64>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(skip_elements<0, Pack<i32, f32, u64>>().get_size() == 3, "Pack size should be 3");
    static_assert(skip_elements<1, Pack<i32, f32, u64>>().get_size() == 2, "Pack size should be 2");
    static_assert(skip_elements<2, Pack<i32, f32, u64>>().get_size() == 1, "Pack size should be 1");
    static_assert(skip_elements<3, Pack<i32, f32, u64>>().get_size() == 0, "Pack size should be 0");

    // trim_elements

    template<usize COUNT, typename PACK, typename = Pack<>>
    struct TrimElements {
        using Type = Pack<>;
    };

    template<usize COUNT, typename HEAD, typename... TAIL, typename... ACCUMULATED>
    struct TrimElements<COUNT, Pack<HEAD, TAIL...>, Pack<ACCUMULATED...>> : public TrimElements<COUNT - 1, Pack<TAIL...>, Pack<ACCUMULATED..., HEAD>> {};

    template<typename HEAD, typename... TAIL, typename... ACCUMULATED>
    struct TrimElements<0, Pack<HEAD, TAIL...>, Pack<ACCUMULATED...>> {
        using Type = Pack<ACCUMULATED...>;
    };

    template<typename... ACCUMULATED>
    struct TrimElements<0, Pack<>, Pack<ACCUMULATED...>> {
        using Type = Pack<ACCUMULATED...>;
    };

    template<usize COUNT, typename PACK> //
    using trim_elements = typename TrimElements<COUNT, PACK>::Type;

    static_assert(is_same<trim_elements<0, Pack<>>, Pack<>>, "Pack type should be Pack<>");
    static_assert(is_same<trim_elements<1, Pack<>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(is_same<trim_elements<3, Pack<i32, f32, u64>>, Pack<i32, f32, u64>>, "Pack type should be Pack<i32, f32, u64>");
    static_assert(is_same<trim_elements<2, Pack<i32, f32, u64>>, Pack<i32, f32>>, "Pack type should be Pack<i32, f32>");
    static_assert(is_same<trim_elements<1, Pack<i32, f32, u64>>, Pack<i32>>, "Pack type should be Pack<i32>");
    static_assert(is_same<trim_elements<0, Pack<i32, f32, u64>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(trim_elements<0, Pack<i32, f32, u64>>().get_size() == 0, "Pack size should be 0");
    static_assert(trim_elements<1, Pack<i32, f32, u64>>().get_size() == 1, "Pack size should be 1");

    static_assert(trim_elements<2, Pack<i32, f32, u64>>().get_size() == 2, "Pack size should be 2");
    static_assert(trim_elements<3, Pack<i32, f32, u64>>().get_size() == 3, "Pack size should be 3");

    // slice_pack

    template<usize BEGIN, usize END, typename PACK>
    struct SlicePack;

    template<usize BEGIN, usize END, typename... TYPES>
    struct SlicePack<BEGIN, END, Pack<TYPES...>> {
        using Type = trim_elements<END + 1 - BEGIN, skip_elements<BEGIN, Pack<TYPES...>>>;
    };

    template<usize BEGIN, usize END>
    struct SlicePack<BEGIN, END, Pack<>> {
        using Type = Pack<>;
    };

    template<usize BEGIN, usize END, typename PACK> //
    using slice_pack = typename SlicePack<BEGIN, END, PACK>::Type;

    static_assert(is_same<slice_pack<0, 0, Pack<>>, Pack<>>, "Pack type should be Pack<>");
    static_assert(is_same<slice_pack<0, 1, Pack<>>, Pack<>>, "Pack type should be Pack<>");
    static_assert(is_same<slice_pack<1, 2, Pack<>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(is_same<slice_pack<0, 3, Pack<i32, f32, u64, u32>>, Pack<i32, f32, u64, u32>>, "Pack type should be Pack<i32, f32, u64, u32>");
    static_assert(is_same<slice_pack<0, 2, Pack<i32, f32, u64, u32>>, Pack<i32, f32, u64>>, "Pack type should be Pack<i32, f32, u64>");
    static_assert(is_same<slice_pack<0, 1, Pack<i32, f32, u64, u32>>, Pack<i32, f32>>, "Pack type should be Pack<i32, f32>");
    static_assert(is_same<slice_pack<0, 0, Pack<i32, f32, u64, u32>>, Pack<i32>>, "Pack type should be Pack<i32>");

    static_assert(is_same<slice_pack<0, 3, Pack<i32, f32, u64, u32>>, Pack<i32, f32, u64, u32>>, "Pack type should be Pack<i32, f32, u64, u32>");
    static_assert(is_same<slice_pack<1, 3, Pack<i32, f32, u64, u32>>, Pack<f32, u64, u32>>, "Pack type should be Pack<f32, u64, u32>");
    static_assert(is_same<slice_pack<2, 3, Pack<i32, f32, u64, u32>>, Pack<u64, u32>>, "Pack type should be Pack<u64, u32>");
    static_assert(is_same<slice_pack<3, 3, Pack<i32, f32, u64, u32>>, Pack<u32>>, "Pack type should be Pack<u32>");

    static_assert(slice_pack<0, 3, Pack<i32, f32, u64, u32>>().get_size() == 4, "Pack size should be 4");
    static_assert(slice_pack<0, 2, Pack<i32, f32, u64, u32>>().get_size() == 3, "Pack size should be 3");
    static_assert(slice_pack<0, 1, Pack<i32, f32, u64, u32>>().get_size() == 2, "Pack size should be 2");
    static_assert(slice_pack<0, 0, Pack<i32, f32, u64, u32>>().get_size() == 1, "Pack size should be 1");

    // join_packs

    template<typename PACK_A, typename PACK_B>
    struct JoinPacks;

    template<typename... TYPES_A, typename... TYPES_B>
    struct JoinPacks<Pack<TYPES_A...>, Pack<TYPES_B...>> {
        using Type = Pack<TYPES_A..., TYPES_B...>;
    };

    template<typename PACK_A, typename PACK_B> //
    using join_packs = typename JoinPacks<PACK_A, PACK_B>::Type;

    static_assert(is_same<join_packs<Pack<>, Pack<>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(is_same<join_packs<Pack<>, Pack<i32, f32>>, Pack<i32, f32>>, "Pack type should be Pack<i32, f32>");
    static_assert(is_same<join_packs<Pack<i32, f32>, Pack<>>, Pack<i32, f32>>, "Pack type should be Pack<i32, f32>");
    static_assert(is_same<join_packs<Pack<i32, f32>, Pack<i32, f32>>, Pack<i32, f32, i32, f32>>, "Pack type should be Pack<i32, f32, i32, f32>");

    // transform_pack

    template<template<typename> typename TRANSFORM, typename... TYPES> //
    struct TransformPack {
        using Type = Pack<typename TRANSFORM<TYPES>::Type...>;
    };

    template<template<typename> typename TRANSFORM, typename... TYPES> //
    using transform_pack = typename TransformPack<TRANSFORM, TYPES...>::Type;

    // substitute_void

    template<typename T, typename SUBSTITUTE> //
    using substitute_void = conditional<is_void<T>, SUBSTITUTE, T>;
}