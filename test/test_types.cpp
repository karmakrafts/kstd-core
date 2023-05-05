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
 * @since 26/04/2023
 */

#include <gtest/gtest.h>
#include <kstd/types.hpp>

TEST(kstd, TestSignedTypes) {
    ASSERT_TRUE(sizeof(kstd::i8) == 1);
    ASSERT_TRUE(sizeof(kstd::i16) == 2);
    ASSERT_TRUE(sizeof(kstd::i32) == 4);
    ASSERT_TRUE(sizeof(kstd::i64) == 8);
    ASSERT_TRUE(sizeof(kstd::isize) == sizeof(void*));
}

TEST(kstd, TestUnsignedTypes) {
    ASSERT_TRUE(sizeof(kstd::u8) == 1);
    ASSERT_TRUE(sizeof(kstd::u16) == 2);
    ASSERT_TRUE(sizeof(kstd::u32) == 4);
    ASSERT_TRUE(sizeof(kstd::u64) == 8);
    ASSERT_TRUE(sizeof(kstd::usize) == sizeof(void*));
}

TEST(kstd, TestFloatTypes) {
    ASSERT_TRUE(sizeof(kstd::f32) == 4);
    ASSERT_TRUE(sizeof(kstd::f64) == 8);
    constexpr auto f64ex_size = sizeof(kstd::f64ex);
    ASSERT_TRUE(f64ex_size >= 8 && f64ex_size <= 16);
}