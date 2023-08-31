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
 * @since 31/08/2023
 */

#include <gtest/gtest.h>
#include <kstd/trace.hpp>

auto bar(const std::string& value) noexcept -> void {
    KSTD_TRACE;
    using namespace std::string_literals;

    ASSERT_EQ(value, "Hello World! :3");

    const auto& stack_trace = kstd::get_stack_trace();
    ASSERT_EQ(stack_trace.size(), 2);
    ASSERT_EQ(stack_trace[0].get_function(), "foo"s);
    ASSERT_EQ(stack_trace[1].get_function(), "bar"s);
}

auto foo(const std::string& value) noexcept -> void {
    KSTD_TRACE;
    bar(value);
}

TEST(kstd, test_get_stack_trace) {
    foo("Hello World! :3");
}