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
#include <string>
#include <string_view>

TEST(kstd_Option, test_empty) {
    kstd::Option<std::string> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, std::string>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_FALSE(opt);
}

TEST(kstd_Option, test_map_empty) {
    using namespace std::string_literals;

    kstd::Option<std::string> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, std::string>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_FALSE(opt);

    auto mapped_opt = opt.map([](auto value) {
        return std::string(value) + ", appended!";
    });

    ASSERT_FALSE(mapped_opt);
}

TEST(kstd_Option, test_value) {
    using namespace std::string_literals;

    auto str = "Hello World!"s;

    kstd::Option opt {str};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, std::string>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, str);
}

TEST(kstd_Option, test_value_assignment) {
    using namespace std::string_literals;
    auto str = "Hello World!"s;

    kstd::Option<std::string> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, std::string>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_FALSE(opt);

    opt = {str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, str);
}

TEST(kstd_Option, test_map_value) {
    using namespace std::string_literals;
    using namespace std::string_literals;

    auto str = "Hello World!"s;

    kstd::Option opt {str};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, std::string>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, str);

    auto mapped_opt = opt.map([](auto value) {
        return std::string(value) + " Appended!";
    });

    ASSERT_TRUE(mapped_opt);
    ASSERT_EQ(*mapped_opt, "Hello World! Appended!"s);
}

TEST(kstd_Option, test_no_copy_value) {
    using namespace std::string_literals;

    auto str = "Hello World!"s;

    kstd::Option opt {std::make_unique<std::string>(str)};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, std::unique_ptr<std::string>>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, std::unique_ptr<std::string>&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::unique_ptr<std::string>&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::unique_ptr<std::string>*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::unique_ptr<std::string>*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(**opt, str);
}

TEST(kstd_Option, test_no_copy_value_assignment) {
    using namespace std::string_literals;
    auto str = "Hello World!"s;

    kstd::Option<std::unique_ptr<std::string>> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, std::unique_ptr<std::string>>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, std::unique_ptr<std::string>&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::unique_ptr<std::string>&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::unique_ptr<std::string>*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::unique_ptr<std::string>*>);

    ASSERT_FALSE(opt);

    opt = {std::make_unique<std::string>(str)};
    ASSERT_TRUE(opt);
    ASSERT_EQ(**opt, str);
}

TEST(kstd_Option, test_pointer) {
    using namespace std::string_literals;
    auto str = "Hello World!"s;

    kstd::Option opt {&str};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, std::string*&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, std::string* const&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(**opt, str);
}

TEST(kstd_Option, test_pointer_assignment) {
    using namespace std::string_literals;
    auto str = "Hello World!"s;

    kstd::Option<std::string*> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, std::string*&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, std::string* const&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_FALSE(opt);

    opt = {&str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, &str);
}

TEST(kstd_Option, test_map_pointer) {
    using namespace std::string_literals;
    using namespace std::string_literals;

    auto str = "Hello World!"s;

    kstd::Option opt {&str};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, std::string*&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, std::string* const&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(**opt, str);

    auto mapped_opt = opt.map([](auto value) {
        return std::string(*value) + " Appended!";
    });

    ASSERT_TRUE(mapped_opt);
    ASSERT_EQ(*mapped_opt, "Hello World! Appended!"s);
}

TEST(kstd_Option, test_const_pointer) {
    using namespace std::string_literals;
    const auto str = "Hello World!"s;

    kstd::Option opt {&str};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, const std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, const std::string*&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::string* const&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(**opt, str);
}

TEST(kstd_Option, test_const_pointer_assignment) {
    using namespace std::string_literals;
    const auto str = "Hello World!"s;

    kstd::Option<const std::string*> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, const std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, const std::string*&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::string* const&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_FALSE(opt);

    opt = {&str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, &str);
}

TEST(kstd_Option, test_map_const_pointer) {
    using namespace std::string_literals;
    using namespace std::string_literals;

    const auto str = "Hello World!"s;

    kstd::Option opt {&str};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, const std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, const std::string*&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::string* const&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(**opt, str);

    auto mapped_opt = opt.map([](auto value) {
        return std::string(*value) + " Appended!";
    });

    ASSERT_TRUE(mapped_opt);
    ASSERT_EQ(*mapped_opt, "Hello World! Appended!"s);
}

TEST(kstd_Option, test_reference) {
    using namespace std::string_literals;
    auto str = "Hello World!"s;
    kstd::Option<std::string&> opt {str};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_TRUE(opt);
}

TEST(kstd_Option, test_reference_assignment) {
    using namespace std::string_literals;
    auto str = "Hello World!"s;

    kstd::Option<std::string&> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_FALSE(opt);

    opt = {str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, str);
}

TEST(kstd_Option, test_map_reference) {
    using namespace std::string_literals;
    using namespace std::string_literals;

    auto str = "Hello World!"s;
    kstd::Option<std::string&> opt {str};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_TRUE(opt);

    auto mapped_opt = opt.map([](auto value) {
        return std::string(value) + " Appended!";
    });

    ASSERT_TRUE(mapped_opt);
    ASSERT_EQ(*mapped_opt, "Hello World! Appended!"s);
}

TEST(kstd_Option, test_const_reference) {
    using namespace std::string_literals;
    const auto str = "Hello World!"s;
    kstd::Option<const std::string&> opt {str};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, const std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_TRUE(opt);
}

TEST(kstd_Option, test_const_reference_assignment) {
    using namespace std::string_literals;
    const auto str = "Hello World!"s;

    kstd::Option<const std::string&> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, const std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_FALSE(opt);

    opt = {str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, str);
}

TEST(kstd_Option, test_map_const_reference) {
    using namespace std::string_literals;
    using namespace std::string_literals;

    const auto str = "Hello World!"s;
    kstd::Option<const std::string&> opt {str};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const std::string&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, const std::string*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const std::string*>);

    ASSERT_TRUE(opt);

    auto mapped_opt = opt.map([](auto value) {
        return std::string(value) + " Appended!";
    });

    ASSERT_TRUE(mapped_opt);
    ASSERT_EQ(*mapped_opt, "Hello World! Appended!"s);
}

TEST(kstd_Option, test_non_zero) {
    using namespace std::string_literals;
    const auto str = "Hello World!"s;

    kstd::Option opt {kstd::NonZero {&str}};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, kstd::NonZero<const std::string*>>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, kstd::NonZero<const std::string*>&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const kstd::NonZero<const std::string*>&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, kstd::NonZero<const std::string*>*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const kstd::NonZero<const std::string*>*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, &str);
}

TEST(kstd_Option, test_non_zero_assignment) {
    using namespace std::string_literals;
    const auto str = "Hello World!"s;

    kstd::Option<kstd::NonZero<const std::string*>> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, kstd::NonZero<const std::string*>>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, kstd::NonZero<const std::string*>&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const kstd::NonZero<const std::string*>&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, kstd::NonZero<const std::string*>*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const kstd::NonZero<const std::string*>*>);

    ASSERT_FALSE(opt);

    opt = {kstd::NonZero {&str}};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, &str);
}

TEST(kstd_Option, test_map_non_zero) {
    using namespace std::string_literals;
    using namespace std::string_literals;

    const auto str = "Hello World!"s;

    kstd::Option opt {kstd::NonZero {&str}};

    static_assert(std::is_same_v<typename decltype(opt)::ValueType, kstd::NonZero<const std::string*>>);
    static_assert(std::is_same_v<typename decltype(opt)::Reference, kstd::NonZero<const std::string*>&>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstReference, const kstd::NonZero<const std::string*>&>);
    static_assert(std::is_same_v<typename decltype(opt)::Pointer, kstd::NonZero<const std::string*>*>);
    static_assert(std::is_same_v<typename decltype(opt)::ConstPointer, const kstd::NonZero<const std::string*>*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, &str);

    auto mapped_opt = opt.map([](auto value) {
        return std::string(*value) + " Appended!";
    });

    ASSERT_TRUE(mapped_opt);
    ASSERT_EQ(*mapped_opt, "Hello World! Appended!"s);
}