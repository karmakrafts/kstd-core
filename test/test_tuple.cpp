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
#include <tuple>

TEST(kstd_Tuple, TestValues) {
    kstd::Pair<kstd::i32, kstd::f32> pair(1337, 3.141F);

    // Template get<>
    const auto a = pair.get<0>();
    ASSERT_TRUE((std::is_same<typename std::decay<decltype(a)>::type, kstd::i32>::value));
    ASSERT_EQ(a, 1337);
    const auto b = pair.get<1>();
    ASSERT_TRUE((std::is_same<typename std::decay<decltype(b)>::type, kstd::f32>::value));
    ASSERT_EQ(b, 3.141F);

    // std::tuple_size support
    const auto size = std::tuple_size<decltype(pair)>::value;
    ASSERT_EQ(size, pair.get_size());

    // Structured bindings
    auto& [x, y] = pair;
    ASSERT_TRUE((std::is_same<typename std::decay<decltype(x)>::type, kstd::i32>::value));
    ASSERT_TRUE((std::is_same<typename std::decay<decltype(y)>::type, kstd::f32>::value));
    ASSERT_EQ(x, 1337);
    ASSERT_EQ(y, 3.141F);
}

TEST(kstd_Tuple, TestReferences) {
    //kstd::i32 a = 1337;
    //kstd::f32 b = 3.141F;
    //kstd::Pair<kstd::i32&, kstd::f32&> pair(a, b);
}

TEST(kstd_Tuple, TestPointers) {
    kstd::i32 a = 1337;
    kstd::f32 b = 3.141F;
    kstd::Pair<kstd::i32*, kstd::f32*> pair(&a, &b);
}