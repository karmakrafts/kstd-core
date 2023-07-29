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
 * @since 29/07/2023
 */

#include <gtest/gtest.h>
#include <kstd/types.hpp>
#include <kstd/utils.hpp>

TEST(kstd_utils, test_transmute) {
    kstd::u32 value = 0x0000'0000U;
    auto& f_value = kstd::utils::transmute<kstd::f32>(value);
    ASSERT_EQ(f_value, 0.0F);
}

TEST(kstd_utils, test_transmute_const) {
    const kstd::u32 value = 0x0000'0000U;
    const auto& f_value = kstd::utils::transmute<const kstd::f32>(value);
    ASSERT_EQ(f_value, 0.0F);
}

TEST(kstd_utils, test_to_mbs) {
    const std::wstring value = LR"(What does the fox say? 🦊)";
    const auto mb_value = kstd::utils::to_mbs(value);
    ASSERT_EQ(mb_value, R"(What does the fox say? 🦊)");
}

TEST(kstd_utils, test_to_mbs_from_pointer) {
    const auto* value = LR"(What does the fox say? 🦊)";
    const auto mb_value = kstd::utils::to_mbs(value);
    ASSERT_EQ(mb_value, R"(What does the fox say? 🦊)");
}

TEST(kstd_utils, test_to_wcs) {
    const std::string value = R"(What does the fox say? 🦊)";
    const auto wc_value = kstd::utils::to_wcs(value);
    ASSERT_EQ(wc_value, LR"(What does the fox say? 🦊)");
}

TEST(kstd_utils, test_to_wcs_from_pointer) {
    const auto* value = R"(What does the fox say? 🦊)";
    const auto wc_value = kstd::utils::to_wcs(value);
    ASSERT_EQ(wc_value, LR"(What does the fox say? 🦊)");
}