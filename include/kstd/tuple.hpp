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
 * @since 08/05/2023
 */

#pragma once

#include <utility>
#include "types.hpp"
#include "meta.hpp"

// Forward declarations for optional support types
#ifdef PLATFORM_APPLE
namespace std {
    namespace __1 {
        template<typename... ELEMENTS>
        class tuple; // NOLINT
    }

    template<typename... ELEMENTS>
    using tuple = __1::tuple<ELEMENTS...>; // NOLINT
}
#else
namespace std {
    template<typename... ELEMENTS>
    class tuple; // NOLINT
}
#endif

namespace kstd {
    namespace {
        template<usize INDEX, typename... TYPES>
        struct TupleInner;

        template<usize INDEX, typename HEAD>
        struct TupleInner<INDEX, HEAD> final {
            static constexpr usize index = INDEX;

            HEAD _head;

            constexpr TupleInner() noexcept :
                    _head() {
            }

            constexpr TupleInner(HEAD head) noexcept : // NOLINT
                    _head() {
                if constexpr (std::is_pointer<HEAD>::value || std::is_reference<HEAD>::value) {
                    _head = head;
                }
                else {
                    _head = std::move(head);
                }
            }

            ~TupleInner() noexcept = default;
        };

        template<usize INDEX, typename HEAD, typename... TAIL>
        struct TupleInner<INDEX, HEAD, TAIL...> final {
            static constexpr usize index = INDEX;

            HEAD _head;
            TupleInner<INDEX + 1, TAIL...> _tail;

            constexpr TupleInner() noexcept :
                    _head(),
                    _tail() {
            }

            constexpr TupleInner(HEAD head, TAIL&& ... tail) noexcept :  // NOLINT
                    _head(),
                    _tail(std::forward<TAIL>(tail)...) {
                if constexpr (std::is_pointer<HEAD>::value || std::is_reference<HEAD>::value) {
                    _head = head;
                }
                else {
                    _head = std::move(head);
                }
            }

            ~TupleInner() noexcept = default;
        };
    }

    template<typename... TYPES>
    struct Tuple final {
        static constexpr usize num_values = sizeof...(TYPES);

        using self_type [[maybe_unused]] = Tuple<TYPES...>;
        using std_type = std::tuple<TYPES...>;
        using types = Pack<TYPES...>;

        private:

        TupleInner<0, TYPES...> _inner;

        template<usize INDEX, usize CURRENT, typename HEAD, typename... TAIL>
        [[nodiscard]] constexpr auto _get(TupleInner<CURRENT, HEAD, TAIL...>& inner) noexcept -> typename nth_pack_element<INDEX, types>::type {
            if constexpr (CURRENT < INDEX) {
                return _get<INDEX, CURRENT + 1, TAIL...>(inner._tail);
            }

            return inner._head;
        }

        public:

        constexpr Tuple() noexcept :
                _inner() {
        }

        constexpr Tuple(TYPES&& ... values) noexcept : // NOLINT
                _inner(std::forward<TYPES>(values)...) {
        }

        ~Tuple() noexcept = default;

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            return num_values;
        }

        template<usize INDEX>
        [[nodiscard]] constexpr auto get() noexcept -> typename nth_pack_element<INDEX, types>::type {
            return _get<INDEX, 0, TYPES...>(_inner);
        }
    };

    template<typename L, typename R> //
    using Pair = Tuple<L, R>;

    template<typename L, typename M, typename R> //
    using Triple = Tuple<L, M, R>;
}

// Specializations for std::tuple_size and std::tuple_element to allow structured bindings
namespace std { // @formatter:off
    template<typename... ARGS> //
    struct tuple_size<kstd::Tuple<ARGS...>> { // NOLINT
        using value_type = size_t;
        static constexpr value_type value = sizeof...(ARGS);
        using type = std::integral_constant<value_type, value>;

        [[nodiscard]] constexpr auto operator ()() const noexcept -> value_type {
            return value;
        }

        [[nodiscard]] constexpr operator value_type() const noexcept { // NOLINT
            return value;
        }
    };

    template<size_t INDEX, typename HEAD, typename... TAIL>
    struct tuple_element<INDEX, kstd::Tuple<HEAD, TAIL...>> : tuple_element<INDEX - 1, kstd::Tuple<TAIL...>> { // NOLINT
    };

    template<typename HEAD, typename... TAIL>
    struct tuple_element<0, kstd::Tuple<HEAD, TAIL...>> { // NOLINT
        using type = HEAD;
    };
} // @formatter:on