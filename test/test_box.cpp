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
    kstd::Box<kstd::i32> val_box {value};

    static_assert(std::is_same_v<typename decltype(val_box)::value_type, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(val_box)::naked_value_type, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(val_box)::reference, kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(val_box)::const_reference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(val_box)::pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(val_box)::const_pointer, const kstd::i32*>);

    ASSERT_EQ(*val_box, value);
}

TEST(kstd_Box, TestValueAssignment) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32> val_box;

    static_assert(std::is_same_v<typename decltype(val_box)::value_type, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(val_box)::naked_value_type, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(val_box)::reference, kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(val_box)::const_reference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(val_box)::pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(val_box)::const_pointer, const kstd::i32*>);

    val_box = {value};
    ASSERT_EQ(*val_box, value);
}

TEST(kstd_Box, TestReference) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32&> ref_box {value};

    static_assert(std::is_same_v<typename decltype(ref_box)::value_type, kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::naked_value_type, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ref_box)::reference, kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::const_reference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ref_box)::const_pointer, const kstd::i32*>);

    ASSERT_EQ(*ref_box, value);
}

TEST(kstd_Box, TestReferenceAssignment) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32&> ref_box;

    static_assert(std::is_same_v<typename decltype(ref_box)::value_type, kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::naked_value_type, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ref_box)::reference, kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::const_reference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ref_box)::const_pointer, const kstd::i32*>);

    ref_box = {value};
    ASSERT_EQ(*ref_box, value);
}

TEST(kstd_Box, TestConstReference) {
    kstd::i32 value = 1337;
    kstd::Box<const kstd::i32&> ref_box {value};

    static_assert(std::is_same_v<typename decltype(ref_box)::value_type, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::naked_value_type, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ref_box)::reference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::const_reference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::pointer, const kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ref_box)::const_pointer, const kstd::i32*>);

    ASSERT_EQ(*ref_box, value);
}

TEST(kstd_Box, TestConstReferenceAssignment) {
    kstd::i32 value = 1337;
    kstd::Box<const kstd::i32&> ref_box;

    static_assert(std::is_same_v<typename decltype(ref_box)::value_type, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::naked_value_type, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ref_box)::reference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::const_reference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::pointer, const kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ref_box)::const_pointer, const kstd::i32*>);

    ref_box = {value};
    ASSERT_EQ(*ref_box, value);
}

TEST(kstd_Box, TestNullPointer) {
    kstd::Box<kstd::i32*> ptr_box {nullptr};

    static_assert(std::is_same_v<typename decltype(ptr_box)::value_type, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::naked_value_type, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::reference, kstd::i32*&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::const_reference, kstd::i32* const&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::const_pointer, const kstd::i32*>);

    ASSERT_EQ(*ptr_box, nullptr);
}

TEST(kstd_Box, TestNullPointerAssignment) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32*> ptr_box {&value};

    static_assert(std::is_same_v<typename decltype(ptr_box)::value_type, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::naked_value_type, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::reference, kstd::i32*&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::const_reference, kstd::i32* const&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::const_pointer, const kstd::i32*>);

    ptr_box = {nullptr};
    ASSERT_EQ(*ptr_box, nullptr);
}

TEST(kstd_Box, TestPointer) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32*> ptr_box {&value};

    static_assert(std::is_same_v<typename decltype(ptr_box)::value_type, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::naked_value_type, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::reference, kstd::i32*&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::const_reference, kstd::i32* const&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::const_pointer, const kstd::i32*>);

    ASSERT_EQ(**ptr_box, value);
}

TEST(kstd_Box, TestPointerAssignment) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32*> ptr_box;

    static_assert(std::is_same_v<typename decltype(ptr_box)::value_type, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::naked_value_type, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::reference, kstd::i32*&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::const_reference, kstd::i32* const&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::const_pointer, const kstd::i32*>);

    ptr_box = {&value};
    ASSERT_EQ(**ptr_box, value);
}

TEST(kstd_Box, TestConstPointer) {
    kstd::i32 value = 1337;
    kstd::Box<const kstd::i32*> ptr_box {&value};

    static_assert(std::is_same_v<typename decltype(ptr_box)::value_type, const kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::naked_value_type, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::reference, const kstd::i32*&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::const_reference, const kstd::i32* const&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::const_pointer, const kstd::i32*>);

    ASSERT_EQ(**ptr_box, value);
}

TEST(kstd_Box, TestConstPointerAssignment) {
    kstd::i32 value = 1337;
    kstd::Box<const kstd::i32*> ptr_box;

    static_assert(std::is_same_v<typename decltype(ptr_box)::value_type, const kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::naked_value_type, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::reference, const kstd::i32*&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::const_reference, const kstd::i32* const&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::const_pointer, const kstd::i32*>);

    ptr_box = {&value};
    ASSERT_EQ(**ptr_box, value);
}