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

using namespace kstd;

TEST(kstd_Slice, test_from_pointer) {
    constexpr auto size = sizeof(u32) << 2;
    auto* memory = reinterpret_cast<u32*>(libc::malloc(size));// NOLINT
    Slice slice {memory, size};

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

    libc::free(memory);// NOLINT
}

TEST(kstd_Slice, test_from_const_pointer) {
    constexpr auto size = sizeof(u32) << 2;

    auto* memory = reinterpret_cast<u32*>(libc::malloc(size));// NOLINT
    memory[0] = 444;                                          // NOLINT
    memory[1] = 1337;                                         // NOLINT
    memory[2] = 69;                                           // NOLINT
    memory[3] = 222;                                          // NOLINT

    Slice slice {reinterpret_cast<const u32*>(memory), size};// NOLINT

    ASSERT_EQ(slice.get_size(), size);
    ASSERT_EQ(slice.get_count(), 4);
    ASSERT_EQ(slice[0], 444);
    ASSERT_EQ(slice[1], 1337);
    ASSERT_EQ(slice[2], 69);
    ASSERT_EQ(slice[3], 222);

    libc::free(memory);// NOLINT
}

TEST(kstd_Slice, test_from_range) {
    std::vector<u32> values {444, 1337, 69, 222};
    Slice slice {values.begin(), values.end()};

    ASSERT_EQ(slice.get_size(), values.size() * sizeof(u32));
    ASSERT_EQ(slice.get_count(), 4);
    ASSERT_EQ(slice[0], 444);
    ASSERT_EQ(slice[1], 1337);
    ASSERT_EQ(slice[2], 69);
    ASSERT_EQ(slice[3], 222);
}

TEST(kstd_Slice, test_from_const_range) {
    std::vector<u32> values {444, 1337, 69, 222};
    Slice slice {values.cbegin(), values.cend()};

    ASSERT_EQ(slice.get_size(), values.size() * sizeof(u32));
    ASSERT_EQ(slice.get_count(), 4);
    ASSERT_EQ(slice[0], 444);
    ASSERT_EQ(slice[1], 1337);
    ASSERT_EQ(slice[2], 69);
    ASSERT_EQ(slice[3], 222);
}

TEST(kstd_Slice, test_from_pointer_range) {
    std::string value {"HELLO"};
    Slice slice {value.begin(), value.end()};

    ASSERT_EQ(slice.get_size(), value.size());
    ASSERT_EQ(slice.get_count(), 5);
    ASSERT_EQ(slice[0], 'H');
    ASSERT_EQ(slice[1], 'E');
    ASSERT_EQ(slice[2], 'L');
    ASSERT_EQ(slice[3], 'L');
    ASSERT_EQ(slice[4], 'O');
    ASSERT_EQ(slice[5], '\0');
}

TEST(kstd_Slice, test_from_const_pointer_range) {
    std::string value {"HELLO"};
    Slice slice {value.cbegin(), value.cend()};

    ASSERT_EQ(slice.get_size(), value.size());
    ASSERT_EQ(slice.get_count(), 5);
    ASSERT_EQ(slice[0], 'H');
    ASSERT_EQ(slice[1], 'E');
    ASSERT_EQ(slice[2], 'L');
    ASSERT_EQ(slice[3], 'L');
    ASSERT_EQ(slice[4], 'O');
    ASSERT_EQ(slice[5], '\0');
}

TEST(kstd_Slice, test_loop_value) {
    using namespace std::string_literals;
    std::vector values {"Hello"s, "World"s, ":3"s};
    Slice slice {values.cbegin(), values.cend()};
    usize index = 0;

    for(const auto& val : slice) {
        ASSERT_EQ(val, values[index++]);
    }
}

TEST(kstd_Slice, test_loop_pointer) {
    using namespace std::string_literals;
    std::vector values {"Hello"s, "World"s, ":3"s};
    std::vector<std::string*> addresses {};

    for(auto& val : values) {
        addresses.push_back(&val);
    }

    Slice slice {addresses.begin(), addresses.end()};
    usize index = 0;

    for(auto* val : slice) {
        ASSERT_EQ(*val, values[index++]);
    }
}
