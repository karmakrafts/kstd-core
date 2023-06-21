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
 * @since 21/06/2023
 */

#include <gtest/gtest.h>
#include <kstd/const_string.hpp>
#include <kstd/types.hpp>

template<kstd::ConstString STR>
struct Foo final {
    static constexpr kstd::ConstString str = STR;
    kstd::usize value = STR.length;
};

template<kstd::ConstWString STR>
struct Bar final {
    static constexpr kstd::ConstWString str = STR;
    kstd::usize value = STR.length;
};

TEST(kstd_ConstString, TestCompare) {
    constexpr auto valueA = Foo<"HELLO WORLD!">();
    constexpr auto valueB = Foo<"HELLO WORLD!">();

    ASSERT_TRUE(valueA.str == valueB.str);
    ASSERT_FALSE(valueA.str != valueB.str);
}

TEST(kstd_ConstWString, TestCompare) {
    constexpr auto valueA = Bar<L"HELLO WORLD!">();
    constexpr auto valueB = Bar<L"HELLO WORLD!">();

    ASSERT_TRUE(valueA.str == valueB.str);
    ASSERT_FALSE(valueA.str != valueB.str);
}