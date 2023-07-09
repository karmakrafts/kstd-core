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
 * @since 26/04/2023
 */

#include <gtest/gtest.h>
#include <kstd/result.hpp>
#include <string_view>

TEST(kstd_Result, TestValue) {
    using namespace std::string_view_literals;

    const auto value = "Hello World!"sv;
    auto result = kstd::Result(value);

    ASSERT_TRUE(result);
    ASSERT_EQ(value, *result);
}

TEST(kstd_Result, TestValueAssignment) {
    using namespace std::string_view_literals;

    const auto value = "Hello World!"sv;
    auto result = kstd::Result(value);

    ASSERT_TRUE(result);
    ASSERT_EQ(value, *result);

    auto other_value = *result;
    result = {};
    ASSERT_TRUE(result.is_empty());
    ASSERT_EQ(result.get_or("Test"sv), "Test"sv);
    ASSERT_EQ(other_value, value);
}

TEST(kstd_Result, TestValueErrorAssignment) {
    using namespace std::string_view_literals;

    const auto value = "Hello World!"sv;
    auto result = kstd::Result(value);

    ASSERT_TRUE(result);
    ASSERT_EQ(value, *result);

    result = kstd::Error("This is an error now!"sv);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"sv);
}

TEST(kstd_Result, TestReference) {
    using namespace std::string_view_literals;

    auto value = "Hello World!"sv;
    auto result = kstd::Result<std::string_view&>(value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result);
}

TEST(kstd_Result, TestReferenceAssignment) {
    using namespace std::string_view_literals;

    auto value = "Hello World!"sv;
    auto result = kstd::Result<std::string_view&>(value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result);

    auto other_value = *result;
    result = {};
    ASSERT_TRUE(result.is_empty());
    auto value2 = "Test"sv;
    ASSERT_EQ(result.get_or(value2), value2);
    ASSERT_EQ(other_value, value);
}

TEST(kstd_Result, TestReferenceErrorAssignment) {
    using namespace std::string_view_literals;

    auto value = "Hello World!"sv;
    auto result = kstd::Result<std::string_view&>(value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result);

    result = kstd::Error("This is an error now!"sv);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"sv);
}

TEST(kstd_Result, TestConstReference) {
    using namespace std::string_view_literals;

    auto value = "Hello World!"sv;
    auto result = kstd::Result<const std::string_view&>(value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result);
}

TEST(kstd_Result, TestConstReferenceAssignment) {
    using namespace std::string_view_literals;

    auto value = "Hello World!"sv;
    auto result = kstd::Result<const std::string_view&>(value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result);

    auto other_value = *result;
    result = {};
    ASSERT_TRUE(result.is_empty());
    auto value2 = "Test"sv;
    ASSERT_EQ(result.get_or(value2), value2);
    ASSERT_EQ(other_value, value);
}

TEST(kstd_Result, TestConstReferenceErrorAssignment) {
    using namespace std::string_view_literals;

    auto value = "Hello World!"sv;
    auto result = kstd::Result<const std::string_view&>(value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result);

    result = kstd::Error("This is an error now!"sv);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"sv);
}

TEST(kstd_Result, TestPointer) {
    using namespace std::string_view_literals;

    auto value = "Hello World!"sv;
    auto result = kstd::Result(&value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, **result);
}

TEST(kstd_Result, TestPointerAssignment) {
    using namespace std::string_view_literals;

    auto value = "Hello World!"sv;
    auto result = kstd::Result(&value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, **result);

    auto* other_value = *result;
    result = {};
    ASSERT_TRUE(result.is_empty());
    auto value2 = "Testing!!!1!"sv;
    ASSERT_EQ(result.get_or(&value2), &value2);
    ASSERT_EQ(*other_value, value);
}

TEST(kstd_Result, TestPointerErrorAssignment) {
    using namespace std::string_view_literals;

    auto value = "Hello World!"sv;
    auto result = kstd::Result(&value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, **result);

    result = kstd::Error("This is an error now!"sv);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"sv);
}

TEST(kstd_Result, TestConstPointer) {
    using namespace std::string_view_literals;

    const auto value = "Hello World!"sv;
    auto result = kstd::Result(&value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, **result);
}

TEST(kstd_Result, TestConstPointerAssignment) {
    using namespace std::string_view_literals;

    const auto value = "Hello World!"sv;
    auto result = kstd::Result(&value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, **result);

    auto* other_value = *result;
    result = {};
    ASSERT_TRUE(result.is_empty());
    auto value2 = "Testing!!!1!"sv;
    ASSERT_EQ(result.get_or(&value2), &value2);
    ASSERT_EQ(*other_value, value);
}

TEST(kstd_Result, TestConstPointerErrorAssignment) {
    using namespace std::string_view_literals;

    const auto value = "Hello World!"sv;
    auto result = kstd::Result(&value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, **result);

    result = kstd::Error("This is an error now!"sv);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"sv);
}

TEST(kstd_Result, TestVoid) {
    using namespace std::string_view_literals;

    auto result = kstd::Result<void>();
    ASSERT_TRUE(result.is_ok());

    result = kstd::Error("This is an error now!"sv);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"sv);
}

TEST(kstd_Result, TestEmpty) {
    using namespace std::string_view_literals;

    auto result = kstd::Result<std::string_view>();
    ASSERT_TRUE(result.is_empty());
}

TEST(kstd_Result, TestEmptyAssignment) {
    using namespace std::string_view_literals;

    auto result = kstd::Result<std::string_view>();
    ASSERT_TRUE(result.is_empty());

    result = kstd::Result("This is a value now!"sv);
    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(*result, "This is a value now!"sv);
}

TEST(kstd_Result, TestEmptyErrorAssignment) {
    using namespace std::string_view_literals;

    auto result = kstd::Result<std::string_view>();
    ASSERT_TRUE(result.is_empty());

    result = kstd::Error("This is an error now!"sv);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"sv);
}