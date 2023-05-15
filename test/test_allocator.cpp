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
 * @since 13/05/2023
 */

#include <gtest/gtest.h>
#include <kstd/allocator.hpp>
#include <kstd/string_slice.hpp>

TEST(kstd_DefaultAllocator, TestAllocateDeallocate) {
    kstd::Allocator<kstd::f32> allocator;
    auto* ptr = allocator.allocate(4);

    ASSERT_TRUE(ptr != nullptr);

    *ptr = 1;
    *(ptr + 1) = 2;
    *(ptr + 2) = 3;
    *(ptr + 3) = 4;

    ASSERT_EQ(*ptr, 1);
    ASSERT_EQ(*(ptr + 1), 2);
    ASSERT_EQ(*(ptr + 2), 3);
    ASSERT_EQ(*(ptr + 3), 4);

    allocator.deallocate(ptr, 4);
}

TEST(kstd_DefaultAllocator, TestConstructDestroy) {
    kstd::Allocator<kstd::StringSlice> allocator;
    auto* ptr = allocator.construct("HELLO WORLD!");

    ASSERT_TRUE(ptr != nullptr);

    allocator.destroy(ptr);
}