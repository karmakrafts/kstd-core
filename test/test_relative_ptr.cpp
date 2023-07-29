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
 * @since 02/05/2023
 */

#include <gtest/gtest.h>
#include <kstd/libc.hpp>
#include <kstd/relative_ptr.hpp>

template<typename T>
struct TestStruct final {
    kstd::RelativePtr<kstd::i32, T> foo;
    kstd::RelativePtr<kstd::f32, T> bar;
};

/*
 * Since a relative pointer imposes linear allocation
 * on the data that it points to, we need to create a
 * test harness which mimics the behaviour of a linear
 * allocator, as found in real-time graphics applications for example.
 */

TEST(kstd_RelativePtr, test_unsigned_offset) {
    constexpr auto size = sizeof(TestStruct<kstd::u32>) + sizeof(kstd::i32) + sizeof(kstd::f32);

    auto* memory = reinterpret_cast<kstd::u8*>(// NOLINT
            kstd::libc::malloc(size));         // NOLINT
    ASSERT_TRUE(memory != nullptr);
    kstd::libc::memset(memory, 0, size);

    // Initialize test values
    *reinterpret_cast<kstd::i32*>(memory + sizeof(TestStruct<kstd::u32>)) = 1337;                      // NOLINT
    *reinterpret_cast<kstd::f32*>(memory + sizeof(TestStruct<kstd::u32>) + sizeof(kstd::i32)) = 3.141F;// NOLINT

    // Initialize relative pointers
    new(memory) TestStruct<kstd::u32> {};
    auto* rel = reinterpret_cast<TestStruct<kstd::u32>*>(memory);                                       // NOLINT
    rel->foo = reinterpret_cast<kstd::i32*>(memory + sizeof(TestStruct<kstd::u32>));                    // NOLINT
    rel->bar = reinterpret_cast<kstd::f32*>(memory + sizeof(TestStruct<kstd::u32>) + sizeof(kstd::i32));// NOLINT

    // Validate data integrity
    ASSERT_EQ(*(rel->foo), 1337);
    ASSERT_EQ(*(rel->bar), 3.141F);

    kstd::libc::free(memory);// NOLINT
}

TEST(kstd_RelativePtr, test_signed_offset) {
    constexpr auto size = sizeof(TestStruct<kstd::i32>) + sizeof(kstd::i32) + sizeof(kstd::f32);

    auto* memory = reinterpret_cast<kstd::u8*>(// NOLINT
            kstd::libc::malloc(size));         // NOLINT
    ASSERT_TRUE(memory != nullptr);
    kstd::libc::memset(memory, 0, size);

    // Initialize test values
    *reinterpret_cast<kstd::i32*>(memory + sizeof(TestStruct<kstd::i32>)) = 1337;                      // NOLINT
    *reinterpret_cast<kstd::f32*>(memory + sizeof(TestStruct<kstd::i32>) + sizeof(kstd::i32)) = 3.141F;// NOLINT

    // Initialize relative pointers
    new(memory) TestStruct<kstd::i32> {};
    auto* rel = reinterpret_cast<TestStruct<kstd::i32>*>(memory);                                       // NOLINT
    rel->foo = reinterpret_cast<kstd::i32*>(memory + sizeof(TestStruct<kstd::i32>));                    // NOLINT
    rel->bar = reinterpret_cast<kstd::f32*>(memory + sizeof(TestStruct<kstd::i32>) + sizeof(kstd::i32));// NOLINT

    // Validate data integrity
    ASSERT_EQ(*(rel->foo), 1337);
    ASSERT_EQ(*(rel->bar), 3.141F);

    kstd::libc::free(memory);// NOLINT
}