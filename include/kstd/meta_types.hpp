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

#include "meta.hpp"

namespace kstd::meta {
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
}// namespace kstd::meta