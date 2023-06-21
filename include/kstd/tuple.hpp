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

#include "box.hpp"
#include "defaults.hpp"
#include "meta.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace kstd {
    namespace {
        template<typename... TYPES>
        struct TupleInner;

        template<typename HEAD>
        struct TupleInner<HEAD> {
            using ValueType = HEAD;

            Box<ValueType> _head;

            KSTD_DEFAULT_MOVE_COPY(TupleInner)

            constexpr TupleInner() noexcept :
                    _head() {
            }

            constexpr TupleInner(ValueType head) noexcept :// NOLINT
                    _head(utils::move_or_copy(head)) {
            }

            ~TupleInner() noexcept = default;
        };

        template<typename HEAD, typename... TAIL>
        struct TupleInner<HEAD, TAIL...> {
            using ValueType = HEAD;

            Box<ValueType> _head;
            TupleInner<TAIL...> _tail;

            KSTD_DEFAULT_MOVE_COPY(TupleInner)

            constexpr TupleInner() noexcept :
                    _head(),
                    _tail() {
            }

            constexpr TupleInner(HEAD head, TAIL&&... tail) noexcept :// NOLINT
                    _head(utils::move_or_copy(head)),
                    _tail(utils::forward<TAIL>(tail)...) {
            }

            ~TupleInner() noexcept = default;
        };
    }// namespace

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
        constexpr auto get_head(TupleInner<HEAD, TAIL...>& inner) noexcept -> Box<meta::PackElement<INDEX, Types>>& {
            if constexpr(CURRENT == INDEX) {
                return inner._head;
            }
            else {
                return get_head<INDEX, CURRENT + 1, TAIL...>(inner._tail);
            }
        }

        template<usize INDEX, usize CURRENT, typename HEAD, typename... TAIL>
        constexpr auto get_head(const TupleInner<HEAD, TAIL...>& inner) const noexcept
                -> const Box<meta::PackElement<INDEX, Types>>& {
            if constexpr(CURRENT == INDEX) {
                return inner._head;
            }
            else {
                return get_head<INDEX, CURRENT + 1, TAIL...>(inner._tail);
            }
        }

        template<usize BEGIN, usize END, usize INDEX>
        constexpr auto slice(TupleImpl<meta::SlicePack<BEGIN, END, Types>>& tuple) const noexcept -> void {
            tuple.template get<INDEX>() = get<BEGIN + INDEX>();
            if constexpr(INDEX < (END - BEGIN)) {
                slice<BEGIN, END, INDEX + 1>(tuple);
            }
        }

        template<usize CURRENT, typename HEAD, typename... TAIL>
        constexpr auto equals(const TupleInner<HEAD, TAIL...>& lhs, const TupleInner<HEAD, TAIL...>& rhs,
                              bool& result) const noexcept -> void {
            if constexpr(meta::has_equals_op<HEAD>) {
                result = result && (lhs._head == rhs._head);
            }
            else {
                result = result && !(lhs._head != rhs._head);// NOLINT
            }

            if constexpr(CURRENT < num_values - 1) {
                equals<CURRENT + 1, TAIL...>(lhs._tail, rhs._tail, result);
            }
        }

        template<usize CURRENT, typename HEAD, typename... TAIL>
        constexpr auto not_equals(const TupleInner<HEAD, TAIL...>& lhs, const TupleInner<HEAD, TAIL...>& rhs,
                                  bool& result) const noexcept -> void {
            if constexpr(meta::has_not_equals_op<HEAD>) {
                result = result || (lhs._head != rhs._head);
            }
            else {
                result = result || !(lhs._head == rhs._head);// NOLINT
            }

            if constexpr(CURRENT < num_values - 1) {
                equals<CURRENT + 1, TAIL...>(lhs._tail, rhs._tail, result);
            }
        }

        template<usize NEW_SIZE, usize CURRENT, typename... OTHER_TYPES>
        constexpr auto concat(const TupleImpl<meta::Pack<OTHER_TYPES...>>& other,
                              TupleImpl<meta::Pack<TYPES..., OTHER_TYPES...>>& result) const noexcept -> void {
            if constexpr(CURRENT < num_values) {
                result.template reset<CURRENT>(get<CURRENT>());
            }
            else {
                result.template reset<CURRENT>(other.template get<CURRENT - num_values>());
            }
            if constexpr(CURRENT < NEW_SIZE - 1) {
                concat<NEW_SIZE, CURRENT + 1, OTHER_TYPES...>(other, result);
            }
        }

        public:
        KSTD_DEFAULT_MOVE_COPY(TupleImpl)

        constexpr TupleImpl() noexcept :
                _inner() {
        }

        constexpr TupleImpl(TYPES&&... values) noexcept :// NOLINT
                _inner(utils::forward<TYPES>(values)...) {
        }

        ~TupleImpl() noexcept = default;

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            return num_values;
        }

        template<usize INDEX>
        constexpr auto reset(meta::PackElement<INDEX, Types> value) noexcept -> void {
            get_head<INDEX, 0, TYPES...>(_inner) = utils::move_or_copy(value);
        }

        template<usize INDEX>
        [[nodiscard]] constexpr auto get() noexcept -> meta::Ref<meta::PackElement<INDEX, Types>> {
            return get_head<INDEX, 0, TYPES...>(_inner).borrow();
        }

        template<usize INDEX>
        [[nodiscard]] constexpr auto get() const noexcept -> meta::ConstRef<meta::PackElement<INDEX, Types>> {
            return get_head<INDEX, 0, TYPES...>(_inner).borrow();
        }

        template<usize BEGIN, usize END>
        [[nodiscard]] constexpr auto slice() const noexcept -> TupleImpl<meta::SlicePack<BEGIN, END, Types>> {
            TupleImpl<meta::SlicePack<BEGIN, END, Types>> result;
            slice<BEGIN, END, 0>(result);
            return result;
        }

        template<typename... OTHER_TYPES>
        [[nodiscard]] constexpr auto concat(const TupleImpl<meta::Pack<OTHER_TYPES...>>& other) const noexcept
                -> TupleImpl<meta::Pack<TYPES..., OTHER_TYPES...>> {
            TupleImpl<meta::Pack<TYPES..., OTHER_TYPES...>> result;
            concat<num_values + sizeof...(OTHER_TYPES), 0, OTHER_TYPES...>(other, result);
            return result;
        }

        template<typename... OTHER_TYPES>
        [[nodiscard]] constexpr auto operator+(const TupleImpl<meta::Pack<OTHER_TYPES...>>& other) const noexcept
                -> TupleImpl<meta::Pack<TYPES..., OTHER_TYPES...>> {
            return concat<OTHER_TYPES...>(other);
        }

        [[nodiscard]] constexpr auto operator==(const Self& other) const noexcept -> bool {
            bool result = true;
            equals<0>(_inner, other._inner, result);
            return result;
        }

        [[nodiscard]] constexpr auto operator!=(const Self& other) const noexcept -> bool {
            bool result = false;
            not_equals<0>(_inner, other._inner, result);
            return !result;
        }
    };

    template<typename PACK>
    using PackTuple = TupleImpl<PACK>;

    template<typename... TYPES>
    using Tuple = TupleImpl<meta::Pack<TYPES...>>;

    template<typename L, typename R>
    using Pair = Tuple<L, R>;

    template<typename L, typename M, typename R>
    using Triple = Tuple<L, M, R>;
}// namespace kstd

// Specializations for std::tuple_size and std::tuple_element to allow structured bindings
namespace std {// @formatter:off
    template<typename... ARGS>
    struct tuple_size<kstd::Tuple<ARGS...>> {// NOLINT
        using value_type = kstd::usize;      // NOLINT: style mismatch
        static constexpr value_type value = sizeof...(ARGS);
        using type = kstd::meta::Constant<value_type, value>;// NOLINT: style mismatch

        [[nodiscard]] constexpr auto operator()() const noexcept -> value_type {
            return value;
        }

        [[nodiscard]] constexpr operator value_type() const noexcept {// NOLINT
            return value;
        }
    };

    template<size_t INDEX, typename HEAD, typename... TAIL>
    struct tuple_element<INDEX, kstd::Tuple<HEAD, TAIL...>> : tuple_element<INDEX - 1, kstd::Tuple<TAIL...>> {// NOLINT
    };

    template<typename HEAD, typename... TAIL>
    struct tuple_element<0, kstd::Tuple<HEAD, TAIL...>> {// NOLINT
        using type = HEAD;                               // NOLINT: style mismatch
    };
}// namespace std
