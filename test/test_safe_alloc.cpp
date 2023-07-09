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
#include <kstd/safe_alloc.hpp>
#include <string>

TEST(kstd, TestTryConstruct) {
    constexpr auto value = "I'm a stack string! :3";
    auto result = kstd::try_construct<std::string>(value);
    ASSERT_TRUE(result);
    ASSERT_EQ(*result, value);
}

TEST(kstd, TestTryNew) {
    constexpr auto value = "I'm a heap string! :3";
    auto result = kstd::try_new<std::string>(value);
    ASSERT_TRUE(result);
    ASSERT_EQ(**result, value);
    delete *result;// Delete memory
}

TEST(kstd, TestTryMakeUnique) {
    constexpr auto value = "Hello World!";
    auto result = kstd::try_make_unique<std::string>(value);
    ASSERT_TRUE(result);
    ASSERT_EQ(**result, value);
}

TEST(kstd, TestTryMakeShared) {
    constexpr auto value = "Hello World!";
    auto result = kstd::try_make_shared<std::string>(value);
    ASSERT_TRUE(result);
    ASSERT_EQ(**result, value);
}