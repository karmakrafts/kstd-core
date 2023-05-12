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
 * @since 10/05/2023
 */

#include <gtest/gtest.h>
#include <kstd/box.hpp>

TEST(kstd_Box, TestValue) {
    kstd::i32 x = 1337;
    kstd::Box<kstd::i32> val_box(x);
    ASSERT_EQ(*val_box, x);
}

TEST(kstd_Box, TestReference) {
    kstd::i32 x = 1337;
    kstd::Box<kstd::i32&> ref_box(x);
    ASSERT_EQ(*ref_box, x);
}

TEST(kstd_Box, TestPointer) {
    kstd::i32 x = 1337;
    kstd::Box<kstd::i32*> ptr_box(&x);
    ASSERT_EQ(**ptr_box, x);
}