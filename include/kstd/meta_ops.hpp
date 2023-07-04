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

#include "meta_types.hpp"

namespace kstd::meta {
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
}// namespace kstd::meta