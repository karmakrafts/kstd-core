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

#include <gtest/gtest.h>
#include <kstd/tuple.hpp>

TEST(kstd_Tuple, TestValues) {
    kstd::Pair<kstd::i32, kstd::f32> pair(1337, 3.141F);

    // Template get<>
    const auto a = pair.get<0>();
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(a)>, kstd::i32>);
    ASSERT_EQ(a, 1337);

    const auto b = pair.get<1>();
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(b)>, kstd::f32>);
    ASSERT_EQ(b, 3.141F);

    // std::tuple_size support
    const auto size = std::tuple_size<decltype(pair)>::value;
    ASSERT_EQ(size, pair.get_size());

    // Structured bindings
    const auto& [x, y] = pair;
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(x)>, kstd::i32>);
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(y)>, kstd::f32>);
    ASSERT_EQ(x, 1337);
    ASSERT_EQ(y, 3.141F);

    // Equality
    kstd::Pair<kstd::i32, kstd::f32> pair2(1337, 3.141F);
    ASSERT_TRUE(pair == pair2);

    kstd::Pair<kstd::i32, kstd::f32> pair3(13, 3.1F);
    ASSERT_TRUE(pair != pair3);

    // Concat
    const auto quad = pair + pair;
    const auto& [qx, qy, qz, qw] = quad;

    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(qx)>, kstd::i32>);
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(qy)>, kstd::f32>);
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(qz)>, kstd::i32>);
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(qw)>, kstd::f32>);

    ASSERT_EQ(qx, 1337);
    ASSERT_EQ(qy, 3.141F);
    ASSERT_EQ(qz, 1337);
    ASSERT_EQ(qw, 3.141F);
}

TEST(kstd_Tuple, TestReferences) {
    kstd::i32 a = 1337;
    kstd::f32 b = 3.141F;
    kstd::Pair<kstd::i32&, kstd::f32&> pair(a, b);

    // Template get<>
    const auto x_a = pair.get<0>();
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(x_a)>, kstd::i32>);
    ASSERT_EQ(a, 1337);

    const auto x_b = pair.get<1>();
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(x_b)>, kstd::f32>);
    ASSERT_EQ(b, 3.141F);

    // std::tuple_size support
    const auto size = std::tuple_size<decltype(pair)>::value;
    ASSERT_EQ(size, pair.get_size());

    // Structured bindings
    auto& [x, y] = pair;
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(x)>, kstd::i32>);
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(y)>, kstd::f32>);
    ASSERT_EQ(x, 1337);
    ASSERT_EQ(y, 3.141F);

    // Equality
    kstd::Pair<kstd::i32&, kstd::f32&> pair2(a, b);
    ASSERT_TRUE(pair == pair2);

    kstd::i32 a2 = 13;
    kstd::f32 b2 = 3.1F;
    kstd::Pair<kstd::i32&, kstd::f32&> pair3(a2, b2);
    ASSERT_TRUE(pair != pair3);

    // Concat
    auto quad = pair + pair;
    auto& [qx, qy, qz, qw] = quad;

    static_assert(kstd::meta::is_same<decltype(qx), kstd::i32>);
    static_assert(kstd::meta::is_same<decltype(qy), kstd::f32>);
    static_assert(kstd::meta::is_same<decltype(qz), kstd::i32>);
    static_assert(kstd::meta::is_same<decltype(qw), kstd::f32>);

    ASSERT_EQ(qx, 1337);
    ASSERT_EQ(qy, 3.141F);
    ASSERT_EQ(qz, 1337);
    ASSERT_EQ(qw, 3.141F);
}

TEST(kstd_Tuple, TestPointers) {
    kstd::i32 a = 1337;
    kstd::f32 b = 3.141F;
    kstd::Pair<kstd::i32*, kstd::f32*> pair(&a, &b);

    // Template get<>
    const auto x_a = pair.get<0>();
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(x_a)>, kstd::i32>);
    ASSERT_EQ(a, 1337);

    const auto x_b = pair.get<1>();
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(x_b)>, kstd::f32>);
    ASSERT_EQ(b, 3.141F);

    // std::tuple_size support
    const auto size = std::tuple_size<decltype(pair)>::value;
    ASSERT_EQ(size, pair.get_size());

    // Structured bindings
    auto& [x, y] = pair;
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(x)>, kstd::i32>);
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(y)>, kstd::f32>);
    ASSERT_EQ(*x, 1337);
    ASSERT_EQ(*y, 3.141F);

    // Equality
    kstd::Pair<kstd::i32*, kstd::f32*> pair2(&a, &b);
    ASSERT_TRUE(pair == pair2);

    kstd::Pair<kstd::i32*, kstd::f32*> pair3(&a, nullptr);
    ASSERT_TRUE(pair != pair3);

    // Concat
    auto quad = pair + pair;
    auto& [qx, qy, qz, qw] = quad;

    static_assert(kstd::meta::is_same<decltype(qx), kstd::i32*>);
    static_assert(kstd::meta::is_same<decltype(qy), kstd::f32*>);
    static_assert(kstd::meta::is_same<decltype(qz), kstd::i32*>);
    static_assert(kstd::meta::is_same<decltype(qw), kstd::f32*>);

    ASSERT_EQ(*qx, 1337);
    ASSERT_EQ(*qy, 3.141F);
    ASSERT_EQ(*qz, 1337);
    ASSERT_EQ(*qw, 3.141F);
}

TEST(kstd_Tuple, TestSlice) {
    kstd::Tuple<kstd::i8, kstd::i16, kstd::i32, kstd::i64> tuple(1, 2, 3, 4);

    const auto [x, y] = tuple.slice<0, 1>();
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(x)>, kstd::i8>);
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(y)>, kstd::i16>);
    ASSERT_EQ(x, 1);
    ASSERT_EQ(y, 2);

    const auto [z, w] = tuple.slice<2, 3>();
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(z)>, kstd::i32>);
    static_assert(kstd::meta::is_same<kstd::meta::naked_type<decltype(w)>, kstd::i64>);
    ASSERT_EQ(z, 3);
    ASSERT_EQ(w, 4);
}