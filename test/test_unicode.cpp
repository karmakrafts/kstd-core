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
 * @since 03/08/2023
 */

#include <gtest/gtest.h>
#include <kstd/unicode.hpp>
#include <string>

using namespace kstd;

TEST(kstd_unicode, test_convert_wcs_to_mbs) {
    const auto* value = LR"(Hello World! 🦊)";

    usize count = 0;
    unicode::convert_buffer<char>(value, nullptr, &count);
    std::string result(count, ' ');
    unicode::convert_buffer(value, result.data(), nullptr);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result, R"(Hello World! 🦊)");
}

TEST(kstd_unicode, test_convert_mbs_to_wcs) {
    const auto* value = R"(Hello World! 🦊)";

    usize count = 0;
    unicode::convert_buffer<wchar_t>(value, nullptr, &count);
    std::wstring result(count, ' ');
    unicode::convert_buffer(value, result.data(), nullptr);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result, LR"(Hello World! 🦊)");
}

TEST(kstd_unicode, test_convert_wstring_to_string) {
    const std::wstring value = LR"(Hello World! 🦊)";
    const auto result = unicode::convert<char>(value);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result, R"(Hello World! 🦊)");
}

TEST(kstd_unicode, test_convert_string_to_wstring) {
    const std::string value = R"(Hello World! 🦊)";
    const auto result = unicode::convert<wchar_t>(value);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result, LR"(Hello World! 🦊)");
}