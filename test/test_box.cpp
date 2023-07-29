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
#include <memory>

TEST(kstd_Box, test_value) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32> val_box {value};

    static_assert(std::is_same_v<typename decltype(val_box)::ValueType, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(val_box)::NakedValueType, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(val_box)::Reference, kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(val_box)::ConstReference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(val_box)::Pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(val_box)::ConstPointer, const kstd::i32*>);

    ASSERT_EQ(*val_box, value);
}

TEST(kstd_Box, test_value_assignment) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32> val_box;

    static_assert(std::is_same_v<typename decltype(val_box)::ValueType, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(val_box)::NakedValueType, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(val_box)::Reference, kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(val_box)::ConstReference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(val_box)::Pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(val_box)::ConstPointer, const kstd::i32*>);

    val_box = {value};
    ASSERT_EQ(*val_box, value);
}

TEST(kstd_Box, test_no_copy_value) {
    kstd::i32 value = 1337;
    kstd::Box<std::unique_ptr<kstd::i32>> val_box {std::make_unique<kstd::i32>(value)};

    static_assert(std::is_same_v<typename decltype(val_box)::ValueType, std::unique_ptr<kstd::i32>>);
    static_assert(std::is_same_v<typename decltype(val_box)::NakedValueType, std::unique_ptr<kstd::i32>>);
    static_assert(std::is_same_v<typename decltype(val_box)::Reference, std::unique_ptr<kstd::i32>&>);
    static_assert(std::is_same_v<typename decltype(val_box)::ConstReference, const std::unique_ptr<kstd::i32>&>);
    static_assert(std::is_same_v<typename decltype(val_box)::Pointer, std::unique_ptr<kstd::i32>*>);
    static_assert(std::is_same_v<typename decltype(val_box)::ConstPointer, const std::unique_ptr<kstd::i32>*>);

    ASSERT_EQ(**val_box, value);

    auto ptr = std::move(*val_box);
    ASSERT_EQ(*ptr, value);
}

TEST(kstd_Box, test_no_copy_value_assignment) {
    kstd::i32 value = 1337;
    kstd::Box<std::unique_ptr<kstd::i32>> val_box;

    static_assert(std::is_same_v<typename decltype(val_box)::ValueType, std::unique_ptr<kstd::i32>>);
    static_assert(std::is_same_v<typename decltype(val_box)::NakedValueType, std::unique_ptr<kstd::i32>>);
    static_assert(std::is_same_v<typename decltype(val_box)::Reference, std::unique_ptr<kstd::i32>&>);
    static_assert(std::is_same_v<typename decltype(val_box)::ConstReference, const std::unique_ptr<kstd::i32>&>);
    static_assert(std::is_same_v<typename decltype(val_box)::Pointer, std::unique_ptr<kstd::i32>*>);
    static_assert(std::is_same_v<typename decltype(val_box)::ConstPointer, const std::unique_ptr<kstd::i32>*>);

    val_box = {std::make_unique<kstd::i32>(value)};
    ASSERT_EQ(**val_box, value);

    auto ptr = std::move(*val_box);
    ASSERT_EQ(*ptr, value);
}

TEST(kstd_Box, test_reference) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32&> ref_box {value};

    static_assert(std::is_same_v<typename decltype(ref_box)::ValueType, kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::NakedValueType, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ref_box)::Reference, kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::ConstReference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::Pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ref_box)::ConstPointer, const kstd::i32*>);

    ASSERT_EQ(*ref_box, value);
}

TEST(kstd_Box, test_reference_assignment) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32&> ref_box;

    static_assert(std::is_same_v<typename decltype(ref_box)::ValueType, kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::NakedValueType, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ref_box)::Reference, kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::ConstReference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::Pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ref_box)::ConstPointer, const kstd::i32*>);

    ref_box = {value};
    ASSERT_EQ(*ref_box, value);
}

TEST(kstd_Box, test_const_reference) {
    kstd::i32 value = 1337;
    kstd::Box<const kstd::i32&> ref_box {value};

    static_assert(std::is_same_v<typename decltype(ref_box)::ValueType, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::NakedValueType, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ref_box)::Reference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::ConstReference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::Pointer, const kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ref_box)::ConstPointer, const kstd::i32*>);

    ASSERT_EQ(*ref_box, value);
}

TEST(kstd_Box, test_const_reference_assignment) {
    kstd::i32 value = 1337;
    kstd::Box<const kstd::i32&> ref_box;

    static_assert(std::is_same_v<typename decltype(ref_box)::ValueType, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::NakedValueType, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ref_box)::Reference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::ConstReference, const kstd::i32&>);
    static_assert(std::is_same_v<typename decltype(ref_box)::Pointer, const kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ref_box)::ConstPointer, const kstd::i32*>);

    ref_box = {value};
    ASSERT_EQ(*ref_box, value);
}

TEST(kstd_Box, test_null_pointer) {
    kstd::Box<kstd::i32*> ptr_box {nullptr};

    static_assert(std::is_same_v<typename decltype(ptr_box)::ValueType, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::NakedValueType, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::Reference, kstd::i32*&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::ConstReference, kstd::i32* const&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::Pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::ConstPointer, const kstd::i32*>);

    ASSERT_EQ(*ptr_box, nullptr);
}

TEST(kstd_Box, test_null_pointer_assignment) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32*> ptr_box {&value};

    static_assert(std::is_same_v<typename decltype(ptr_box)::ValueType, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::NakedValueType, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::Reference, kstd::i32*&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::ConstReference, kstd::i32* const&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::Pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::ConstPointer, const kstd::i32*>);

    ptr_box = {nullptr};
    ASSERT_EQ(*ptr_box, nullptr);
}

TEST(kstd_Box, test_pointer) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32*> ptr_box {&value};

    static_assert(std::is_same_v<typename decltype(ptr_box)::ValueType, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::NakedValueType, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::Reference, kstd::i32*&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::ConstReference, kstd::i32* const&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::Pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::ConstPointer, const kstd::i32*>);

    ASSERT_EQ(**ptr_box, value);
}

TEST(kstd_Box, test_pointer_assignment) {
    kstd::i32 value = 1337;
    kstd::Box<kstd::i32*> ptr_box;

    static_assert(std::is_same_v<typename decltype(ptr_box)::ValueType, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::NakedValueType, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::Reference, kstd::i32*&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::ConstReference, kstd::i32* const&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::Pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::ConstPointer, const kstd::i32*>);

    ptr_box = {&value};
    ASSERT_EQ(**ptr_box, value);
}

TEST(kstd_Box, test_const_pointer) {
    kstd::i32 value = 1337;
    kstd::Box<const kstd::i32*> ptr_box {&value};

    static_assert(std::is_same_v<typename decltype(ptr_box)::ValueType, const kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::NakedValueType, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::Reference, const kstd::i32*&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::ConstReference, const kstd::i32* const&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::Pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::ConstPointer, const kstd::i32*>);

    ASSERT_EQ(**ptr_box, value);
}

TEST(kstd_Box, test_const_pointer_assignment) {
    kstd::i32 value = 1337;
    kstd::Box<const kstd::i32*> ptr_box;

    static_assert(std::is_same_v<typename decltype(ptr_box)::ValueType, const kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::NakedValueType, kstd::i32>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::Reference, const kstd::i32*&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::ConstReference, const kstd::i32* const&>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::Pointer, kstd::i32*>);
    static_assert(std::is_same_v<typename decltype(ptr_box)::ConstPointer, const kstd::i32*>);

    ptr_box = {&value};
    ASSERT_EQ(**ptr_box, value);
}