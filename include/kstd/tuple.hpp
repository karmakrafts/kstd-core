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

            using HeadType = HEAD;
            using Self = TupleInner<HeadType>;

            Box<HeadType> _head;

            KSTD_DEFAULT_MOVE_COPY(TupleInner, Self, constexpr)

            constexpr TupleInner() noexcept :
                    _head {} {
            }

            constexpr TupleInner(HeadType head) noexcept :
                    _head {std::forward<HeadType>(head)} {
            }

            ~TupleInner() noexcept = default;
        };

        template<typename HEAD, typename... TAIL>
        struct TupleInner<HEAD, TAIL...> {
            static_assert(!std::is_same_v<std::remove_all_extents_t<HEAD>, Void>, "Type cannot be Void");

            using HeadType = HEAD;
            using Self = TupleInner<HeadType, TAIL...>;

            Box<HeadType> _head;
            TupleInner<TAIL...> _tail;

            KSTD_DEFAULT_MOVE_COPY(TupleInner, Self, constexpr)

            constexpr TupleInner() noexcept :
                    _head {},
                    _tail {} {
            }

            constexpr TupleInner(HeadType head, TAIL&&... tail) noexcept :
                    _head {std::forward<HeadType>(head)},
                    _tail {std::forward<TAIL>(tail)...} {
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
    struct PackedTuple;

    template<typename... TYPES>
    struct PackedTuple<Pack<TYPES...>> {
        static constexpr usize num_values = sizeof...(TYPES);

        using Types = Pack<TYPES...>;
        using Self = PackedTuple<Types>;

        private:
        TupleInner<TYPES...> _inner;

        template<usize INDEX, usize CURRENT, typename HEAD, typename... TAIL>
        constexpr auto get_head(TupleInner<HEAD, TAIL...>& inner) noexcept -> Box<PackElementT<INDEX, Types>>& {
            if constexpr(CURRENT == INDEX) {
                return inner._head;
            }
            else {
                return get_head<INDEX, CURRENT + 1, TAIL...>(inner._tail);
            }
        }

        template<usize INDEX, usize CURRENT, typename HEAD, typename... TAIL>
        constexpr auto get_head(const TupleInner<HEAD, TAIL...>& inner) const noexcept
                -> const Box<PackElementT<INDEX, Types>>& {
            if constexpr(CURRENT == INDEX) {
                return inner._head;
            }
            else {
                return get_head<INDEX, CURRENT + 1, TAIL...>(inner._tail);
            }
        }

        template<usize BEGIN, usize END, usize INDEX>
        constexpr auto slice(PackedTuple<SlicePackT<BEGIN, END, Types>>& tuple) const noexcept -> void {
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
        constexpr auto concat(const PackedTuple<Pack<OTHER_TYPES...>>& other,
                              PackedTuple<Pack<TYPES..., OTHER_TYPES...>>& result) const noexcept -> void {
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
            using Type = std::conditional_t<!std::is_pointer_v<X> && !std::is_reference_v<X>, X&, X>;
        };

        template<typename X>
        struct GetImmutableTransform final {
            using Type = std::conditional_t<!std::is_pointer_v<X> && !std::is_reference_v<X>, X const&, X>;
        };

        public:
        KSTD_DEFAULT_MOVE_COPY(PackedTuple, Self, constexpr)

        constexpr PackedTuple() noexcept :
                _inner {} {
        }

        constexpr PackedTuple(TYPES&&... values) noexcept :
                _inner {std::forward<TYPES>(values)...} {
        }

        ~PackedTuple() noexcept = default;

        [[nodiscard]] constexpr auto size() const noexcept -> usize {
            return num_values;
        }

        template<usize INDEX>
        [[nodiscard]] constexpr auto get_head() noexcept -> Box<PackElementT<INDEX, Types>>& {
            return get_head<INDEX, 0, TYPES...>(_inner);
        }

        template<usize INDEX>
        [[nodiscard]] constexpr auto get_head() const noexcept -> const Box<PackElementT<INDEX, Types>>& {
            return get_head<INDEX, 0, TYPES...>(_inner);
        }

        template<usize INDEX>
        [[nodiscard]] constexpr auto get() noexcept -> PackElementT<INDEX, TransformPackT<GetMutableTransform, Types>> {
            return get_head<INDEX, 0, TYPES...>(_inner).get();
        }

        template<usize INDEX>
        [[nodiscard]] constexpr auto get() const noexcept
                -> PackElementT<INDEX, TransformPackT<GetImmutableTransform, Types>> {
            return get_head<INDEX, 0, TYPES...>(_inner).get();
        }

        template<usize BEGIN, usize END>
        [[nodiscard]] constexpr auto slice() const noexcept -> PackedTuple<SlicePackT<BEGIN, END, Types>> {
            PackedTuple<SlicePackT<BEGIN, END, Types>> result {};
            slice<BEGIN, END, 0>(result);
            return result;
        }

        template<typename... OTHER_TYPES>
        [[nodiscard]] constexpr auto concat(const PackedTuple<Pack<OTHER_TYPES...>>& other) const noexcept
                -> PackedTuple<Pack<TYPES..., OTHER_TYPES...>> {
            PackedTuple<Pack<TYPES..., OTHER_TYPES...>> result {};
            concat<num_values + sizeof...(OTHER_TYPES), 0, OTHER_TYPES...>(other, result);
            return result;
        }

        template<typename... OTHER_TYPES>
        [[nodiscard]] constexpr auto operator+(const PackedTuple<Pack<OTHER_TYPES...>>& other) const noexcept
                -> PackedTuple<Pack<TYPES..., OTHER_TYPES...>> {
            return concat<OTHER_TYPES...>(other);
        }

        template<typename OTHER_TYPES>
        [[nodiscard]] constexpr auto operator==([[maybe_unused]] const PackedTuple<OTHER_TYPES>& other) const noexcept
                -> bool {
            return false;
        }

        [[nodiscard]] constexpr auto operator==(const Self& other) const noexcept -> bool {
            bool result = true;
            equals<0>(_inner, other._inner, result);
            return result;
        }

        template<typename OTHER_TYPES>
        [[nodiscard]] constexpr auto operator!=([[maybe_unused]] const PackedTuple<OTHER_TYPES>& other) const noexcept
                -> bool {
            return true;
        }

        [[nodiscard]] constexpr auto operator!=(const Self& other) const noexcept -> bool {
            bool result = false;
            not_equals<0>(_inner, other._inner, result);
            return !result;
        }
    };

    template<typename... TYPES>
    PackedTuple(TYPES&&...) -> PackedTuple<Pack<TYPES...>>;

    template<typename... TYPES>
    using Tuple = PackedTuple<Pack<TYPES...>>;
}// namespace kstd

namespace std {
    template<typename... TYPES>
    struct hash<kstd::Tuple<TYPES...>> {
        private:
        template<size_t INDEX>
        constexpr auto compute(size_t& result, const kstd::Tuple<TYPES...>& value) const noexcept -> void {
            kstd::combined_hash_into(result, kstd::hash(value.template get_head<INDEX>()));
            if constexpr(INDEX < sizeof...(TYPES)) {
                compute<INDEX + 1>(result, value);
            }
        }

        public:
        constexpr auto operator()(const kstd::Tuple<TYPES...>& value) const noexcept -> size_t {
            size_t result = 0;
            compute<0>(result, value);
            return result;
        }
    };

    template<typename... TYPES>
    struct tuple_size<kstd::Tuple<TYPES...>> {
        using value_type = kstd::usize;// NOLINT: style mismatch
        static constexpr value_type value = sizeof...(TYPES);
        using type = std::integral_constant<value_type, value>;// NOLINT: style mismatch

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
        using type = HEAD;// NOLINT: style mismatch
    };
}// namespace std
