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
 * @since 09/08/2023
 */

#include <gtest/gtest.h>
#include <kstd/libc.hpp>
#include <kstd/slice.hpp>

TEST(kstd_Slice, test_from_pointer) {
    constexpr auto size = sizeof(kstd::u32) << 2;
    auto* memory = reinterpret_cast<kstd::u32*>(kstd::libc::malloc(size));// NOLINT
    kstd::Slice slice {memory, size};

    slice[0] = 444;
    slice[1] = 1337;
    slice[2] = 69;
    slice[3] = 222;

    ASSERT_EQ(slice.get_size(), size);
    ASSERT_EQ(slice.get_count(), 4);
    ASSERT_EQ(slice[0], 444);
    ASSERT_EQ(slice[1], 1337);
    ASSERT_EQ(slice[2], 69);
    ASSERT_EQ(slice[3], 222);

    kstd::libc::free(memory);// NOLINT
}

TEST(kstd_Slice, test_from_const_pointer) {
    constexpr auto size = sizeof(kstd::u32) << 2;

    auto* memory = reinterpret_cast<kstd::u32*>(kstd::libc::malloc(size));// NOLINT
    memory[0] = 444;                                                      // NOLINT
    memory[1] = 1337;                                                     // NOLINT
    memory[2] = 69;                                                       // NOLINT
    memory[3] = 222;                                                      // NOLINT

    kstd::Slice slice {reinterpret_cast<const kstd::u32*>(memory), size};// NOLINT

    ASSERT_EQ(slice.get_size(), size);
    ASSERT_EQ(slice.get_count(), 4);
    ASSERT_EQ(slice[0], 444);
    ASSERT_EQ(slice[1], 1337);
    ASSERT_EQ(slice[2], 69);
    ASSERT_EQ(slice[3], 222);

    kstd::libc::free(memory);// NOLINT
}

TEST(kstd_Slice, test_from_range) {
    std::vector<kstd::u32> values {444, 1337, 69, 222};
    kstd::Slice slice {values.begin(), values.end()};

    ASSERT_EQ(slice.get_size(), values.size() * sizeof(kstd::u32));
    ASSERT_EQ(slice.get_count(), 4);
    ASSERT_EQ(slice[0], 444);
    ASSERT_EQ(slice[1], 1337);
    ASSERT_EQ(slice[2], 69);
    ASSERT_EQ(slice[3], 222);
}

TEST(kstd_Slice, test_from_const_range) {
    std::vector<kstd::u32> values {444, 1337, 69, 222};
    kstd::Slice slice {values.cbegin(), values.cend()};

    ASSERT_EQ(slice.get_size(), values.size() * sizeof(kstd::u32));
    ASSERT_EQ(slice.get_count(), 4);
    ASSERT_EQ(slice[0], 444);
    ASSERT_EQ(slice[1], 1337);
    ASSERT_EQ(slice[2], 69);
    ASSERT_EQ(slice[3], 222);
}