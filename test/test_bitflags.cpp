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
 * @since 05/05/2023
 */

#include <gtest/gtest.h>
#include <kstd/bitflags.hpp>
#include <kstd/types.hpp>

KSTD_BITFLAGS(kstd::u8, SomeFlags, FOO = 0b0000'0001, BAR = 0b0000'0010, BAZ = 0b0000'0100)

TEST(kstd, test_bit_flags) {
    SomeFlags flags = SomeFlags::NONE;
    flags |= SomeFlags::FOO;
    ASSERT_EQ(flags, SomeFlags::FOO);
    flags |= SomeFlags::BAZ;
    flags &= ~SomeFlags::FOO;
    ASSERT_EQ(flags, SomeFlags::BAZ);
}