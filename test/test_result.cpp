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
#include <string>

using namespace kstd::string_literals;

TEST(kstd_Result, TestValue) {
    std::string value("Hello World!");
    kstd::Result<std::string> result(value);

    ASSERT_TRUE(result);
    ASSERT_EQ(value, result.borrow_value());

    auto other_value = *result;
    ASSERT_TRUE(result.is_empty());
    ASSERT_EQ(other_value, value);

    result = kstd::error<std::string>("This is an error now!"_str);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"_str);
}

TEST(kstd_Result, TestReference) {
    std::string value("Hello World!");
    kstd::Result<std::string&> result(value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, result.borrow_value());

    auto other_value = *result;
    ASSERT_TRUE(result.is_empty());
    ASSERT_EQ(other_value, value);

    result = kstd::error<std::string&>("This is an error now!"_str);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"_str);
}

TEST(kstd_Result, TestPointer) {
    std::string value("Hello World!");
    kstd::Result<std::string*> result(&value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result.borrow_value());

    auto other_value = *result;
    ASSERT_TRUE(result.is_empty());
    ASSERT_EQ(*other_value, value);

    result = kstd::error<std::string*>("This is an error now!"_str);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"_str);
}

TEST(kstd_Result, TestVoid) {
    auto result = kstd::Result<void>();
    ASSERT_TRUE(result.is_ok());

    result = kstd::error<void>("This is an error now!"_str);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"_str);
}

TEST(kstd_Result, TestEmpty) {
    auto result = kstd::Result<std::string>();
    ASSERT_TRUE(result.is_empty());

    result = kstd::error<std::string>("This is an error now!"_str);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"_str);
}