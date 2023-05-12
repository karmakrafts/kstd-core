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

#include <type_traits>
#include "types.hpp"
#include "kstd/concepts.hpp"

namespace kstd::meta {
    // Constant

    template<typename T, T VALUE>
    struct Constant {
        static constexpr T value = VALUE;
        using ValueType = T;
        using value_type = ValueType; // std compatibility

        [[nodiscard]] constexpr auto operator ()() const noexcept -> ValueType {
            return value;
        }
    };

    // True/False

    struct True : public Constant<bool, true> {};

    struct False : public Constant<bool, false> {};

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
        using type = T;
    };

    template<typename T>
    struct DefIf<false, T> {};

    template<bool CONDITION, typename T = void> //
    using def_if = typename DefIf<CONDITION, T>::type;

    // cond

    template<bool CONDITION, typename IF_TRUE, typename IF_FALSE>
    struct Conditional;

    template<typename IF_TRUE, typename IF_FALSE>
    struct Conditional<true, IF_TRUE, IF_FALSE> {
        using type = IF_TRUE;
    };

    template<typename IF_TRUE, typename IF_FALSE>
    struct Conditional<false, IF_TRUE, IF_FALSE> {
        using type = IF_FALSE;
    };

    template<bool CONDITION, typename IF_TRUE, typename IF_FALSE> //
    using conditional = typename Conditional<CONDITION, IF_TRUE, IF_FALSE>::type;

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

    // is_pod

    template<typename T>
    struct IsPod final {
        // We should use abstraction over the std-lib here since this ties into compiler builtins
        static constexpr bool value = std::is_standard_layout<T>::value;
    };

    template<typename T> //
    constexpr bool is_pod = IsPod<T>::value;

    // is_move_assignable

    template<typename T>
    struct IsMoveAssignable final {
        // We should use abstraction over the std-lib here since this ties into compiler builtins
        static constexpr bool value = std::is_move_assignable<T>::value;
    };

    template<typename T> //
    constexpr bool is_move_assignable = IsMoveAssignable<T>::value;

    // is_move_constructible

    template<typename T>
    struct IsMoveConstructible final {
        // We should use abstraction over the std-lib here since this ties into compiler builtins
        static constexpr bool value = std::is_move_constructible<T>::value;
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
        // We should use abstraction over the std-lib here since this ties into compiler builtins
        static constexpr bool value = std::is_copy_assignable<T>::value;
    };

    template<typename T> //
    constexpr bool is_copy_assignable = IsCopyAssignable<T>::value;

    // is_copy_constructible

    template<typename T>
    struct IsCopyConstructible final {
        // We should use abstraction over the std-lib here since this ties into compiler builtins
        static constexpr bool value = std::is_copy_constructible<T>::value;
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

    // is_destructible

    template<typename T>
    struct IsDestructible final {
        // We should use abstraction over the std-lib here since this ties into compiler builtins
        static constexpr bool value = std::is_destructible<T>::value;
    };

    template<typename T> //
    constexpr bool is_destructible = IsDestructible<T>::value;

    // is_default_constructible

    template<typename T>
    struct IsDefaultConstructible final {
        // We should use abstraction over the std-lib here since this ties into compiler builtins
        static constexpr bool value = std::is_default_constructible<T>::value;
    };

    template<typename T> //
    constexpr bool is_default_constructible = IsDefaultConstructible<T>::value;

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
        using type = T;
    };

    template<typename T>
    struct RemoveRef<T&> {
        using type = T;
    };

    template<typename T>
    struct RemoveRef<const T&> {
        using type = T;
    };

    template<typename T>
    struct RemoveRef<T&&> {
        using type = T;
    };

    template<typename T>
    struct RemoveRef<const T&&> {
        using type = T;
    };

    template<typename T> //
    using remove_ref = typename RemoveRef<T>::type;

    // remove_ptr

    template<typename T>
    struct RemovePtr {
        using type = T;
    };

    template<typename T>
    struct RemovePtr<T*> {
        using type = T;
    };

    template<typename T>
    struct RemovePtr<const T*> {
        using type = T;
    };

    template<typename T>
    struct RemovePtr<volatile T*> {
        using type = T;
    };

    template<typename T>
    struct RemovePtr<const volatile T*> {
        using type = T;
    };

    template<typename T> //
    using remove_ptr = typename RemovePtr<T>::type;

    // remove_const

    template<typename T>
    struct RemoveConst {
        using type = T;
    };

    template<typename T>
    struct RemoveConst<const T> {
        using type = T;
    };

    template<typename T> //
    using remove_const = typename RemoveConst<T>::type;

    // naked_type

    template<typename T>
    struct NakedType final {
        using type = remove_ref<remove_ptr<remove_const<T>>>;
    };

    template<typename T> //
    using naked_type = typename NakedType<T>::type;

    // Pack

    template<typename... TYPES>
    struct Pack final {
        [[maybe_unused]] static constexpr usize num_types = sizeof...(TYPES);
    };

    // pack_element

    template<usize INDEX, typename PACK>
    struct PackElement;

    template<usize INDEX, typename HEAD, typename... TAIL>
    struct PackElement<INDEX, Pack<HEAD, TAIL...>> : public PackElement<INDEX - 1, Pack<TAIL...>> {};

    template<typename HEAD, typename... TAIL>
    struct PackElement<0, Pack<HEAD, TAIL...>> {
        using type = HEAD;
    };

    template<usize INDEX, typename PACK> //
    using pack_element = typename PackElement<INDEX, PACK>::type;

    // join_packs

    template<typename PACK_A, typename PACK_B>
    struct JoinPacks;

    template<typename... TYPES_A, typename... TYPES_B>
    struct JoinPacks<Pack<TYPES_A...>, Pack<TYPES_B...>> {
        using type = Pack<TYPES_A..., TYPES_B...>;
    };

    template<typename PACK_A, typename PACK_B> //
    using join_packs = typename JoinPacks<PACK_A, PACK_B>::type;

    // transform_pack

    template<template<typename> typename TRANSFORM, typename... TYPES> //
    KSTD_REQUIRES(requires { typename TRANSFORM<TYPES...>::type; })
    struct TransformPack {
        using type = Pack<typename TRANSFORM<TYPES>::type...>;
    };

    template<template<typename> typename TRANSFORM, typename... TYPES> //
            KSTD_REQUIRES(requires { typename TRANSFORM<TYPES...>::type; })
    using transform_pack = typename TransformPack<TRANSFORM, TYPES...>::type;

    // substitute_void

    template<typename T, typename SUBSTITUTE> //
    using substitute_void = conditional<is_void<T>, SUBSTITUTE, T>;
}