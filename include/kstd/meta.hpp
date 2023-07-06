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

#define KSTD_SFINAE_TRAIT(t, n, c)        /* NOLINT */                                                                 \
    template<typename T, typename = void> /* NOLINT */                                                                 \
    struct t : public kstd::meta::False {};                                                                            \
    template<typename T>                                                                                               \
    struct t<T, kstd::meta::DefIf<(c)>> : public kstd::meta::True {};                                                  \
    template<typename T>                                                                                               \
    constexpr bool n = t<T>::value;// NOLINT

namespace kstd::meta {
    // Constant

    template<typename T, T VALUE>
    struct Constant {
        static constexpr T value = VALUE;
        using ValueType = T;

        [[nodiscard]] constexpr auto operator()() const noexcept -> ValueType {
            return value;
        }
    };

    // True/False

    struct True : public Constant<bool, true> {};

    struct False : public Constant<bool, false> {};

    // uneval

    template<typename T>
    [[nodiscard]] constexpr auto val() noexcept -> T {
        throw;// NOLINT: if this is reached at runtime, terminate
    }

    // is_standard_layout

    template<typename T>
    struct IsStandardLayout final {
        static constexpr bool value = __is_standard_layout(T);
    };

    template<typename T>//
    constexpr bool is_standard_layout = IsStandardLayout<T>::value;

    // is_trivial

    template<typename T>
    struct IsTrivial final {
        static constexpr bool value = __is_trivial(T);
    };

    template<typename T>//
    constexpr bool is_trivial = IsTrivial<T>::value;

    // is_trivially_copyable

    template<typename T>
    struct IsTriviallyCopyable final {
        static constexpr bool value = __is_trivially_copyable(T);
    };

    template<typename T>//
    constexpr bool is_trivially_copyable = IsTriviallyCopyable<T>::value;

    // is_base_of

    template<typename BASE, typename DERIVED>
    struct IsBaseOf final {
        static constexpr bool value = __is_base_of(BASE, DERIVED);
    };

    template<typename BASE, typename DERIVED>//
    constexpr bool is_base_of = IsBaseOf<BASE, DERIVED>::value;

    // is_constructible

    template<typename T, typename... ARGS>
    struct IsConstructible final {
        static constexpr bool value = __is_constructible(T, ARGS...);
    };

    template<typename T, typename... ARGS>//
    constexpr bool is_constructible = IsConstructible<T, ARGS...>::value;

    // is_default_constructible

    template<typename T>
    struct IsDefaultConstructible final {
        static constexpr bool value = __is_constructible(T);
    };

    template<typename T>//
    constexpr bool is_default_constructible = IsDefaultConstructible<T>::value;

    // is_assignable

    template<typename LHS, typename RHS>
    struct IsAssignable final {
        static constexpr bool value = __is_assignable(LHS, RHS);
    };

    template<typename LHS, typename RHS>//
    constexpr bool is_assignable = IsAssignable<LHS, RHS>::value;

    // is_same

    template<typename A, typename B>
    struct IsSame : public False {};

    template<typename T>
    struct IsSame<T, T> : public True {};

    template<typename A, typename B>//
    constexpr bool is_same = IsSame<A, B>::value;

#ifdef BUILD_DEBUG
    static_assert(is_same<i32, i32>, "Types are matching, trait should return true");
    static_assert(!is_same<i32, f32>, "Types aren't matching, trait should return false");
#endif

    // is_void

    template<typename T>
    struct IsVoid : public False {};

    template<>
    struct IsVoid<void> : public True {};

    template<typename T>//
    constexpr bool is_void = IsVoid<T>::value;

#ifdef BUILD_DEBUG
    static_assert(is_void<void>, "Type is void, trait should return true");
    static_assert(!is_void<i32>, "Type is not a void, trait should return false");
#endif

    // DefIf

    template<bool CONDITION, typename T = void>
    struct DefIfImpl;

    template<typename T>
    struct DefIfImpl<true, T> {
        using Type = T;
    };

    template<typename T>
    struct DefIfImpl<false, T> {};

    template<bool CONDITION, typename T = void>//
    using DefIf = typename DefIfImpl<CONDITION, T>::Type;

    // If

    template<bool CONDITION, typename IF_TRUE, typename IF_FALSE>
    struct IfImpl;

    template<typename IF_TRUE, typename IF_FALSE>
    struct IfImpl<true, IF_TRUE, IF_FALSE> {
        using Type = IF_TRUE;
    };

    template<typename IF_TRUE, typename IF_FALSE>
    struct IfImpl<false, IF_TRUE, IF_FALSE> {
        using Type = IF_FALSE;
    };

    template<bool CONDITION, typename IF_TRUE, typename IF_FALSE>//
    using If = typename IfImpl<CONDITION, IF_TRUE, IF_FALSE>::Type;

    // is_destructible

    /*
     * Quick note: on GCC and Clang we need to implement this ourselves.
     * We simply do this using SFINAE; We obtain the type of an unevaluated destructor call expression over T.
     * If the destructor call expression is valid, the surrounding decltype expression will just give us void,
     * since a destructor is just a void function.
     * Otherwise, if it is ill-formed, the surrounding decltype expression will prevent the specialization from matching
     * because it itself will become ill-formed.
     */

#if defined(COMPILER_GCC) || defined(COMPILER_CLANG)
    KSTD_SFINAE_TRAIT(IsDestructible, is_destructible, is_void<decltype(val<T>().~T())>)
#else
    template<typename T>
    struct IsDestructible final {
        static constexpr bool value = __is_destructible(T);
    };

    template<typename T>//
    constexpr bool is_destructible = IsDestructible<T>::value;
#endif// defined(COMPILER_GCC) || defined(COMPILER_CLANG)

#ifdef BUILD_DEBUG
    namespace {
        struct Destructible final {};

        struct NonDestructible final {// NOLINT
            ~NonDestructible() noexcept = delete;
        };

        static_assert(is_destructible<Destructible>, "Type is destructible, trait should return true");
        static_assert(!is_destructible<NonDestructible>, "Type is non-destructible, trait should return false");
    }// namespace
#endif

    // is_ptr

    template<typename T>
    struct IsPtr : public False {};

    template<typename T>
    struct IsPtr<T*> : public True {};

    template<typename T>
    constexpr bool is_ptr = IsPtr<T>::value;

#ifdef BUILD_DEBUG
    static_assert(is_ptr<i32*>, "Type is pointer, trait should return true");
    static_assert(is_ptr<const i32*>, "Type is pointer, trait should return true");
    static_assert(!is_ptr<i32>, "Type is not a pointer, trait should return false");
#endif

    // is_owned_ref

    template<typename T>
    struct IsOwnedRefImpl : public False {};

    template<typename T>
    struct IsOwnedRefImpl<T&> : public True {};

    template<typename T>//
    constexpr bool is_owned_ref = IsOwnedRefImpl<T>::value;

#ifdef BUILD_DEBUG
    static_assert(is_owned_ref<i32&>, "Type is an lvalue reference, trait should return true");
    static_assert(is_owned_ref<const i32&>, "Type is an lvalue reference, trait should return true");
    static_assert(!is_owned_ref<void>, "Type is not an lvalue reference, trait should return false");
    static_assert(!is_owned_ref<i32&&>, "Type is not an lvalue reference, trait should return false");
#endif

    // is_temp_ref

    template<typename T>
    struct IsTempRefImpl : public False {};

    template<typename T>
    struct IsTempRefImpl<T&&> : public True {};

    template<typename T>//
    constexpr bool is_temp_ref = IsTempRefImpl<T>::value;

#ifdef BUILD_DEBUG
    static_assert(!is_temp_ref<i32&>, "Type is not an rvalue reference, trait should return true");
    static_assert(!is_temp_ref<const i32&>, "Type is not an rvalue reference, trait should return true");
    static_assert(is_temp_ref<i32&&>, "Type is an rvalue reference, trait should return false");
    static_assert(is_temp_ref<const i32&&>, "Type is an rvalue reference, trait should return false");
#endif

    // is_ref

    template<typename T>
    struct IsRef final {
        static constexpr bool value = is_owned_ref<T> || is_temp_ref<T>;
    };

    template<typename T>//
    constexpr bool is_ref = IsRef<T>::value;

    // is_move_assignable

    template<typename T>
    struct IsMoveAssignable final {
        static constexpr bool value = is_assignable<T&, T&&>;
    };

    template<typename T>//
    constexpr bool is_move_assignable = IsMoveAssignable<T>::value;

    // is_move_constructible

    template<typename T>
    struct IsMoveConstructible final {
        static constexpr bool value = is_constructible<T, T&&>;
    };

    template<typename T>//
    constexpr bool is_move_constructible = IsMoveConstructible<T>::value;

    // is_movable

    template<typename T>
    struct IsMovable final {
        static constexpr bool value = is_move_assignable<T> || is_move_constructible<T>;
    };

    template<typename T>//
    constexpr bool is_movable = IsMovable<T>::value;

    // is_copy_assignable

    template<typename T>
    struct IsCopyAssignable final {
        static constexpr bool value = is_assignable<T&, const T&>;
    };

    template<typename T>//
    constexpr bool is_copy_assignable = IsCopyAssignable<T>::value;

    // is_copy_constructible

    template<typename T>
    struct IsCopyConstructible final {
        static constexpr bool value = is_constructible<T, const T&>;
    };

    template<typename T>//
    constexpr bool is_copy_constructible = IsCopyConstructible<T>::value;

    // is_copyable

    template<typename T>
    struct IsCopyable final {
        static constexpr bool value = is_copy_assignable<T> || is_copy_constructible<T>;
    };

    template<typename T>//
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

    template<typename T>//
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

    template<typename T>//
    constexpr bool is_volatile = IsVolatile<T>::value;

    // is_cv

    template<typename T>
    struct IsCV final {
        static constexpr bool value = is_const<T> || is_volatile<T>;
    };

    template<typename T>//
    constexpr bool is_cv = IsCV<T>::value;

#ifdef BUILD_DEBUG
    static_assert(!is_cv<void>);
    static_assert(is_cv<const i32*>);
    static_assert(is_cv<const i32&>);
    static_assert(is_cv<volatile i32*>);
    static_assert(is_cv<volatile i32&>);
    static_assert(is_cv<const volatile i32*>);
    static_assert(is_cv<const volatile i32&>);
#endif

    // is_value

    template<typename T>
    struct IsValue final {
        static constexpr bool value = !is_ptr<T> && !is_ref<T>;
    };

    template<typename T>//
    constexpr bool is_value = IsValue<T>::value;

#ifdef BUILD_DEBUG
    static_assert(is_value<i32>);
    static_assert(!is_value<i32*>);
    static_assert(!is_value<i32&>);
#endif

    // is_integral

    template<typename T>
    struct IsIntegral final {
        static constexpr bool value = is_same<T, i8> || is_same<T, i16> || is_same<T, i32> || is_same<T, i64> ||
                                      is_same<T, isize> || is_same<T, u8> || is_same<T, u16> || is_same<T, u32> ||
                                      is_same<T, u64> || is_same<T, usize>;
    };

    template<typename T>
    constexpr bool is_integral = IsIntegral<T>::value;

#ifdef BUILD_DEBUG
    static_assert(is_integral<i8>);
    static_assert(is_integral<i16>);
    static_assert(!is_integral<f32>);
    static_assert(!is_integral<void>);
#endif

    // is_float

    template<typename T>
    struct IsFloat final {
        static constexpr bool value = is_same<T, f32> || is_same<T, f64> || is_same<T, f64ex>;
    };

    template<typename T>
    constexpr bool is_float = IsFloat<T>::value;

#ifdef BUILD_DEBUG
    static_assert(!is_float<i8>);
    static_assert(!is_float<i16>);
    static_assert(is_float<f32>);
    static_assert(!is_float<void>);
#endif

    // is_convertible

    namespace {
        template<typename FROM, typename TO>
        struct IsConvertibleImpl final {
            constexpr auto operator()() noexcept -> TO {
                return val<FROM>();
            }
        };
    }// namespace

    template<typename FROM, typename TO>
    struct IsConvertible : public False {
        static constexpr bool value =
                !is_void<FROM> && !is_void<TO> && is_same<decltype(IsConvertibleImpl<FROM, TO>()()), TO>;
    };

    template<typename FROM, typename TO>
    constexpr bool is_convertible = IsConvertible<FROM, TO>::value;

#ifdef BUILD_DEBUG
    static_assert(is_convertible<i32, i32>);
    static_assert(is_convertible<void*, i32*>);
    static_assert(is_convertible<i32*, void*>);
    static_assert(is_convertible<i32*, usize>);
    static_assert(is_convertible<usize, i32*>);
    static_assert(!is_convertible<void, i32*>);
    static_assert(!is_convertible<i32*, void>);
#endif
}// namespace kstd::meta