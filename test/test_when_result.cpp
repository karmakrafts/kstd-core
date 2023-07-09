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
 * @since 09/07/2023
 */

#include <gtest/gtest.h>
#include <kstd/when_result.hpp>

TEST(kstd_ResultWhenScope, TestSome) {
    using namespace std::string_view_literals;
    auto result = kstd::Result("Hello World!"sv);
    // clang-format off
    std::string_view value = kstd::when<std::string_view>(result)
        .some   ([](auto value) { return value; })
        .error  ([](auto)       { return "An error occurred"sv; })
        .none   ([]             { return "No value present"sv; });
    // clang-format on
    ASSERT_EQ(value, "Hello World!"sv);
}

TEST(kstd_ResultWhenScope, TestNone) {
    using namespace std::string_view_literals;
    auto result = kstd::Result<std::string_view>();
    // clang-format off
    std::string_view value = kstd::when<std::string_view>(result)
        .some   ([](auto value) { return value; })
        .error  ([](auto)       { return "An error occurred"sv; })
        .none   ([]             { return "No value present"sv; });
    // clang-format on
    ASSERT_EQ(value, "No value present"sv);
}

TEST(kstd_ResultWhenScope, TestEmpty) {
    using namespace std::string_view_literals;
    kstd::Result<std::string_view> result = kstd::Error("Ignored value"sv);
    // clang-format off
    std::string_view value = kstd::when<std::string_view>(result)
        .some   ([](auto value) { return value; })
        .error  ([](auto)       { return "An error occurred"sv; })
        .none   ([]             { return "No value present"sv; });
    // clang-format on
    ASSERT_EQ(value, "An error occurred"sv);
}

TEST(kstd_ResultWhenScope, TestVoid) {
    using namespace std::string_view_literals;
    auto result = kstd::Result<void>();
    // clang-format off
    std::string_view value = kstd::when<std::string_view>(result)
        .error  ([](auto)   { return "An error occurred"sv; })
        .none   ([]         { return "No value present"sv; });
    // clang-format on
    ASSERT_EQ(value, "No value present"sv);
}