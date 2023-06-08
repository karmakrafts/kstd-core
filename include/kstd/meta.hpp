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

    // Pack

    template<typename... TYPES>
    struct Pack final {
        [[maybe_unused]] static constexpr usize num_types = sizeof...(TYPES);

        [[nodiscard, maybe_unused]] constexpr auto get_size() const noexcept -> usize {
            return num_types;
        }
    };

    // uneval

    template<typename T>
    [[nodiscard]] constexpr auto uneval() noexcept -> T {
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
    KSTD_SFINAE_TRAIT(IsDestructible, is_destructible, is_void<decltype(uneval<T>().~T())>)
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

    // NonRef

    template<typename T>
    struct NonRefImpl {
        using Type = T;
    };

    template<typename T>
    struct NonRefImpl<T&> {
        using Type = T;
    };

    template<typename T>//
    using NonRef = typename NonRefImpl<T>::Type;

    // NonPtr

    template<typename T>
    struct NonPtrImpl {
        using Type = T;
    };

    template<typename T>
    struct NonPtrImpl<T*> {
        using Type = T;
    };

    template<typename T>//
    using NonPtr = typename NonPtrImpl<T>::Type;

    // NonConst

    template<typename T>
    struct NonConstImpl {
        using Type = T;
    };

    template<typename T>
    struct NonConstImpl<const T> {
        using Type = T;
    };

    template<typename T>
    struct NonConstImpl<const T*> {
        using Type = T*;
    };

    template<typename T>
    struct NonConstImpl<const T&> {
        using Type = T&;
    };

    template<typename T>
    struct NonConstImpl<const T&&> {
        using Type = T&&;
    };

    template<typename T>//
    using NonConst = typename NonConstImpl<T>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<NonConst<i32>, i32>);
    static_assert(is_same<NonConst<const i32*>, i32*>);
    static_assert(is_same<NonConst<const i32&>, i32&>);
    static_assert(is_same<NonConst<const i32&&>, i32&&>);
#endif

    // NonVolatile

    template<typename T>
    struct NonVolatileImpl {
        using Type = T;
    };

    template<typename T>
    struct NonVolatileImpl<volatile T> {
        using Type = T;
    };

    template<typename T>
    struct NonVolatileImpl<volatile T*> {
        using Type = T*;
    };

    template<typename T>
    struct NonVolatileImpl<volatile T&> {
        using Type = T&;
    };

    template<typename T>
    struct NonVolatileImpl<volatile T&&> {
        using Type = T&&;
    };

    template<typename T>//
    using NonVolatile = typename NonVolatileImpl<T>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<NonVolatile<i32>, i32>);
    static_assert(is_same<NonVolatile<volatile i32*>, i32*>);
    static_assert(is_same<NonVolatile<volatile i32&>, i32&>);
    static_assert(is_same<NonVolatile<volatile i32&&>, i32&&>);
#endif

    // NonCV

    template<typename T>
    struct NonCVImpl final {
        using Type = NonConst<NonVolatile<T>>;
    };

    template<typename T>//
    using NonCV = typename NonCVImpl<T>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<NonCV<const i32*>, i32*>);
    static_assert(is_same<NonCV<volatile i32*>, i32*>);
    static_assert(is_same<NonCV<const volatile i32*>, i32*>);
#endif

    // Naked

    template<typename T>
    struct NakedImpl final {
        using Type = NonRef<NonPtr<NonCV<T>>>;
    };

    template<typename T>//
    using Naked = typename NakedImpl<T>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<Naked<i32>, i32>);
    static_assert(is_same<Naked<i32*>, i32>);
    static_assert(is_same<Naked<const i32*>, i32>);
    static_assert(is_same<Naked<i32&>, i32>);
    static_assert(is_same<Naked<const i32&>, i32>);
#endif

    // Ref

    template<typename T>
    struct RefImpl final {
        using Type = T&;
    };

    template<typename T>//
    using Ref = typename RefImpl<T>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<Ref<i32>, i32&>);
#endif

    // ConstRef

    template<typename T>
    struct ConstRefImpl final {
        using Type = T const&;
    };

    template<typename T>//
    using ConstRef = typename ConstRefImpl<T>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<ConstRef<i32>, const i32&>);
#endif

    // TempRef

    template<typename T>
    struct TempRefImpl final {
        using Type = T&&;
    };

    template<typename T>//
    using TempRef = typename TempRefImpl<T>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<TempRef<i32>, i32&&>);
#endif

    // Const

    template<typename T>
    struct ConstImpl final {
        using Type = const T;
    };

    template<typename T>
    struct ConstImpl<T*> final {
        using Type = const T*;
    };

    template<typename T>
    struct ConstImpl<T&> final {
        using Type = const T&;
    };

    template<typename T>
    struct ConstImpl<T&&> final {
        using Type = const T&&;
    };

    template<typename T>//
    using Const = typename ConstImpl<T>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<Const<i32*>, const i32*>);
    static_assert(is_same<Const<i32&>, const i32&>);
    static_assert(is_same<Const<i32&&>, const i32&&>);
#endif

    // Volatile

    template<typename T>
    struct VolatileImpl final {
        using Type = volatile T;
    };

    template<typename T>
    struct VolatileImpl<T*> final {
        using Type = volatile T*;
    };

    template<typename T>
    struct VolatileImpl<T&> final {
        using Type = volatile T&;
    };

    template<typename T>
    using Volatile = typename VolatileImpl<T>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<Volatile<i32>, volatile i32>);
    static_assert(is_same<Volatile<i32*>, volatile i32*>);
    static_assert(is_same<Volatile<i32&>, volatile i32&>);
#endif

    // Ptr

    template<typename T>
    struct PtrImpl final {
        using Type = T*;
    };

    template<typename T>
    using Ptr = typename PtrImpl<T>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<Ptr<void>, void*>);
    static_assert(is_same<Ptr<void*>, void**>);
#endif

    // has_add_op

    KSTD_SFINAE_TRAIT(HasAddOp, has_add_op, (is_same<T, Naked<decltype(uneval<T>() + uneval<T>())>>) )
#ifdef BUILD_DEBUG
    static_assert(!has_add_op<void>);
    static_assert(has_add_op<i32>);
#endif

    // has_sub_op

    KSTD_SFINAE_TRAIT(HasSubOp, has_sub_op, (is_same<T, Naked<decltype(uneval<T>() - uneval<T>())>>) )
#ifdef BUILD_DEBUG
    static_assert(!has_sub_op<void>);
    static_assert(has_sub_op<i32>);
#endif

    // has_mul_op

    KSTD_SFINAE_TRAIT(HasMulOp, has_mul_op, (is_same<T, Naked<decltype(uneval<T>() * uneval<T>())>>) )
#ifdef BUILD_DEBUG
    static_assert(!has_mul_op<void>);
    static_assert(has_mul_op<i32>);
#endif

    // has_div_op

    KSTD_SFINAE_TRAIT(HasDivOp, has_div_op, (is_same<T, Naked<decltype(uneval<T>() / uneval<T>())>>) )
#ifdef BUILD_DEBUG
    static_assert(!has_div_op<void>);
    static_assert(has_div_op<i32>);
#endif

    // has_mod_op

    KSTD_SFINAE_TRAIT(HasModOp, has_mod_op, (is_same<T, Naked<decltype(uneval<T>() % uneval<T>())>>) )
#ifdef BUILD_DEBUG
    static_assert(!has_mod_op<void>);
    static_assert(has_mod_op<i32>);
#endif

    // has_shl_op

    KSTD_SFINAE_TRAIT(HasShlOp, has_shl_op, (is_same<T, Naked<decltype(uneval<T>() << uneval<i32>())>>) )
#ifdef BUILD_DEBUG
    static_assert(!has_shl_op<void>);
    static_assert(has_shl_op<i32>);
#endif

    // has_shr_op

    KSTD_SFINAE_TRAIT(HasShrOp, has_shr_op, (is_same<T, Naked<decltype(uneval<T>() >> uneval<i32>())>>) )
#ifdef BUILD_DEBUG
    static_assert(!has_shr_op<void>);
    static_assert(has_shr_op<i32>);
#endif

    // has_and_op

    KSTD_SFINAE_TRAIT(HasAndOp, has_and_op, (is_same<T, Naked<decltype(uneval<T>() & uneval<T>())>>) )
#ifdef BUILD_DEBUG
    static_assert(!has_and_op<void>);
    static_assert(has_and_op<i32>);
#endif

    // has_or_op

    KSTD_SFINAE_TRAIT(HasOrOp, has_or_op, (is_same<T, Naked<decltype(uneval<T>() | uneval<T>())>>) )
#ifdef BUILD_DEBUG
    static_assert(!has_or_op<void>);
    static_assert(has_or_op<i32>);
#endif

    // has_xor_op

    KSTD_SFINAE_TRAIT(HasXorOp, has_xor_op, (is_same<T, Naked<decltype(uneval<T>() ^ uneval<T>())>>) )
#ifdef BUILD_DEBUG
    static_assert(!has_xor_op<void>);
    static_assert(has_xor_op<i32>);
#endif

    // has_inv_op

    KSTD_SFINAE_TRAIT(HasInvOp, has_inv_op, (is_same<T, Naked<decltype(~uneval<T>())>>) )
#ifdef BUILD_DEBUG
    static_assert(!has_inv_op<void>);
    static_assert(has_inv_op<i32>);
#endif

    // has_equals_op

    KSTD_SFINAE_TRAIT(HasEqualsOp, has_equals_op, (is_same<Naked<decltype(uneval<T>() == uneval<T>())>, bool>) )
#ifdef BUILD_DEBUG
    static_assert(!has_equals_op<void>);
    static_assert(has_equals_op<i32>);
#endif

    // has_not_equals_op

    KSTD_SFINAE_TRAIT(HasNotEqualsOp, has_not_equals_op, (is_same<Naked<decltype(uneval<T>() != uneval<T>())>, bool>) )
#ifdef BUILD_DEBUG
    static_assert(!has_not_equals_op<void>);
    static_assert(has_not_equals_op<i32>);
#endif

    // PackElement

    template<usize INDEX, typename PACK>
    struct PackElementImpl;

    template<usize INDEX, typename HEAD, typename... TAIL>
    struct PackElementImpl<INDEX, Pack<HEAD, TAIL...>> : public PackElementImpl<INDEX - 1, Pack<TAIL...>> {};

    template<typename HEAD, typename... TAIL>
    struct PackElementImpl<0, Pack<HEAD, TAIL...>> {
        using Type = HEAD;
    };

    template<usize INDEX, typename PACK>//
    using PackElement = typename PackElementImpl<INDEX, PACK>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<PackElement<0, Pack<i32, f32, u32>>, i32>, "Type should be i32");
    static_assert(is_same<PackElement<1, Pack<i32, f32, u32>>, f32>, "Type should be f32");
    static_assert(is_same<PackElement<2, Pack<i32, f32, u32>>, u32>, "Type should be u32");
#endif

    // LeftTrimPack

    template<usize COUNT, typename PACK, typename = Pack<>>
    struct LeftTrimPackImpl {
        using Type = Pack<>;
    };

    template<usize COUNT, typename HEAD, typename... TAIL, typename... SKIPPED>
    struct LeftTrimPackImpl<COUNT, Pack<HEAD, TAIL...>, Pack<SKIPPED...>>
            : public LeftTrimPackImpl<COUNT - 1, Pack<TAIL...>, Pack<SKIPPED..., HEAD>> {};

    template<typename HEAD, typename... TAIL, typename... SKIPPED>
    struct LeftTrimPackImpl<0, Pack<HEAD, TAIL...>, Pack<SKIPPED...>> {
        using Type = Pack<HEAD, TAIL...>;
    };

    template<typename... SKIPPED>
    struct LeftTrimPackImpl<0, Pack<>, Pack<SKIPPED...>> {
        using Type = Pack<>;
    };

    template<usize COUNT, typename PACK>//
    using LeftTrimPack = typename LeftTrimPackImpl<COUNT, PACK>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<LeftTrimPack<0, Pack<>>, Pack<>>, "Pack type should be Pack<>");
    static_assert(is_same<LeftTrimPack<1, Pack<>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(is_same<LeftTrimPack<0, Pack<i32, f32, u64>>, Pack<i32, f32, u64>>,
                  "Pack type should be Pack<i32, f32, u64>");
    static_assert(is_same<LeftTrimPack<1, Pack<i32, f32, u64>>, Pack<f32, u64>>, "Pack type should be Pack<f32, u64>");
    static_assert(is_same<LeftTrimPack<2, Pack<i32, f32, u64>>, Pack<u64>>, "Pack type should be Pack<u64>");
    static_assert(is_same<LeftTrimPack<3, Pack<i32, f32, u64>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(LeftTrimPack<0, Pack<i32, f32, u64>>().get_size() == 3, "Pack size should be 3");
    static_assert(LeftTrimPack<1, Pack<i32, f32, u64>>().get_size() == 2, "Pack size should be 2");
    static_assert(LeftTrimPack<2, Pack<i32, f32, u64>>().get_size() == 1, "Pack size should be 1");
    static_assert(LeftTrimPack<3, Pack<i32, f32, u64>>().get_size() == 0, "Pack size should be 0");
#endif

    // RightTrimPack

    template<usize COUNT, typename PACK, typename = Pack<>>
    struct RightTrimPackImpl {
        using Type = Pack<>;
    };

    template<usize COUNT, typename HEAD, typename... TAIL, typename... ACCUMULATED>
    struct RightTrimPackImpl<COUNT, Pack<HEAD, TAIL...>, Pack<ACCUMULATED...>>
            : public RightTrimPackImpl<COUNT - 1, Pack<TAIL...>, Pack<ACCUMULATED..., HEAD>> {};

    template<typename HEAD, typename... TAIL, typename... ACCUMULATED>
    struct RightTrimPackImpl<0, Pack<HEAD, TAIL...>, Pack<ACCUMULATED...>> {
        using Type = Pack<ACCUMULATED...>;
    };

    template<typename... ACCUMULATED>
    struct RightTrimPackImpl<0, Pack<>, Pack<ACCUMULATED...>> {
        using Type = Pack<ACCUMULATED...>;
    };

    template<usize COUNT, typename PACK>//
    using RightTrimPack = typename RightTrimPackImpl<COUNT, PACK>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<RightTrimPack<0, Pack<>>, Pack<>>, "Pack type should be Pack<>");
    static_assert(is_same<RightTrimPack<1, Pack<>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(is_same<RightTrimPack<3, Pack<i32, f32, u64>>, Pack<i32, f32, u64>>,
                  "Pack type should be Pack<i32, f32, u64>");
    static_assert(is_same<RightTrimPack<2, Pack<i32, f32, u64>>, Pack<i32, f32>>, "Pack type should be Pack<i32, f32>");
    static_assert(is_same<RightTrimPack<1, Pack<i32, f32, u64>>, Pack<i32>>, "Pack type should be Pack<i32>");
    static_assert(is_same<RightTrimPack<0, Pack<i32, f32, u64>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(RightTrimPack<0, Pack<i32, f32, u64>>().get_size() == 0, "Pack size should be 0");
    static_assert(RightTrimPack<1, Pack<i32, f32, u64>>().get_size() == 1, "Pack size should be 1");

    static_assert(RightTrimPack<2, Pack<i32, f32, u64>>().get_size() == 2, "Pack size should be 2");
    static_assert(RightTrimPack<3, Pack<i32, f32, u64>>().get_size() == 3, "Pack size should be 3");
#endif

    // SlicePack

    template<usize BEGIN, usize END, typename PACK>
    struct SlicePackImpl;

    template<usize BEGIN, usize END, typename... TYPES>
    struct SlicePackImpl<BEGIN, END, Pack<TYPES...>> {
        using Type = RightTrimPack<END + 1 - BEGIN, LeftTrimPack<BEGIN, Pack<TYPES...>>>;
    };

    template<usize BEGIN, usize END>
    struct SlicePackImpl<BEGIN, END, Pack<>> {
        using Type = Pack<>;
    };

    template<usize BEGIN, usize END, typename PACK>//
    using SlicePack = typename SlicePackImpl<BEGIN, END, PACK>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<SlicePack<0, 0, Pack<>>, Pack<>>, "Pack type should be Pack<>");
    static_assert(is_same<SlicePack<0, 1, Pack<>>, Pack<>>, "Pack type should be Pack<>");
    static_assert(is_same<SlicePack<1, 2, Pack<>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(is_same<SlicePack<0, 3, Pack<i32, f32, u64, u32>>, Pack<i32, f32, u64, u32>>,
                  "Pack type should be Pack<i32, f32, u64, u32>");
    static_assert(is_same<SlicePack<0, 2, Pack<i32, f32, u64, u32>>, Pack<i32, f32, u64>>,
                  "Pack type should be Pack<i32, f32, u64>");
    static_assert(is_same<SlicePack<0, 1, Pack<i32, f32, u64, u32>>, Pack<i32, f32>>,
                  "Pack type should be Pack<i32, f32>");
    static_assert(is_same<SlicePack<0, 0, Pack<i32, f32, u64, u32>>, Pack<i32>>, "Pack type should be Pack<i32>");

    static_assert(is_same<SlicePack<0, 3, Pack<i32, f32, u64, u32>>, Pack<i32, f32, u64, u32>>,
                  "Pack type should be Pack<i32, f32, u64, u32>");
    static_assert(is_same<SlicePack<1, 3, Pack<i32, f32, u64, u32>>, Pack<f32, u64, u32>>,
                  "Pack type should be Pack<f32, u64, u32>");
    static_assert(is_same<SlicePack<2, 3, Pack<i32, f32, u64, u32>>, Pack<u64, u32>>,
                  "Pack type should be Pack<u64, u32>");
    static_assert(is_same<SlicePack<3, 3, Pack<i32, f32, u64, u32>>, Pack<u32>>, "Pack type should be Pack<u32>");

    static_assert(SlicePack<0, 3, Pack<i32, f32, u64, u32>>().get_size() == 4, "Pack size should be 4");
    static_assert(SlicePack<0, 2, Pack<i32, f32, u64, u32>>().get_size() == 3, "Pack size should be 3");
    static_assert(SlicePack<0, 1, Pack<i32, f32, u64, u32>>().get_size() == 2, "Pack size should be 2");
    static_assert(SlicePack<0, 0, Pack<i32, f32, u64, u32>>().get_size() == 1, "Pack size should be 1");
#endif

    // ConcatPacks

    template<typename PACK_A, typename PACK_B>
    struct ConcatPacksImpl;

    template<typename... TYPES_A, typename... TYPES_B>
    struct ConcatPacksImpl<Pack<TYPES_A...>, Pack<TYPES_B...>> {
        using Type = Pack<TYPES_A..., TYPES_B...>;
    };

    template<typename PACK_A, typename PACK_B>//
    using ConcatPacks = typename ConcatPacksImpl<PACK_A, PACK_B>::Type;

#ifdef BUILD_DEBUG
    static_assert(is_same<ConcatPacks<Pack<>, Pack<>>, Pack<>>, "Pack type should be Pack<>");

    static_assert(is_same<ConcatPacks<Pack<>, Pack<i32, f32>>, Pack<i32, f32>>, "Pack type should be Pack<i32, f32>");
    static_assert(is_same<ConcatPacks<Pack<i32, f32>, Pack<>>, Pack<i32, f32>>, "Pack type should be Pack<i32, f32>");
    static_assert(is_same<ConcatPacks<Pack<i32, f32>, Pack<i32, f32>>, Pack<i32, f32, i32, f32>>,
                  "Pack type should be Pack<i32, f32, i32, f32>");
#endif

    // TransformPack

    template<template<typename> typename TRANSFORM, typename PACK>
    struct TransformPackImpl;

    template<template<typename> typename TRANSFORM, typename... TYPES>//
    struct TransformPackImpl<TRANSFORM, Pack<TYPES...>> {
        using Type = Pack<typename TRANSFORM<TYPES>::Type...>;
    };

    template<template<typename> typename TRANSFORM, typename... TYPES>//
    using TransformPack = typename TransformPackImpl<TRANSFORM, TYPES...>::Type;

    // substitute_void

    template<typename T, typename SUBSTITUTE>//
    using IfVoid = If<is_void<T>, SUBSTITUTE, T>;
}// namespace kstd::meta