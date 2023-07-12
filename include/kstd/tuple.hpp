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

#include <type_traits>

#include "box.hpp"
#include "defaults.hpp"
#include "pack.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace kstd {
    namespace {
        template<typename... TYPES>
        struct TupleInner;

        template<typename HEAD>
        struct TupleInner<HEAD> {
            static_assert(!std::is_same_v<std::remove_all_extents_t<HEAD>, Void>, "Type cannot be Void");

            // clang-format off
            using head_type = HEAD;
            using self      = TupleInner<head_type>;
            // clang-format on

            Box<head_type> _head;

            KSTD_DEFAULT_MOVE_COPY(TupleInner, self, constexpr)

            constexpr TupleInner() noexcept :
                    _head() {
            }

            constexpr TupleInner(head_type head) noexcept :// NOLINT
                    _head(std::forward<head_type>(head)) {
            }

            ~TupleInner() noexcept = default;
        };

        template<typename HEAD, typename... TAIL>
        struct TupleInner<HEAD, TAIL...> {
            static_assert(!std::is_same_v<std::remove_all_extents_t<HEAD>, Void>, "Type cannot be Void");

            // clang-format off
            using head_type = HEAD;
            using self      = TupleInner<head_type, TAIL...>;
            // clang-format on

            Box<head_type> _head;
            TupleInner<TAIL...> _tail;

            KSTD_DEFAULT_MOVE_COPY(TupleInner, self, constexpr)

            constexpr TupleInner() noexcept :
                    _head(),
                    _tail() {
            }

            constexpr TupleInner(head_type head, TAIL... tail) noexcept :// NOLINT
                    _head(std::forward<head_type>(head)),
                    _tail(std::forward<TAIL>(tail)...) {
            }

            ~TupleInner() noexcept = default;
        };
    }// namespace

    /**
     * The actual implementation of the tuple type which deduces
     * the variadic list of types to be constructed from, by using
     * the given pack of types. This is to allow interfacing with
     * the pack metaprogramming API.
     *
     * @tparam PACK The pack of types to create the tuple from.
     *  Each type in the given pack will be unrolled into a field
     *  within the instantiated tuple template type.
     */
    template<typename PACK>
    struct TupleImpl;

    template<typename... TYPES>
    struct TupleImpl<Pack<TYPES...>> {
        static constexpr usize num_values = sizeof...(TYPES);

        // clang-format off
        using types = Pack<TYPES...>;
        using self  = TupleImpl<types>;
        // clang-format on

        private:
        TupleInner<TYPES...> _inner;

        template<usize INDEX, usize CURRENT, typename HEAD, typename... TAIL>
        constexpr auto get_head(TupleInner<HEAD, TAIL...>& inner) noexcept -> Box<pack_element<INDEX, types>>& {
            if constexpr(CURRENT == INDEX) {
                return inner._head;
            }
            else {
                return get_head<INDEX, CURRENT + 1, TAIL...>(inner._tail);
            }
        }

        template<usize INDEX, usize CURRENT, typename HEAD, typename... TAIL>
        constexpr auto get_head(const TupleInner<HEAD, TAIL...>& inner) const noexcept
                -> const Box<pack_element<INDEX, types>>& {
            if constexpr(CURRENT == INDEX) {
                return inner._head;
            }
            else {
                return get_head<INDEX, CURRENT + 1, TAIL...>(inner._tail);
            }
        }

        template<usize BEGIN, usize END, usize INDEX>
        constexpr auto slice(TupleImpl<slice_pack<BEGIN, END, types>>& tuple) const noexcept -> void {
            tuple.template get_head<INDEX>() = get_head<BEGIN + INDEX>();
            if constexpr(INDEX < (END - BEGIN)) {
                slice<BEGIN, END, INDEX + 1>(tuple);
            }
        }

        template<usize CURRENT, typename HEAD, typename... TAIL>
        constexpr auto equals(const TupleInner<HEAD, TAIL...>& lhs, const TupleInner<HEAD, TAIL...>& rhs,
                              bool& result) const noexcept -> void {
            result = result && (lhs._head == rhs._head);

            if constexpr(CURRENT < num_values - 1) {
                equals<CURRENT + 1, TAIL...>(lhs._tail, rhs._tail, result);
            }
        }

        template<usize CURRENT, typename HEAD, typename... TAIL>
        constexpr auto not_equals(const TupleInner<HEAD, TAIL...>& lhs, const TupleInner<HEAD, TAIL...>& rhs,
                                  bool& result) const noexcept -> void {
            result = result || (lhs._head != rhs._head);

            if constexpr(CURRENT < num_values - 1) {
                equals<CURRENT + 1, TAIL...>(lhs._tail, rhs._tail, result);
            }
        }

        template<usize NEW_SIZE, usize CURRENT, typename... OTHER_TYPES>
        constexpr auto concat(const TupleImpl<Pack<OTHER_TYPES...>>& other,
                              TupleImpl<Pack<TYPES..., OTHER_TYPES...>>& result) const noexcept -> void {
            if constexpr(CURRENT < num_values) {
                result.template get_head<CURRENT>() = get_head<CURRENT>();
            }
            else {
                result.template get_head<CURRENT>() = other.template get_head<CURRENT - num_values>();
            }
            if constexpr(CURRENT < NEW_SIZE - 1) {
                concat<NEW_SIZE, CURRENT + 1, OTHER_TYPES...>(other, result);
            }
        }

        template<typename X>
        struct GetMutableTransform final {
            // clang-format off
            using type = std::conditional_t<
                !std::is_pointer_v<X> && !std::is_reference_v<X>,
                X&,
                X>;
            // clang-format on
        };

        template<typename X>
        struct GetImmutableTransform final {
            // clang-format off
            using type = std::conditional_t<
                !std::is_pointer_v<X> && !std::is_reference_v<X>,
                X const&,
                X>;
            // clang-format on
        };

        public:
        KSTD_DEFAULT_MOVE_COPY(TupleImpl, self, constexpr)

        constexpr TupleImpl() noexcept :
                _inner() {
        }

        constexpr TupleImpl(TYPES... values) noexcept :// NOLINT
                _inner(std::forward<TYPES>(values)...) {
        }

        ~TupleImpl() noexcept = default;

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            return num_values;
        }

        template<usize INDEX>
        [[nodiscard]] constexpr auto get_head() noexcept -> Box<pack_element<INDEX, types>>& {
            return get_head<INDEX, 0, TYPES...>(_inner);
        }

        template<usize INDEX>
        [[nodiscard]] constexpr auto get_head() const noexcept -> const Box<pack_element<INDEX, types>>& {
            return get_head<INDEX, 0, TYPES...>(_inner);
        }

        template<usize INDEX>
        [[nodiscard]] constexpr auto get() noexcept -> pack_element<INDEX, transform_pack<GetMutableTransform, types>> {
            return get_head<INDEX, 0, TYPES...>(_inner).get();
        }

        template<usize INDEX>
        [[nodiscard]] constexpr auto get() const noexcept
                -> pack_element<INDEX, transform_pack<GetImmutableTransform, types>> {
            return get_head<INDEX, 0, TYPES...>(_inner).get();
        }

        template<usize BEGIN, usize END>
        [[nodiscard]] constexpr auto slice() const noexcept -> TupleImpl<slice_pack<BEGIN, END, types>> {
            TupleImpl<slice_pack<BEGIN, END, types>> result;
            slice<BEGIN, END, 0>(result);
            return result;
        }

        template<typename... OTHER_TYPES>
        [[nodiscard]] constexpr auto concat(const TupleImpl<Pack<OTHER_TYPES...>>& other) const noexcept
                -> TupleImpl<Pack<TYPES..., OTHER_TYPES...>> {
            TupleImpl<Pack<TYPES..., OTHER_TYPES...>> result;
            concat<num_values + sizeof...(OTHER_TYPES), 0, OTHER_TYPES...>(other, result);
            return result;
        }

        template<typename... OTHER_TYPES>
        [[nodiscard]] constexpr auto operator+(const TupleImpl<Pack<OTHER_TYPES...>>& other) const noexcept
                -> TupleImpl<Pack<TYPES..., OTHER_TYPES...>> {
            return concat<OTHER_TYPES...>(other);
        }

        template<typename OTHER_TYPES>
        [[nodiscard]] constexpr auto operator==(const TupleImpl<OTHER_TYPES>& other) const noexcept -> bool {
            return false;
        }

        [[nodiscard]] constexpr auto operator==(const self& other) const noexcept -> bool {
            bool result = true;
            equals<0>(_inner, other._inner, result);
            return result;
        }

        template<typename OTHER_TYPES>
        [[nodiscard]] constexpr auto operator!=(const TupleImpl<OTHER_TYPES>& other) const noexcept -> bool {
            return true;
        }

        [[nodiscard]] constexpr auto operator!=(const self& other) const noexcept -> bool {
            bool result = false;
            not_equals<0>(_inner, other._inner, result);
            return !result;
        }
    };

    template<typename PACK>
    using PackTuple = TupleImpl<PACK>;

    template<typename... TYPES>
    using Tuple = TupleImpl<Pack<TYPES...>>;

    template<typename L, typename R>
    using Pair = Tuple<L, R>;

    template<typename L, typename M, typename R>
    using Triple = Tuple<L, M, R>;
}// namespace kstd

// Specializations for std::tuple_size and std::tuple_element to allow structured bindings
namespace std {
    template<typename... ARGS>
    struct tuple_size<kstd::Tuple<ARGS...>> {
        using value_type = kstd::usize;
        static constexpr value_type value = sizeof...(ARGS);
        using type = std::integral_constant<value_type, value>;

        [[nodiscard]] constexpr auto operator()() const noexcept -> value_type {
            return value;
        }

        [[nodiscard]] constexpr operator value_type() const noexcept {
            return value;
        }
    };

    template<size_t INDEX, typename HEAD, typename... TAIL>
    struct tuple_element<INDEX, kstd::Tuple<HEAD, TAIL...>> : tuple_element<INDEX - 1, kstd::Tuple<TAIL...>> {};

    template<typename HEAD, typename... TAIL>
    struct tuple_element<0, kstd::Tuple<HEAD, TAIL...>> {
        using type = HEAD;
    };
}// namespace std
