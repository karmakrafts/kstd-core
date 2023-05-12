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
#include <kstd/large_string.hpp>
#include <kstd/string_slice.hpp>

using namespace kstd::string_literals;

TEST(kstd_Result, TestValue) {
    const auto value = "Hello World!"_str;
    auto result = kstd::make_ok(value);

    ASSERT_TRUE(result);
    ASSERT_EQ(value, result.borrow_value());

    auto other_value = *result;
    ASSERT_FALSE(result.is_empty());
    ASSERT_EQ(other_value, value);

    result = kstd::Error("This is an error now!"_str);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"_str);
}

TEST(kstd_Result, TestReference) {
    auto value = "Hello World!"_str;
    auto result = kstd::make_ok<kstd::StringSlice&>(value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, result.borrow_value());

    auto other_value = *result;
    ASSERT_FALSE(result.is_empty());
    ASSERT_EQ(other_value, value);

    result = kstd::Error("This is an error now!"_str);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"_str);
}

TEST(kstd_Result, TestPointer) {
    auto value = "Hello World!"_str;
    auto result = kstd::make_ok(&value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result.borrow_value());

    auto other_value = *result;
    ASSERT_FALSE(result.is_empty());
    ASSERT_EQ(*other_value, value);

    result = kstd::Error("This is an error now!"_str);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"_str);
}

TEST(kstd_Result, TestVoid) {
    auto result = kstd::Result<void>();
    ASSERT_TRUE(result.is_ok());

    result = kstd::Error("This is an error now!"_str);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"_str);
}

TEST(kstd_Result, TestEmpty) {
    auto result = kstd::Result<kstd::StringSlice>();
    ASSERT_TRUE(result.is_empty());

    result = kstd::Error("This is an error now!"_str);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"_str);
}