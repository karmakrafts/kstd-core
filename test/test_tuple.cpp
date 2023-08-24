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

using namespace kstd;

TEST(kstd_Tuple, test_values) {
    Tuple<i32, f32> pair {1337, 3.141F};

    // Template get<>
    const auto& ref_a = pair.get<0>();
    static_assert(std::is_same_v<std::decay_t<decltype(ref_a)>, i32>);
    ASSERT_EQ(ref_a, 1337);

    const auto& ref_b = pair.get<1>();
    static_assert(std::is_same_v<std::decay_t<decltype(ref_b)>, f32>);
    ASSERT_EQ(ref_b, 3.141F);

    // std::tuple_size support
    const auto size = std::tuple_size<decltype(pair)>::value;
    ASSERT_EQ(size, pair.size());

    // Structured bindings
    const auto& [x, y] = pair;
    static_assert(std::is_same_v<std::decay_t<decltype(x)>, i32>);
    static_assert(std::is_same_v<std::decay_t<decltype(y)>, f32>);
    ASSERT_EQ(x, 1337);
    ASSERT_EQ(y, 3.141F);

    // Equality
    Tuple<i32, f32> pair2 {1337, 3.141F};
    ASSERT_TRUE(pair == pair2);

    Tuple<i32, f32> pair3 {13, 3.1F};
    ASSERT_TRUE(pair != pair3);

    // Concat
    auto quad = pair + pair;
    auto& [qx, qy, qz, qw] = quad;

    static_assert(std::is_same_v<std::decay_t<decltype(qx)>, i32>);
    static_assert(std::is_same_v<std::decay_t<decltype(qy)>, f32>);
    static_assert(std::is_same_v<std::decay_t<decltype(qz)>, i32>);
    static_assert(std::is_same_v<std::decay_t<decltype(qw)>, f32>);

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

TEST(kstd_Tuple, test_references) {
    i32 value_a = 1337;
    f32 value_b = 3.141F;
    Tuple<i32&, f32&> pair {value_a, value_b};

    // Template get<>
    const auto x_a = pair.get<0>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_a)>, i32>);
    ASSERT_EQ(value_a, 1337);

    const auto x_b = pair.get<1>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_b)>, f32>);
    ASSERT_EQ(value_b, 3.141F);

    // std::tuple_size support
    const auto size = std::tuple_size<decltype(pair)>::value;
    ASSERT_EQ(size, pair.size());

    // Structured bindings
    auto& [x, y] = pair;
    static_assert(std::is_same_v<std::decay_t<decltype(x)>, i32>);
    static_assert(std::is_same_v<std::decay_t<decltype(y)>, f32>);
    ASSERT_EQ(x, 1337);
    ASSERT_EQ(y, 3.141F);

    // Equality
    Tuple<i32&, f32&> pair2 {value_a, value_b};
    ASSERT_TRUE(pair == pair2);

    i32 value_a2 = 13;
    f32 value_b2 = 3.1F;
    Tuple<i32&, f32&> pair3 {value_a2, value_b2};
    ASSERT_TRUE(pair != pair3);

    // Concat
    auto quad = pair + pair;
    auto& [qx, qy, qz, qw] = quad;

    static_assert(std::is_same_v<decltype(qx), i32&>);
    static_assert(std::is_same_v<decltype(qy), f32&>);
    static_assert(std::is_same_v<decltype(qz), i32&>);
    static_assert(std::is_same_v<decltype(qw), f32&>);

    ASSERT_EQ(qx, 1337);
    ASSERT_EQ(qy, 3.141F);
    ASSERT_EQ(qz, 1337);
    ASSERT_EQ(qw, 3.141F);

    qx = 200;
    qy = 6.21F;

    ASSERT_EQ(value_a, 200);
    ASSERT_EQ(value_b, 6.21F);
}

TEST(kstd_Tuple, test_const_references) {
    i32 value_a = 1337;
    f32 value_b = 3.141F;
    Tuple<const i32&, const f32&> pair {value_a, value_b};

    // Template get<>
    const auto& x_a = pair.get<0>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_a)>, i32>);
    ASSERT_EQ(value_a, 1337);

    const auto& x_b = pair.get<1>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_b)>, f32>);
    ASSERT_EQ(value_b, 3.141F);

    // std::tuple_size support
    const auto size = std::tuple_size<decltype(pair)>::value;
    ASSERT_EQ(size, pair.size());

    // Structured bindings
    const auto& [x, y] = pair;
    static_assert(std::is_same_v<std::decay_t<decltype(x)>, i32>);
    static_assert(std::is_same_v<std::decay_t<decltype(y)>, f32>);
    ASSERT_EQ(x, 1337);
    ASSERT_EQ(y, 3.141F);

    // Equality
    Tuple<const i32&, const f32&> pair2 {value_a, value_b};
    ASSERT_TRUE(pair == pair2);

    i32 value_a2 = 13;
    f32 value_b2 = 3.1F;
    Tuple<const i32&, const f32&> pair3 {value_a2, value_b2};
    ASSERT_TRUE(pair != pair3);

    // Concat
    const auto quad = pair + pair;
    const auto& [qx, qy, qz, qw] = quad;

    static_assert(std::is_same_v<decltype(qx), const i32&>);
    static_assert(std::is_same_v<decltype(qy), const f32&>);
    static_assert(std::is_same_v<decltype(qz), const i32&>);
    static_assert(std::is_same_v<decltype(qw), const f32&>);

    ASSERT_EQ(qx, 1337);
    ASSERT_EQ(qy, 3.141F);
    ASSERT_EQ(qz, 1337);
    ASSERT_EQ(qw, 3.141F);
}

TEST(kstd_Tuple, test_pointers) {
    i32 value_a = 1337;
    f32 value_b = 3.141F;
    Tuple<i32*, f32*> pair {&value_a, &value_b};

    // Template get<>
    const auto* x_a = pair.get<0>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_a)>, const i32*>);
    ASSERT_EQ(value_a, 1337);

    const auto* x_b = pair.get<1>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_b)>, const f32*>);
    ASSERT_EQ(value_b, 3.141F);

    // std::tuple_size support
    const auto size = std::tuple_size<decltype(pair)>::value;
    ASSERT_EQ(size, pair.size());

    // Structured bindings
    auto& [x, y] = pair;
    static_assert(std::is_same_v<std::decay_t<decltype(x)>, i32*>);
    static_assert(std::is_same_v<std::decay_t<decltype(y)>, f32*>);
    ASSERT_EQ(*x, 1337);
    ASSERT_EQ(*y, 3.141F);

    // Equality
    Tuple<i32*, f32*> pair2 {&value_a, &value_b};
    ASSERT_TRUE(pair == pair2);

    Tuple<i32*, f32*> pair3 {&value_a, nullptr};
    ASSERT_TRUE(pair != pair3);

    // Concat
    auto quad = pair + pair;
    auto& [qx, qy, qz, qw] = quad;

    static_assert(std::is_same_v<decltype(qx), i32*>);
    static_assert(std::is_same_v<decltype(qy), f32*>);
    static_assert(std::is_same_v<decltype(qz), i32*>);
    static_assert(std::is_same_v<decltype(qw), f32*>);

    ASSERT_EQ(*qx, 1337);
    ASSERT_EQ(*qy, 3.141F);
    ASSERT_EQ(*qz, 1337);
    ASSERT_EQ(*qw, 3.141F);

    *qx = 200;
    *qy = 6.21F;

    ASSERT_EQ(value_a, 200);
    ASSERT_EQ(value_b, 6.21F);
}

TEST(kstd_Tuple, test_const_pointers) {
    i32 value_a = 1337;
    f32 value_b = 3.141F;
    Tuple<const i32*, const f32*> pair {&value_a, &value_b};

    // Template get<>
    const auto* x_a = pair.get<0>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_a)>, const i32*>);
    ASSERT_EQ(value_a, 1337);

    const auto* x_b = pair.get<1>();
    static_assert(std::is_same_v<std::decay_t<decltype(x_b)>, const f32*>);
    ASSERT_EQ(value_b, 3.141F);

    // std::tuple_size support
    const auto size = std::tuple_size<decltype(pair)>::value;
    ASSERT_EQ(size, pair.size());

    // Structured bindings
    auto& [x, y] = pair;
    static_assert(std::is_same_v<std::decay_t<decltype(x)>, const i32*>);
    static_assert(std::is_same_v<std::decay_t<decltype(y)>, const f32*>);
    ASSERT_EQ(*x, 1337);
    ASSERT_EQ(*y, 3.141F);

    // Equality
    Tuple<const i32*, const f32*> pair2 {&value_a, &value_b};
    ASSERT_TRUE(pair == pair2);

    Tuple<const i32*, const f32*> pair3 {&value_a, nullptr};
    ASSERT_TRUE(pair != pair3);

    // Concat
    auto quad = pair + pair;
    auto& [qx, qy, qz, qw] = quad;

    static_assert(std::is_same_v<decltype(qx), const i32*>);
    static_assert(std::is_same_v<decltype(qy), const f32*>);
    static_assert(std::is_same_v<decltype(qz), const i32*>);
    static_assert(std::is_same_v<decltype(qw), const f32*>);

    ASSERT_EQ(*qx, 1337);
    ASSERT_EQ(*qy, 3.141F);
    ASSERT_EQ(*qz, 1337);
    ASSERT_EQ(*qw, 3.141F);
}

TEST(kstd_Tuple, test_slice) {
    Tuple<i8, i16, i32, i64> tuple {1, 2, 3, 4};

    const auto [x, y] = tuple.slice<0, 1>();
    static_assert(std::is_same_v<std::decay_t<decltype(x)>, i8>);
    static_assert(std::is_same_v<std::decay_t<decltype(y)>, i16>);
    ASSERT_EQ(x, 1);
    ASSERT_EQ(y, 2);

    const auto [z, w] = tuple.slice<2, 3>();
    static_assert(std::is_same_v<std::decay_t<decltype(z)>, i32>);
    static_assert(std::is_same_v<std::decay_t<decltype(w)>, i64>);
    ASSERT_EQ(z, 3);
    ASSERT_EQ(w, 4);
}