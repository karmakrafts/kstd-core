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
 * @since 21/05/2023
 */

#include <gtest/gtest.h>
#include <kstd/small_string.hpp>

TEST(kstd_SmallString, TestEmpty) {
    kstd::SmallString<16> str;
    static_assert(str.get_capacity() == 16);
    ASSERT_EQ(str.get_size(), 0);
    ASSERT_EQ(str, "");
}

TEST(kstd_SmallString, TestConcat) {
    kstd::SmallString<16> a("Hello ");
    ASSERT_EQ(a.get_size(), 6);
    ASSERT_EQ(a, "Hello ");

    kstd::SmallString<16> b("World!");
    ASSERT_EQ(b.get_size(), 6);
    ASSERT_EQ(b, "World!");

    auto x = a + b;
    static_assert(x.get_capacity() == 32);
    ASSERT_EQ(x.get_size(), 12);
    ASSERT_EQ(x, "Hello World!");
}