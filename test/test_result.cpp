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

using namespace kstd;

TEST(kstd_Result, TestValue) {
    using namespace std::string_view_literals;

    std::string value("Hello World!");
    Result<std::string> result(value);

    ASSERT_TRUE(result);
    ASSERT_EQ(value, result.borrow_value());

    auto other_value = *result;
    ASSERT_TRUE(result.is_empty());
    ASSERT_EQ(other_value, value);

    result = Error("This is an error now!"sv);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"sv);
}

TEST(kstd_Result, TestReference) {
    using namespace std::string_view_literals;

    std::string value("Hello World!");
    Result<std::string &> result(value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, result.borrow_value());

    auto other_value = *result;
    ASSERT_TRUE(result.is_empty());
    ASSERT_EQ(other_value, value);

    result = Error("This is an error now!"sv);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"sv);
}

TEST(kstd_Result, TestPointer) {
    using namespace std::string_view_literals;

    std::string value("Hello World!");
    Result<std::string *> result(&value);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result.borrow_value());

    auto other_value = *result;
    ASSERT_TRUE(result.is_empty());
    ASSERT_EQ(*other_value, value);

    result = Error("This is an error now!"sv);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"sv);
}

TEST(kstd_Result, TestVoid) {
    using namespace std::string_view_literals;

    auto result = Result<void>();
    ASSERT_TRUE(result.is_ok());

    result = Error("This is an error now!"sv);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"sv);
}

TEST(kstd_Result, TestEmpty) {
    using namespace std::string_view_literals;

    auto result = Result<std::string>();
    ASSERT_TRUE(result.is_empty());

    result = Error("This is an error now!"sv);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"sv);
}