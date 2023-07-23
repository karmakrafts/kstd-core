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

TEST(kstd_Option, TestEmpty) {
    kstd::Option<std::string_view> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, std::string_view>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_FALSE(opt);
}

TEST(kstd_Option, TestMapEmpty) {
    using namespace std::string_literals;

    kstd::Option<std::string_view> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, std::string_view>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_FALSE(opt);

    auto mapped_opt = opt.map([](auto value) {
        return std::string(value) + ", appended!";
    });

    ASSERT_FALSE(mapped_opt);
}

TEST(kstd_Option, TestValue) {
    using namespace std::string_view_literals;

    constexpr auto str = "Hello World!"sv;

    kstd::Option opt {str};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, std::string_view>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, str);
}

TEST(kstd_Option, TestValueAssignment) {
    using namespace std::string_view_literals;
    auto str = "Hello World!"sv;

    kstd::Option<std::string_view> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, std::string_view>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_FALSE(opt);

    opt = {str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, str);
}

TEST(kstd_Option, TestMapValue) {
    using namespace std::string_view_literals;
    using namespace std::string_literals;

    constexpr auto str = "Hello World!"sv;

    kstd::Option opt {str};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, std::string_view>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, str);

    auto mapped_opt = opt.map([](auto value) {
        return std::string(value) + " Appended!";
    });

    ASSERT_TRUE(mapped_opt);
    ASSERT_EQ(*mapped_opt, "Hello World! Appended!"s);
}

TEST(kstd_Option, TestPointer) {
    using namespace std::string_view_literals;
    auto str = "Hello World!"sv;

    kstd::Option opt {&str};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, std::string_view*&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, std::string_view* const&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(**opt, str);
}

TEST(kstd_Option, TestPointerAssignment) {
    using namespace std::string_view_literals;
    auto str = "Hello World!"sv;

    kstd::Option<std::string_view*> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, std::string_view*&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, std::string_view* const&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_FALSE(opt);

    opt = {&str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, &str);
}

TEST(kstd_Option, TestMapPointer) {
    using namespace std::string_view_literals;
    using namespace std::string_literals;

    auto str = "Hello World!"sv;

    kstd::Option opt {&str};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, std::string_view*&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, std::string_view* const&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(**opt, str);

    auto mapped_opt = opt.map([](auto value) {
        return std::string(*value) + " Appended!";
    });

    ASSERT_TRUE(mapped_opt);
    ASSERT_EQ(*mapped_opt, "Hello World! Appended!"s);
}

TEST(kstd_Option, TestConstPointer) {
    using namespace std::string_view_literals;
    const auto str = "Hello World!"sv;

    kstd::Option opt {&str};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, const std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, const std::string_view*&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, const std::string_view* const&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(**opt, str);
}

TEST(kstd_Option, TestConstPointerAssignment) {
    using namespace std::string_view_literals;
    const auto str = "Hello World!"sv;

    kstd::Option<const std::string_view*> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, const std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, const std::string_view*&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, const std::string_view* const&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_FALSE(opt);

    opt = {&str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, &str);
}

TEST(kstd_Option, TestMapConstPointer) {
    using namespace std::string_view_literals;
    using namespace std::string_literals;

    const auto str = "Hello World!"sv;

    kstd::Option opt {&str};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, const std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, const std::string_view*&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, const std::string_view* const&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(**opt, str);

    auto mapped_opt = opt.map([](auto value) {
        return std::string(*value) + " Appended!";
    });

    ASSERT_TRUE(mapped_opt);
    ASSERT_EQ(*mapped_opt, "Hello World! Appended!"s);
}

TEST(kstd_Option, TestReference) {
    using namespace std::string_view_literals;
    auto str = "Hello World!"sv;
    kstd::Option<std::string_view&> opt {str};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_TRUE(opt);
}

TEST(kstd_Option, TestReferenceAssignment) {
    using namespace std::string_view_literals;
    auto str = "Hello World!"sv;

    kstd::Option<std::string_view&> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_FALSE(opt);

    opt = {str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, str);
}

TEST(kstd_Option, TestMapReference) {
    using namespace std::string_view_literals;
    using namespace std::string_literals;

    auto str = "Hello World!"sv;
    kstd::Option<std::string_view&> opt {str};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_TRUE(opt);

    auto mapped_opt = opt.map([](auto value) {
        return std::string(value) + " Appended!";
    });

    ASSERT_TRUE(mapped_opt);
    ASSERT_EQ(*mapped_opt, "Hello World! Appended!"s);
}

TEST(kstd_Option, TestConstReference) {
    using namespace std::string_view_literals;
    const auto str = "Hello World!"sv;
    kstd::Option<const std::string_view&> opt {str};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, const std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_TRUE(opt);
}

TEST(kstd_Option, TestConstReferenceAssignment) {
    using namespace std::string_view_literals;
    const auto str = "Hello World!"sv;

    kstd::Option<const std::string_view&> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, const std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_FALSE(opt);

    opt = {str};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, str);
}

TEST(kstd_Option, TestMapConstReference) {
    using namespace std::string_view_literals;
    using namespace std::string_literals;

    const auto str = "Hello World!"sv;
    kstd::Option<const std::string_view&> opt {str};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::const_reference, const std::string_view&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, const std::string_view*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const std::string_view*>);

    ASSERT_TRUE(opt);

    auto mapped_opt = opt.map([](auto value) {
        return std::string(value) + " Appended!";
    });

    ASSERT_TRUE(mapped_opt);
    ASSERT_EQ(*mapped_opt, "Hello World! Appended!"s);
}

TEST(kstd_Option, TestNonZero) {
    using namespace std::string_view_literals;
    const auto str = "Hello World!"sv;

    kstd::Option opt {kstd::NonZero {&str}};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, kstd::NonZero<const std::string_view*>>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, kstd::NonZero<const std::string_view*>&>);
    static_assert(
            std::is_same_v<typename decltype(opt)::const_reference, const kstd::NonZero<const std::string_view*>&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, kstd::NonZero<const std::string_view*>*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const kstd::NonZero<const std::string_view*>*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, &str);
}

TEST(kstd_Option, TestNonZeroAssignment) {
    using namespace std::string_view_literals;
    const auto str = "Hello World!"sv;

    kstd::Option<kstd::NonZero<const std::string_view*>> opt {};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, kstd::NonZero<const std::string_view*>>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, kstd::NonZero<const std::string_view*>&>);
    static_assert(
            std::is_same_v<typename decltype(opt)::const_reference, const kstd::NonZero<const std::string_view*>&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, kstd::NonZero<const std::string_view*>*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const kstd::NonZero<const std::string_view*>*>);

    ASSERT_FALSE(opt);

    opt = {kstd::NonZero {&str}};
    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, &str);
}

TEST(kstd_Option, TestMapNonZero) {
    using namespace std::string_view_literals;
    using namespace std::string_literals;

    const auto str = "Hello World!"sv;

    kstd::Option opt {kstd::NonZero {&str}};

    static_assert(std::is_same_v<typename decltype(opt)::value_type, kstd::NonZero<const std::string_view*>>);
    static_assert(std::is_same_v<typename decltype(opt)::reference, kstd::NonZero<const std::string_view*>&>);
    static_assert(
            std::is_same_v<typename decltype(opt)::const_reference, const kstd::NonZero<const std::string_view*>&>);
    static_assert(std::is_same_v<typename decltype(opt)::pointer, kstd::NonZero<const std::string_view*>*>);
    static_assert(std::is_same_v<typename decltype(opt)::const_pointer, const kstd::NonZero<const std::string_view*>*>);

    ASSERT_TRUE(opt);
    ASSERT_EQ(*opt, &str);

    auto mapped_opt = opt.map([](auto value) {
        return std::string(*value) + " Appended!";
    });

    ASSERT_TRUE(mapped_opt);
    ASSERT_EQ(*mapped_opt, "Hello World! Appended!"s);
}