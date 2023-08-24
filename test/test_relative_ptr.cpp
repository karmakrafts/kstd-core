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

using namespace kstd;

template<typename T>
struct TestStruct final {
    RelativePtr<i32, T> foo;
    RelativePtr<f32, T> bar;
};

/*
 * Since a relative pointer imposes linear allocation
 * on the data that it points to, we need to create a
 * test harness which mimics the behaviour of a linear
 * allocator, as found in real-time graphics applications for example.
 */

TEST(kstd_RelativePtr, test_unsigned_offset) {
    constexpr auto size = sizeof(TestStruct<u32>) + sizeof(i32) + sizeof(f32);

    auto* memory = reinterpret_cast<u8*>(// NOLINT
            libc::malloc(size));         // NOLINT
    ASSERT_TRUE(memory != nullptr);
    libc::memset(memory, 0, size);

    // Initialize test values
    *reinterpret_cast<i32*>(memory + sizeof(TestStruct<u32>)) = 1337;                // NOLINT
    *reinterpret_cast<f32*>(memory + sizeof(TestStruct<u32>) + sizeof(i32)) = 3.141F;// NOLINT

    // Initialize relative pointers
    new(memory) TestStruct<u32> {};
    auto* rel = reinterpret_cast<TestStruct<u32>*>(memory);                           // NOLINT
    rel->foo = reinterpret_cast<i32*>(memory + sizeof(TestStruct<u32>));              // NOLINT
    rel->bar = reinterpret_cast<f32*>(memory + sizeof(TestStruct<u32>) + sizeof(i32));// NOLINT

    // Validate data integrity
    ASSERT_EQ(*(rel->foo), 1337);
    ASSERT_EQ(*(rel->bar), 3.141F);

    libc::free(memory);// NOLINT
}

TEST(kstd_RelativePtr, test_signed_offset) {
    constexpr auto size = sizeof(TestStruct<i32>) + sizeof(i32) + sizeof(f32);

    auto* memory = reinterpret_cast<u8*>(// NOLINT
            libc::malloc(size));         // NOLINT
    ASSERT_TRUE(memory != nullptr);
    libc::memset(memory, 0, size);

    // Initialize test values
    *reinterpret_cast<i32*>(memory + sizeof(TestStruct<i32>)) = 1337;                // NOLINT
    *reinterpret_cast<f32*>(memory + sizeof(TestStruct<i32>) + sizeof(i32)) = 3.141F;// NOLINT

    // Initialize relative pointers
    new(memory) TestStruct<i32> {};
    auto* rel = reinterpret_cast<TestStruct<i32>*>(memory);                           // NOLINT
    rel->foo = reinterpret_cast<i32*>(memory + sizeof(TestStruct<i32>));              // NOLINT
    rel->bar = reinterpret_cast<f32*>(memory + sizeof(TestStruct<i32>) + sizeof(i32));// NOLINT

    // Validate data integrity
    ASSERT_EQ(*(rel->foo), 1337);
    ASSERT_EQ(*(rel->bar), 3.141F);

    libc::free(memory);// NOLINT
}