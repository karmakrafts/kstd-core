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
 * @since 17/05/2023
 */

#include <gtest/gtest.h>
#include <kstd/string.hpp>

TEST(kstd_HeapString, TestAllocate) {
    kstd::HeapString string;
}

TEST(kstd_HeapString, TestConcat) {
    kstd::HeapString a;
    kstd::HeapString b;
}

TEST(kstd_SmallString, TestAllocate) {
    kstd::SmallString<64> string;
    ASSERT_EQ(string.get_size(), 0);
}

TEST(kstd_SmallString, TestConcat) {
    kstd::SmallString<64> a;
    kstd::SmallString<64> b;
    ASSERT_EQ(a.get_size(), 0);
    ASSERT_EQ(b.get_size(), 0);
}

TEST(kstd_String, TestAllocate) {
    kstd::String string;
}

TEST(kstd_String, TestConcat) {
    kstd::String a;
    kstd::String b;
}