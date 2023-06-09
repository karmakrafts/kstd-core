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
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32> val_box(value);
    ASSERT_EQ(*val_box, value);
}

TEST(kstd_Box, TestValueAssignment) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32> val_box;
    val_box = {value};
    ASSERT_EQ(*val_box, value);
}

TEST(kstd_Box, TestReference) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32&> ref_box(value);
    ASSERT_EQ(*ref_box, value);
}

TEST(kstd_Box, TestReferenceAssignment) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32&> ref_box;
    ref_box = {value};
    ASSERT_EQ(*ref_box, value);
}

TEST(kstd_Box, TestConstReference) {
    kstd::i32 value = 1337;
    kstd::Box<const kstd::i32&> ref_box(value);
    ASSERT_EQ(*ref_box, value);
}

TEST(kstd_Box, TestConstReferenceAssignment) {
    kstd::i32 value = 1337;
    kstd::Box<const kstd::i32&> ref_box;
    ref_box = {value};
    ASSERT_EQ(*ref_box, value);
}

TEST(kstd_Box, TestPointer) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32*> ptr_box(&value);
    ASSERT_EQ(**ptr_box, value);
}

TEST(kstd_Box, TestPointerAssignment) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32*> ptr_box;
    ptr_box = {&value};
    ASSERT_EQ(**ptr_box, value);
}

TEST(kstd_Box, TestConstPointer) {
    kstd::i32 value = 1337;
    kstd::Box<const kstd::i32*> ptr_box(&value);
    ASSERT_EQ(**ptr_box, value);
}

TEST(kstd_Box, TestConstPointerAssignment) {
    kstd::i32 value = 1337;
    kstd::Box<const kstd::i32*> ptr_box;
    ptr_box = {&value};
    ASSERT_EQ(**ptr_box, value);
}