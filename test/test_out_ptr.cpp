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
 * @since 02/05/2023
 */

#include <gtest/gtest.h>
#include <kstd/rc.hpp>
#include <kstd/out_ptr.hpp>
#include <kstd/types.hpp>

// Define function with C-linkage for testing
extern "C" auto the_c_function(kstd::i32** data_to_set) -> void {
    *data_to_set = reinterpret_cast<kstd::i32*>(::malloc(sizeof(kstd::i32)));
    *(*data_to_set) = 420;
}

TEST(kstd_OutPtr, TestOutPtr) {
    auto the_data = kstd::make_rc<kstd::i32, kstd::Allocator, kstd::FreeDeleter>(nullptr);
    ASSERT_TRUE(the_data == nullptr);

    the_c_function(kstd::make_out(the_data));
    ASSERT_EQ(*the_data, 420);
}