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
    auto opt = kstd::Option<std::string_view>();
    ASSERT_FALSE(opt);
}

TEST(kstd_Option, TestValue) {
    using namespace std::string_view_literals;
    constexpr auto str = "Hello World!"sv;

    auto opt = kstd::Option(str);
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, str);
}

TEST(kstd_Option, TestValueAssignment) {
    using namespace std::string_view_literals;
    auto str = "Hello World!"sv;

    auto opt = kstd::Option<std::string_view>();
    ASSERT_FALSE(opt);

    opt = {str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, str);
}

TEST(kstd_Option, TestPointer) {
    using namespace std::string_view_literals;
    auto str = "Hello World!"sv;

    auto opt = kstd::Option(&str);
    ASSERT_TRUE(opt);
    ASSERT_EQ(**opt, str);
}

TEST(kstd_Option, TestPointerAssignment) {
    using namespace std::string_view_literals;
    auto str = "Hello World!"sv;

    auto opt = kstd::Option<std::string_view*>();
    ASSERT_FALSE(opt);

    opt = {&str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, &str);
}

TEST(kstd_Option, TestConstPointer) {
    using namespace std::string_view_literals;
    const auto str = "Hello World!"sv;

    auto opt = kstd::Option(&str);
    ASSERT_TRUE(opt);
    ASSERT_EQ(**opt, str);
}

TEST(kstd_Option, TestConstPointerAssignment) {
    using namespace std::string_view_literals;
    const auto str = "Hello World!"sv;

    auto opt = kstd::Option<const std::string_view*>();
    ASSERT_FALSE(opt);

    opt = {&str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, &str);
}

TEST(kstd_Option, TestReference) {
    using namespace std::string_view_literals;
    auto str = "Hello World!"sv;
    auto opt = kstd::Option<std::string_view&>(str);
    ASSERT_TRUE(opt);
}

TEST(kstd_Option, TestReferenceAssignment) {
    using namespace std::string_view_literals;
    auto str = "Hello World!"sv;

    auto opt = kstd::Option<std::string_view&>();
    ASSERT_FALSE(opt);

    opt = {str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, str);
}

TEST(kstd_Option, TestConstReference) {
    using namespace std::string_view_literals;
    const auto str = "Hello World!"sv;
    auto opt = kstd::Option<const std::string_view&>(str);
    ASSERT_TRUE(opt);
}

TEST(kstd_Option, TestConstReferenceAssignment) {
    using namespace std::string_view_literals;
    const auto str = "Hello World!"sv;

    auto opt = kstd::Option<const std::string_view&>();
    ASSERT_FALSE(opt);

    opt = {str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, str);
}

TEST(kstd_Option, TestNonZero) {
    using namespace std::string_view_literals;
    const auto str = "Hello World!"sv;

    auto opt = kstd::Option(kstd::NonZero(&str));
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, &str);
}

TEST(kstd_Option, TestNonZeroAssignment) {
    using namespace std::string_view_literals;
    const auto str = "Hello World!"sv;

    auto opt = kstd::Option<kstd::NonZero<const std::string_view*>>();
    ASSERT_FALSE(opt);

    opt = {kstd::NonZero(&str)};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, &str);
}