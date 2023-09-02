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
 * @since 01/09/2023
 */

#include <gtest/gtest.h>
#include <kstd/static_vector.hpp>

using namespace kstd;

TEST(kstd_StaticVector, test_push_back) {
    StaticVector<u32, 10> values {};
    ASSERT_EQ(values.get_capacity(), 10);
    ASSERT_EQ(values.get_size(), 0);

    for(usize index = 0; index < 10; ++index) {
        values.push_back(11 * (index + 1));
    }

    ASSERT_EQ(values.get_size(), 10);

    for(usize index = 0; index < 10; ++index) {
        ASSERT_EQ(values[index], 11 * (index + 1));
    }
}

TEST(kstd_StaticVector, test_emplace_back) {
    StaticVector<u32, 10> values {};
    ASSERT_EQ(values.get_capacity(), 10);
    ASSERT_EQ(values.get_size(), 0);

    for(usize index = 0; index < 10; ++index) {
        values.emplace_back(11 * (index + 1));
    }

    ASSERT_EQ(values.get_size(), 10);

    for(usize index = 0; index < 10; ++index) {
        ASSERT_EQ(values[index], 11 * (index + 1));
    }
}

TEST(kstd_StaticVector, test_pop_back) {
    StaticVector<u32, 10> values {11U, 22U, 33U, 44U, 55U};
    ASSERT_EQ(values.get_capacity(), 10);
    ASSERT_EQ(values.get_size(), 5);

    for(usize index = 0; index < 5; ++index) {
        ASSERT_EQ(values.pop_back(), 11 * ((4 - index) + 1));
    }
}

TEST(kstd_StaticVector, test_push_front) {
    StaticVector<u32, 10> values {};
    ASSERT_EQ(values.get_capacity(), 10);
    ASSERT_EQ(values.get_size(), 0);

    for(usize index = 0; index < 10; ++index) {
        values.push_front(11 * (index + 1));
    }

    ASSERT_EQ(values.get_size(), 10);

    for(usize index = 0; index < 10; ++index) {
        ASSERT_EQ(values[9 - index], 11 * (index + 1));
    }
}

TEST(kstd_StaticVector, test_emplace_front) {
    StaticVector<u32, 10> values {};
    ASSERT_EQ(values.get_capacity(), 10);
    ASSERT_EQ(values.get_size(), 0);

    for(usize index = 0; index < 10; ++index) {
        values.emplace_front(11 * (index + 1));
    }

    ASSERT_EQ(values.get_size(), 10);

    for(usize index = 0; index < 10; ++index) {
        ASSERT_EQ(values[9 - index], 11 * (index + 1));
    }
}

TEST(kstd_StaticVector, test_pop_front) {
    StaticVector<u32, 10> values {11U, 22U, 33U, 44U, 55U};
    ASSERT_EQ(values.get_capacity(), 10);
    ASSERT_EQ(values.get_size(), 5);

    for(usize index = 0; index < 5; ++index) {
        ASSERT_EQ(values.pop_front(), 11 * (index + 1));
    }
}

TEST(kstd_StaticVector, test_insert) {
    StaticVector<u32, 10> values {11U, 22U, 44U, 55U};
    ASSERT_EQ(values.get_capacity(), 10);
    ASSERT_EQ(values.get_size(), 4);

    values.insert(2, 33U);
    ASSERT_EQ(values.get_size(), 5);

    for(usize index = 0; index < 5; ++index) {
        ASSERT_EQ(values.pop_front(), 11 * (index + 1));
    }
}

TEST(kstd_StaticVector, test_replace) {
    StaticVector<u32, 10> values {11U, 22U, 22U, 44U, 55U};
    ASSERT_EQ(values.get_capacity(), 10);
    ASSERT_EQ(values.get_size(), 5);

    ASSERT_EQ(values.replace(2, 33U), 22U);
    ASSERT_EQ(values[2], 33U);
}