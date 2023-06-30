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
 * @since 06/05/2023
 */

#include <gtest/gtest.h>
#include <kstd/option.hpp>

TEST(kstd_Option, TestEmpty) {
    auto opt = kstd::make_empty<std::string_view>();
    ASSERT_FALSE(opt);
}

TEST(kstd_Option, TestValue) {
    using namespace std::string_view_literals;
    constexpr auto str = "Hello World!"sv;

    auto opt = kstd::make_value(str);
    ASSERT_TRUE(opt);

    ASSERT_EQ(opt.borrow(), str);
    opt.drop();

    ASSERT_FALSE(opt);
    opt = {str};

    ASSERT_TRUE(opt);
    ASSERT_EQ(opt.get(), str);
}

TEST(kstd_Option, TestPointer) {
    using namespace std::string_view_literals;
    constexpr auto str = "Hello World!"sv;

    auto opt = kstd::make_value(&str);
    ASSERT_TRUE(opt);

    ASSERT_EQ(*opt.borrow(), str);
    opt.drop();

    ASSERT_FALSE(opt);
    opt = {&str};

    ASSERT_TRUE(opt);
    ASSERT_EQ(opt.get(), &str);
}

TEST(kstd_Option, TestReference) {
    using namespace std::string_view_literals;
    constexpr auto str = "Hello World!"sv;

    auto opt = kstd::make_value<const std::string_view&>(str);
    ASSERT_TRUE(opt);

    ASSERT_EQ(opt.borrow(), str);
    opt.drop();

    ASSERT_FALSE(opt);
    opt = {str};

    ASSERT_TRUE(opt);
    ASSERT_EQ(opt.get(), str);
}

TEST(kstd_Option, TestNonZero) {
    using namespace std::string_view_literals;
    constexpr auto str = "Hello World!"sv;

    auto opt = kstd::make_value(kstd::make_non_zero(&str));
    ASSERT_TRUE(opt);
    ASSERT_EQ(opt.borrow(), &str);
}