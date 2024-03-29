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
 * @since 13/05/2023
 */

#pragma once

#if __cplusplus > 202002L && __cpp_concepts >= 202002L
#define KSTD_CPP_23
#endif

#if __cplusplus >= 202002L
#define KSTD_CPP_20
#endif

#if __cplusplus >= 201703L
#define KSTD_CPP_17
#endif

#if __cplusplus >= 201402L
#define KSTD_CPP_14
#endif

#if __cplusplus >= 201103L
#define KSTD_CPP_11
#endif