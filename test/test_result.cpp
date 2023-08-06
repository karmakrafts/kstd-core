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

TEST(kstd_Result, test_value) {
    using namespace std::string_literals;

    const auto value = "Hello World!"s;
    kstd::Result result {value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result);
    ASSERT_EQ(value, *result);
}

TEST(kstd_Result, test_value_assignment) {
    using namespace std::string_literals;

    const auto value = "Hello World!"s;
    kstd::Result result {value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result);
    ASSERT_EQ(value, *result);

    auto other_value = *result;
    result = {};
    ASSERT_TRUE(result.is_empty());
    ASSERT_EQ(result.get_or("Test"s), "Test"s);
    ASSERT_EQ(other_value, value);
}

TEST(kstd_Result, test_value_error_assignment) {
    using namespace std::string_literals;

    const auto value = "Hello World!"s;
    kstd::Result result {value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result);
    ASSERT_EQ(value, *result);

    result = kstd::Error("This is an error now!"s);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"s);
}

TEST(kstd_Result, test_map_value) {
    using namespace std::string_literals;
    using namespace std::string_literals;

    const auto value = "Hello World!"s;
    kstd::Result result {value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result);
    ASSERT_EQ(value, *result);

    auto mapped_result = result.map([](auto value) {
        return std::string(value) + " Appended!";
    });

    ASSERT_TRUE(mapped_result);
    ASSERT_EQ("Hello World! Appended!"s, *mapped_result);
}

TEST(kstd_Result, test_no_copy_value) {
    using namespace std::string_literals;

    const auto value = "Hello World!"s;
    kstd::Result result {std::make_unique<std::string>(value)};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::unique_ptr<std::string>>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::unique_ptr<std::string>&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::unique_ptr<std::string>&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::unique_ptr<std::string>*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::unique_ptr<std::string>*>);

    ASSERT_TRUE(result);
    ASSERT_EQ(value, **result);
}

TEST(kstd_Result, test_no_copy_value_assignment) {
    using namespace std::string_literals;

    const auto value = "Hello World!"s;
    kstd::Result result {std::make_unique<std::string>(value)};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::unique_ptr<std::string>>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::unique_ptr<std::string>&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::unique_ptr<std::string>&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::unique_ptr<std::string>*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::unique_ptr<std::string>*>);

    ASSERT_TRUE(result);
    ASSERT_EQ(value, **result);

    auto other_value = **result;
    result = {};
    ASSERT_TRUE(result.is_empty());
    ASSERT_EQ(other_value, value);
}

TEST(kstd_Result, test_reference) {
    using namespace std::string_literals;

    auto value = "Hello World!"s;
    kstd::Result<std::string&> result {value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result);
}

TEST(kstd_Result, test_reference_assignment) {
    using namespace std::string_literals;

    auto value = "Hello World!"s;
    kstd::Result<std::string&> result {value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result);

    auto other_value = *result;
    result = {};
    ASSERT_TRUE(result.is_empty());
    auto value2 = "Test"s;
    ASSERT_EQ(result.get_or(value2), value2);
    ASSERT_EQ(other_value, value);
}

TEST(kstd_Result, test_reference_error_assignment) {
    using namespace std::string_literals;

    auto value = "Hello World!"s;
    kstd::Result<std::string&> result {value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result);

    result = kstd::Error("This is an error now!"s);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"s);
}

TEST(kstd_Result, test_map_reference) {
    using namespace std::string_literals;
    using namespace std::string_literals;

    auto value = "Hello World!"s;
    kstd::Result<std::string&> result {value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result);

    auto mapped_result = result.map([](auto value) {
        return std::string(value) + " Appended!";
    });

    ASSERT_TRUE(mapped_result);
    ASSERT_EQ("Hello World! Appended!"s, *mapped_result);
}

TEST(kstd_Result, test_const_reference) {
    using namespace std::string_literals;

    auto value = "Hello World!"s;
    kstd::Result<const std::string&> result {value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, const std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result);
}

TEST(kstd_Result, test_const_reference_assignment) {
    using namespace std::string_literals;

    auto value = "Hello World!"s;
    kstd::Result<const std::string&> result {value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, const std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result);

    auto other_value = *result;
    result = {};
    ASSERT_TRUE(result.is_empty());
    auto value2 = "Test"s;
    ASSERT_EQ(result.get_or(value2), value2);
    ASSERT_EQ(other_value, value);
}

TEST(kstd_Result, test_const_reference_error_assignment) {
    using namespace std::string_literals;

    auto value = "Hello World!"s;
    kstd::Result<const std::string&> result {value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, const std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result);

    result = kstd::Error("This is an error now!"s);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"s);
}

TEST(kstd_Result, test_map_const_reference) {
    using namespace std::string_literals;
    using namespace std::string_literals;

    auto value = "Hello World!"s;
    kstd::Result<const std::string&> result {value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, const std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, *result);

    auto mapped_result = result.map([](auto value) {
        return std::string(value) + " Appended!";
    });

    ASSERT_TRUE(mapped_result);
    ASSERT_EQ("Hello World! Appended!"s, *mapped_result);
}

TEST(kstd_Result, test_pointer) {
    using namespace std::string_literals;

    auto value = "Hello World!"s;
    kstd::Result result {&value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string*&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, std::string* const&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, **result);
}

TEST(kstd_Result, test_pointer_assignment) {
    using namespace std::string_literals;

    auto value = "Hello World!"s;
    kstd::Result result {&value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string*&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, std::string* const&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, **result);

    auto* other_value = *result;
    result = {};
    ASSERT_TRUE(result.is_empty());
    auto value2 = "Testing!!!1!"s;
    ASSERT_EQ(result.get_or(&value2), &value2);
    ASSERT_EQ(*other_value, value);
}

TEST(kstd_Result, test_pointer_error_assignment) {
    using namespace std::string_literals;

    auto value = "Hello World!"s;
    kstd::Result result {&value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string*&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, std::string* const&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, **result);

    result = kstd::Error("This is an error now!"s);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"s);
}

TEST(kstd_Result, test_map_pointer) {
    using namespace std::string_literals;
    using namespace std::string_literals;

    auto value = "Hello World!"s;
    kstd::Result result {&value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string*&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, std::string* const&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, **result);

    auto mapped_result = result.map([](auto value) {
        return std::string(*value) + " Appended!";
    });

    ASSERT_TRUE(mapped_result);
    ASSERT_EQ("Hello World! Appended!"s, *mapped_result);
}

TEST(kstd_Result, test_const_pointer) {
    using namespace std::string_literals;

    const auto value = "Hello World!"s;
    kstd::Result result {&value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, const std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, const std::string*&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string* const&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, **result);
}

TEST(kstd_Result, test_const_pointer_assignment) {
    using namespace std::string_literals;

    const auto value = "Hello World!"s;
    kstd::Result result {&value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, const std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, const std::string*&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string* const&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, **result);

    const auto* other_value = *result;
    result = {};
    ASSERT_TRUE(result.is_empty());
    auto value2 = "Testing!!!1!"s;
    ASSERT_EQ(result.get_or(&value2), &value2);
    ASSERT_EQ(*other_value, value);
}

TEST(kstd_Result, test_const_pointer_error_assignment) {
    using namespace std::string_literals;

    const auto value = "Hello World!"s;
    kstd::Result result {&value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, const std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, const std::string*&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string* const&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, **result);

    result = kstd::Error("This is an error now!"s);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"s);
}

TEST(kstd_Result, test_map_const_pointer) {
    using namespace std::string_literals;
    using namespace std::string_literals;

    const auto value = "Hello World!"s;
    kstd::Result result {&value};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, const std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, const std::string*&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string* const&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(value, **result);
#

    auto mapped_result = result.map([](auto value) {
        return std::string(*value) + " Appended!";
    });

    ASSERT_TRUE(mapped_result);
    ASSERT_EQ("Hello World! Appended!"s, *mapped_result);
}

TEST(kstd_Result, test_void) {
    using namespace std::string_literals;

    kstd::Result<void> result {};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, void>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, void>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, void>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, void>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, void>);

    ASSERT_TRUE(result.is_ok());

    result = kstd::Error("This is an error now!"s);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"s);
}

TEST(kstd_Result, test_void_throw_if_error) {
    using namespace std::string_literals;

    kstd::Result<void> result {kstd::Error {"I am an error >:3"s}};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, void>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, void>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, void>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, void>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, void>);

    ASSERT_THROW(result.throw_if_error(), std::runtime_error);

    result = {};
    ASSERT_NO_THROW(result.throw_if_error());
}

TEST(kstd_Result, test_empty) {
    using namespace std::string_literals;

    kstd::Result<std::string> result {};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_empty());
}

TEST(kstd_Result, test_empty_assignment) {
    using namespace std::string_literals;

    kstd::Result<std::string> result {};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_empty());

    result = "This is a value now!"s;
    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(*result, "This is a value now!"s);
}

TEST(kstd_Result, test_empty_error_assignment) {
    using namespace std::string_literals;

    kstd::Result<std::string> result {};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_TRUE(result.is_empty());

    result = kstd::Error("This is an error now!"s);
    ASSERT_TRUE(result.is_error());
    ASSERT_EQ(result.get_error(), "This is an error now!"s);
}

TEST(kstd_Result, test_throw_if_error) {
    using namespace std::string_literals;

    kstd::Result<std::string> result {kstd::Error {"I am an error >:|"s}};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_THROW(result.throw_if_error(), std::runtime_error);

    result = "This is a value now!"s;
    ASSERT_NO_THROW(result.throw_if_error());
}

TEST(kstd_Result, test_get_or_throw) {
    using namespace std::string_literals;

    kstd::Result<std::string> result {kstd::Error {"I am an error!"s}};

    static_assert(std::is_same_v<typename decltype(result)::ValueType, std::string>);
    static_assert(std::is_same_v<typename decltype(result)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(result)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(result)::ConstPointer, const std::string*>);

    ASSERT_THROW(auto ___ = result.get_or_throw(), std::runtime_error);

    result = "This is a value now!"s;
    ASSERT_EQ(result.get_or_throw(), "This is a value now!"s);
}