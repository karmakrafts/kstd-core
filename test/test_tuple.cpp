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
#include <type_traits>

TEST(kstd_Tuple, TestValues) {
    kstd::Pair<kstd::i32, kstd::f32> pair(1337, 3.141F);

    // Template get<>
    const auto& a = pair.get<0>();
    static_assert(std::is_same_v<std::decay_t<decltype(a)>, kstd::i32>);
    ASSERT_EQ(a, 1337);

    const auto& b = pair.get<1>();
    static_assert(std::is_same_v<std::decay_t<decltype(b)>, kstd::f32>);
    ASSERT_EQ(b, 3.141F);

    // std::tuple_size support
    const auto size = std::tuple_size<decltype(pair)>::value;
    ASSERT_EQ(size, pair.get_size());

    // Structured bindings
    const auto& [x, y] = pair;
    static_assert(std::is_same_v<std::decay_t<decltype(x)>, kstd::i32>);
    static_assert(std::is_same_v<std::decay_t<decltype(y)>, kstd::f32>);
    ASSERT_EQ(x, 1337);
    ASSERT_EQ(y, 3.141F);

    // Equality
    kstd::Pair<kstd::i32, kstd::f32> pair2(1337, 3.141F);
    ASSERT_TRUE(pair == pair2);

    kstd::Pair<kstd::i32, kstd::f32> pair3(13, 3.1F);
    ASSERT_TRUE(pair != pair3);

    // Concat
    auto quad = pair + pair;
    auto& [qx, qy, qz, qw] = quad;

    static_assert(std::is_same_v<std::decay_t<decltype(qx)>, kstd::i32>);
    static_assert(std::is_same_v<std::decay_t<decltype(qy)>, kstd::f32>);
    static_assert(std::is_same_v<std::decay_t<decltype(qz)>, kstd::i32>);
    static_assert(std::is_same_v<std::decay_t<decltype(qw)>, kstd::f32>);

    ASSERT_EQ(qx, 1337);
    ASSERT_EQ(qy, 3.141F);
    ASSERT_EQ(qz, 1337);
    ASSERT_EQ(qw, 3.141F);

    qx = 200;
    qy = 6.666F;
    qz = 444;
    qw = 6.21F;

    ASSERT_EQ(quad.get<0>(), 200);
    ASSERT_EQ(quad.get<1>(), 6.666F);
    ASSERT_EQ(quad.get<2>(), 444);
    ASSERT_EQ(quad.get<3>(), 6.21F);
}

TEST(kstd_Tuple, TestReferences) {
    kstd::i32 valueA = 1337;
    kstd::f32 valueB = 3.141F;
    kstd::Pair<kstd::i32&, kstd::f32&> pair(valueA, valueB);

    // Template get<>
    const auto x_a = pair.get<0>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_a)>, kstd::i32>);
    ASSERT_EQ(valueA, 1337);

    const auto x_b = pair.get<1>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_b)>, kstd::f32>);
    ASSERT_EQ(valueB, 3.141F);

    // std::tuple_size support
    const auto size = std::tuple_size<decltype(pair)>::value;
    ASSERT_EQ(size, pair.get_size());

    // Structured bindings
    auto& [x, y] = pair;
    static_assert(std::is_same_v<std::decay_t<decltype(x)>, kstd::i32>);
    static_assert(std::is_same_v<std::decay_t<decltype(y)>, kstd::f32>);
    ASSERT_EQ(x, 1337);
    ASSERT_EQ(y, 3.141F);

    // Equality
    kstd::Pair<kstd::i32&, kstd::f32&> pair2(valueA, valueB);
    ASSERT_TRUE(pair == pair2);

    kstd::i32 valueA2 = 13;
    kstd::f32 valueB2 = 3.1F;
    kstd::Pair<kstd::i32&, kstd::f32&> pair3(valueA2, valueB2);
    ASSERT_TRUE(pair != pair3);

    // Concat
    auto quad = pair + pair;
    auto& [qx, qy, qz, qw] = quad;

    static_assert(std::is_same_v<decltype(qx), kstd::i32&>);
    static_assert(std::is_same_v<decltype(qy), kstd::f32&>);
    static_assert(std::is_same_v<decltype(qz), kstd::i32&>);
    static_assert(std::is_same_v<decltype(qw), kstd::f32&>);

    ASSERT_EQ(qx, 1337);
    ASSERT_EQ(qy, 3.141F);
    ASSERT_EQ(qz, 1337);
    ASSERT_EQ(qw, 3.141F);

    qx = 200;
    qy = 6.21F;

    ASSERT_EQ(valueA, 200);
    ASSERT_EQ(valueB, 6.21F);
}

TEST(kstd_Tuple, TestConstReferences) {
    kstd::i32 valueA = 1337;
    kstd::f32 valueB = 3.141F;
    kstd::Pair<const kstd::i32&, const kstd::f32&> pair(valueA, valueB);

    // Template get<>
    const auto& x_a = pair.get<0>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_a)>, kstd::i32>);
    ASSERT_EQ(valueA, 1337);

    const auto& x_b = pair.get<1>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_b)>, kstd::f32>);
    ASSERT_EQ(valueB, 3.141F);

    // std::tuple_size support
    const auto size = std::tuple_size<decltype(pair)>::value;
    ASSERT_EQ(size, pair.get_size());

    // Structured bindings
    const auto& [x, y] = pair;
    static_assert(std::is_same_v<std::decay_t<decltype(x)>, kstd::i32>);
    static_assert(std::is_same_v<std::decay_t<decltype(y)>, kstd::f32>);
    ASSERT_EQ(x, 1337);
    ASSERT_EQ(y, 3.141F);

    // Equality
    kstd::Pair<const kstd::i32&, const kstd::f32&> pair2(valueA, valueB);
    ASSERT_TRUE(pair == pair2);

    kstd::i32 valueA2 = 13;
    kstd::f32 valueB2 = 3.1F;
    kstd::Pair<const kstd::i32&, const kstd::f32&> pair3(valueA2, valueB2);
    ASSERT_TRUE(pair != pair3);

    // Concat
    const auto quad = pair + pair;
    const auto& [qx, qy, qz, qw] = quad;

    static_assert(std::is_same_v<decltype(qx), const kstd::i32&>);
    static_assert(std::is_same_v<decltype(qy), const kstd::f32&>);
    static_assert(std::is_same_v<decltype(qz), const kstd::i32&>);
    static_assert(std::is_same_v<decltype(qw), const kstd::f32&>);

    ASSERT_EQ(qx, 1337);
    ASSERT_EQ(qy, 3.141F);
    ASSERT_EQ(qz, 1337);
    ASSERT_EQ(qw, 3.141F);
}

TEST(kstd_Tuple, TestPointers) {
    kstd::i32 valueA = 1337;
    kstd::f32 valueB = 3.141F;
    kstd::Pair<kstd::i32*, kstd::f32*> pair(&valueA, &valueB);

    // Template get<>
    const auto* x_a = pair.get<0>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_a)>, const kstd::i32*>);
    ASSERT_EQ(valueA, 1337);

    const auto* x_b = pair.get<1>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_b)>, const kstd::f32*>);
    ASSERT_EQ(valueB, 3.141F);

    // std::tuple_size support
    const auto size = std::tuple_size<decltype(pair)>::value;
    ASSERT_EQ(size, pair.get_size());

    // Structured bindings
    auto& [x, y] = pair;
    static_assert(std::is_same_v<std::decay_t<decltype(x)>, kstd::i32*>);
    static_assert(std::is_same_v<std::decay_t<decltype(y)>, kstd::f32*>);
    ASSERT_EQ(*x, 1337);
    ASSERT_EQ(*y, 3.141F);

    // Equality
    kstd::Pair<kstd::i32*, kstd::f32*> pair2(&valueA, &valueB);
    ASSERT_TRUE(pair == pair2);

    kstd::Pair<kstd::i32*, kstd::f32*> pair3(&valueA, nullptr);
    ASSERT_TRUE(pair != pair3);

    // Concat
    auto quad = pair + pair;
    auto& [qx, qy, qz, qw] = quad;

    static_assert(std::is_same_v<decltype(qx), kstd::i32*>);
    static_assert(std::is_same_v<decltype(qy), kstd::f32*>);
    static_assert(std::is_same_v<decltype(qz), kstd::i32*>);
    static_assert(std::is_same_v<decltype(qw), kstd::f32*>);

    ASSERT_EQ(*qx, 1337);
    ASSERT_EQ(*qy, 3.141F);
    ASSERT_EQ(*qz, 1337);
    ASSERT_EQ(*qw, 3.141F);

    *qx = 200;
    *qy = 6.21F;

    ASSERT_EQ(valueA, 200);
    ASSERT_EQ(valueB, 6.21F);
}

TEST(kstd_Tuple, TestConstPointers) {
    kstd::i32 valueA = 1337;
    kstd::f32 valueB = 3.141F;
    kstd::Pair<const kstd::i32*, const kstd::f32*> pair(&valueA, &valueB);

    // Template get<>
    const auto* x_a = pair.get<0>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_a)>, const kstd::i32*>);
    ASSERT_EQ(valueA, 1337);

    const auto* x_b = pair.get<1>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_b)>, const kstd::f32*>);
    ASSERT_EQ(valueB, 3.141F);

    // std::tuple_size support
    const auto size = std::tuple_size<decltype(pair)>::value;
    ASSERT_EQ(size, pair.get_size());

    // Structured bindings
    auto& [x, y] = pair;
    static_assert(std::is_same_v<std::decay_t<decltype(x)>, const kstd::i32*>);
    static_assert(std::is_same_v<std::decay_t<decltype(y)>, const kstd::f32*>);
    ASSERT_EQ(*x, 1337);
    ASSERT_EQ(*y, 3.141F);

    // Equality
    kstd::Pair<const kstd::i32*, const kstd::f32*> pair2(&valueA, &valueB);
    ASSERT_TRUE(pair == pair2);

    kstd::Pair<const kstd::i32*, const kstd::f32*> pair3(&valueA, nullptr);
    ASSERT_TRUE(pair != pair3);

    // Concat
    auto quad = pair + pair;
    auto& [qx, qy, qz, qw] = quad;

    static_assert(std::is_same_v<decltype(qx), const kstd::i32*>);
    static_assert(std::is_same_v<decltype(qy), const kstd::f32*>);
    static_assert(std::is_same_v<decltype(qz), const kstd::i32*>);
    static_assert(std::is_same_v<decltype(qw), const kstd::f32*>);

    ASSERT_EQ(*qx, 1337);
    ASSERT_EQ(*qy, 3.141F);
    ASSERT_EQ(*qz, 1337);
    ASSERT_EQ(*qw, 3.141F);
}

TEST(kstd_Tuple, TestSlice) {
    kstd::Tuple<kstd::i8, kstd::i16, kstd::i32, kstd::i64> tuple(1, 2, 3, 4);

    const auto [x, y] = tuple.slice<0, 1>();
    static_assert(std::is_same_v<std::decay_t<decltype(x)>, kstd::i8>);
    static_assert(std::is_same_v<std::decay_t<decltype(y)>, kstd::i16>);
    ASSERT_EQ(x, 1);
    ASSERT_EQ(y, 2);

    const auto [z, w] = tuple.slice<2, 3>();
    static_assert(std::is_same_v<std::decay_t<decltype(z)>, kstd::i32>);
    static_assert(std::is_same_v<std::decay_t<decltype(w)>, kstd::i64>);
    ASSERT_EQ(z, 3);
    ASSERT_EQ(w, 4);
}