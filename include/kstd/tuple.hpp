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

#include "types.hpp"
#include "meta.hpp"
#include "box.hpp"
#include "utils.hpp"

namespace kstd {
    namespace {
        template<typename T, typename = void> // NOLINT
        struct NakedTypeIfRefImpl {
            using Type = T;
        };

        template<typename T>
        struct NakedTypeIfRefImpl<T, meta::def_if<meta::is_ref<T>>> {
            using Type = meta::remove_ref<T>;
        };

        template<typename T>
        struct NakedTypeIfRef final {
            using Type = typename NakedTypeIfRefImpl<T>::Type;
        };

        template<typename... TYPES>
        struct TupleInner;

        template<typename HEAD>
        struct TupleInner<HEAD> {
            using ValueType = HEAD;

            Box<ValueType> _head;

            constexpr TupleInner() noexcept :
                    _head() {
            }

            constexpr TupleInner(ValueType head) noexcept : // NOLINT
                    _head(move_or_copy(head)) {
            }

            ~TupleInner() noexcept = default;
        };

        template<typename HEAD, typename... TAIL>
        struct TupleInner<HEAD, TAIL...> {
            using ValueType = HEAD;

            Box<ValueType> _head;
            TupleInner<TAIL...> _tail;

            constexpr TupleInner() noexcept :
                    _head(),
                    _tail() {
            }

            constexpr TupleInner(HEAD head, TAIL&& ... tail) noexcept :  // NOLINT
                    _head(move_or_copy(head)),
                    _tail(forward<TAIL>(tail)...) {
            }

            ~TupleInner() noexcept = default;
        };

        template<typename PACK>
        struct TupleImpl;

        template<typename... TYPES>
        struct TupleImpl<meta::Pack<TYPES...>> {
            static constexpr usize num_values = sizeof...(TYPES);

            using Types = meta::Pack<TYPES...>;
            using Self [[maybe_unused]] = TupleImpl<Types>;

            private:

            TupleInner<TYPES...> _inner;

            template<usize INDEX, usize CURRENT, typename HEAD, typename... TAIL>
            [[nodiscard]] constexpr auto _get(TupleInner<HEAD, TAIL...>& inner) noexcept -> meta::lvalue_ref<meta::pack_element<INDEX, Types>> {
                if constexpr (CURRENT == INDEX) {
                    return inner._head.borrow();
                }
                else {
                    return _get<INDEX, CURRENT + 1, TAIL...>(inner._tail);
                }
            }

            template<usize INDEX, usize CURRENT, typename HEAD, typename... TAIL>
            [[nodiscard]] constexpr auto _get(const TupleInner<HEAD, TAIL...>& inner) const noexcept -> meta::const_lvalue_ref<meta::pack_element<INDEX, Types>> {
                if constexpr (CURRENT == INDEX) {
                    return inner._head.borrow();
                }
                else {
                    return _get<INDEX, CURRENT + 1, TAIL...>(inner._tail);
                }
            }

            template<usize BEGIN, usize END, usize INDEX>
            constexpr auto _slice(TupleImpl<meta::slice_pack<BEGIN, END, Types>>& tuple) const noexcept -> void {
                tuple.template get<INDEX>() = get<BEGIN + INDEX>();

                if constexpr (INDEX < (END - BEGIN)) {
                    _slice<BEGIN, END, INDEX + 1>(tuple);
                }
            }

            template<usize CURRENT, typename HEAD, typename... TAIL>
            constexpr auto _equals(const TupleInner<HEAD, TAIL...>& a, const TupleInner<HEAD, TAIL...>& b, bool& result) const noexcept -> void {
                if constexpr (meta::has_equals_op<HEAD>) {
                    result = result && (a._head == b._head);
                }
                else {
                    result = result && !(a._head != b._head); // NOLINT
                }

                if constexpr (CURRENT < num_values - 1) {
                    _equals<CURRENT + 1, TAIL...>(a._tail, b._tail, result);
                }
            }

            template<usize CURRENT, typename HEAD, typename... TAIL>
            constexpr auto _not_equals(const TupleInner<HEAD, TAIL...>& a, const TupleInner<HEAD, TAIL...>& b, bool& result) const noexcept -> void {
                if constexpr (meta::has_not_equals_op<HEAD>) {
                    result = result || (a._head != b._head);
                }
                else {
                    result = result || !(a._head == b._head); // NOLINT
                }

                if constexpr (CURRENT < num_values - 1) {
                    _equals<CURRENT + 1, TAIL...>(a._tail, b._tail, result);
                }
            }

            template<usize NEW_SIZE, usize CURRENT, typename... OTHER_TYPES> // @formatter:off
            constexpr auto _concat(const TupleImpl<meta::Pack<OTHER_TYPES...>>& other,
                    TupleImpl<meta::transform_pack<NakedTypeIfRef, meta::Pack<TYPES..., OTHER_TYPES...>>>& result) const noexcept -> void { // @formatter:on
                if constexpr (CURRENT < num_values) {
                    result.template get<CURRENT>() = get<CURRENT>();
                }
                else {
                    result.template get<CURRENT>() = other.template get<CURRENT - num_values>();
                }

                if constexpr (CURRENT < NEW_SIZE - 1) {
                    _concat<NEW_SIZE, CURRENT + 1, OTHER_TYPES...>(other, result);
                }
            }

            public:

            constexpr TupleImpl() noexcept :
                    _inner() {
            }

            constexpr TupleImpl(TYPES&& ... values) noexcept : // NOLINT
                    _inner(forward<TYPES>(values)...) {
            }

            ~TupleImpl() noexcept = default;

            [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
                return num_values;
            }

            template<usize INDEX>
            [[nodiscard]] constexpr auto get() noexcept -> meta::lvalue_ref<meta::pack_element<INDEX, Types>> {
                return _get<INDEX, 0, TYPES...>(_inner);
            }

            template<usize INDEX>
            [[nodiscard]] constexpr auto get() const noexcept -> meta::const_lvalue_ref<meta::pack_element<INDEX, Types>> {
                return _get<INDEX, 0, TYPES...>(_inner);
            }

            template<usize BEGIN, usize END>
            [[nodiscard]] constexpr auto slice() const noexcept -> TupleImpl<meta::slice_pack<BEGIN, END, Types>> {
                TupleImpl<meta::slice_pack<BEGIN, END, Types>> result;
                _slice<BEGIN, END, 0>(result);
                return result;
            }

            template<typename... OTHER_TYPES> // @formatter:off
            [[nodiscard]] constexpr auto concat(const TupleImpl<meta::Pack<OTHER_TYPES...>>& other) const noexcept ->
                    TupleImpl<meta::transform_pack<NakedTypeIfRef, meta::Pack<TYPES..., OTHER_TYPES...>>> { // @formatter:on
                TupleImpl<meta::transform_pack<NakedTypeIfRef, meta::Pack<TYPES..., OTHER_TYPES...>>> result;
                _concat<num_values + sizeof...(OTHER_TYPES), 0, OTHER_TYPES...>(other, result);
                return result;
            }

            template<typename... OTHER_TYPES> // @formatter:off
            [[nodiscard]] constexpr auto operator +(const TupleImpl<meta::Pack<OTHER_TYPES...>>& other) const noexcept ->
                    TupleImpl<meta::transform_pack<NakedTypeIfRef, meta::Pack<TYPES..., OTHER_TYPES...>>> { // @formatter:on
                return concat<OTHER_TYPES...>(other);
            }

            [[nodiscard]] constexpr auto operator ==(const Self& other) const noexcept -> bool {
                bool result = true;
                _equals<0>(_inner, other._inner, result);
                return result;
            }

            [[nodiscard]] constexpr auto operator !=(const Self& other) const noexcept -> bool {
                bool result = false;
                _not_equals<0>(_inner, other._inner, result);
                return !result;
            }
        };
    }

    template<typename... TYPES> //
    using Tuple = TupleImpl<meta::Pack<TYPES...>>;

    template<typename L, typename R> //
    using Pair = Tuple<L, R>;

    template<typename L, typename M, typename R> //
    using Triple = Tuple<L, M, R>;

    namespace meta {
        template<typename PACK> //
        using tuple_from_pack = TupleImpl<PACK>;
    }
}

// Specializations for std::tuple_size and std::tuple_element to allow structured bindings
namespace std { // @formatter:off
    template<typename... ARGS> //
    struct tuple_size<kstd::Tuple<ARGS...>> { // NOLINT
        using value_type = kstd::usize;
        static constexpr value_type value = sizeof...(ARGS);
        using type = kstd::meta::Constant<value_type, value>;

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