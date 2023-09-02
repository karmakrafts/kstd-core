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
 * @since 01/09/2023
 */

#pragma once

#include "assert.hpp"
#include "defaults.hpp"
#include "types.hpp"

#include <array>
#include <iterator>
#include <utility>
#include <vector>

namespace kstd {
    template<typename T, usize SIZE>
    struct StaticVector final {
        using ValueType = T;
        using Self = StaticVector<ValueType, SIZE>;
        using Reference = ValueType&;
        using ConstReference = const ValueType&;
        using Pointer = ValueType*;
        using ConstPointer = const ValueType*;
        using Iterator = Pointer;
        using ConstIterator = ConstPointer;
        using ReverseIterator = std::reverse_iterator<Iterator>;
        using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

        static constexpr usize size = SIZE;

        private:
        std::array<ValueType, size> _data;
        usize _index;

        public:
        KSTD_DEFAULT_MOVE_COPY(StaticVector, Self, constexpr)

        constexpr StaticVector() noexcept :
                _data {},
                _index {0} {
        }

        template<typename HEAD, typename... TAIL>
        constexpr StaticVector(HEAD&& head, TAIL&&... tail) noexcept :
                _data {std::forward<HEAD>(head), std::forward<TAIL>(tail)...},
                _index {1 + sizeof...(TAIL)} {
            static_assert(std::is_convertible_v<HEAD, ValueType> && (std::is_convertible_v<TAIL, ValueType> && ...),
                          "One of the parameters is not convertible to the value type of the vector");
        }

        constexpr StaticVector(Iterator begin, Iterator end) noexcept :
                _data {begin, end},
                _index {reinterpret_cast<usize>(&*end - &*begin)} {
        }

        constexpr StaticVector(ConstIterator begin, ConstIterator end) noexcept :
                _data {begin, end},
                _index {reinterpret_cast<usize>(&*end - &*begin)} {
        }

        ~StaticVector() noexcept = default;

        [[nodiscard]] constexpr auto allocate() const noexcept -> std::vector<ValueType> {
            return {_data.cbegin(), _data.cend()};
        }

        [[nodiscard]] constexpr auto to_array() const noexcept -> std::array<ValueType, size> {
            return {_data.cbegin(), _data.cend()};
        }

        constexpr auto clear() noexcept -> void {
            std::fill(_data.begin(), _data.end(), {});
            _index = 0;
        }

        template<typename... ARGS>
        constexpr auto emplace_back(ARGS&&... args) noexcept -> void {
            assert_true(_index < size);
            _data[_index] = ValueType(std::forward<ARGS>(args)...);
            ++_index;
        }

        constexpr auto push_back(ValueType value) noexcept -> void {
            assert_true(_index < size);
            _data[_index] = std::move(value);
            ++_index;
        }

        constexpr auto pop_back() noexcept -> ValueType {
            assert_true(_index > 0);
            return std::move(_data[--_index]);
        }

        template<typename... ARGS>
        constexpr auto emplace_front(ARGS&&... args) noexcept -> void {
            assert_true(_index < size);
            std::copy(_data.cbegin(), _data.cend() - 1, _data.begin() + 1);
            _data[0] = ValueType(std::forward<ARGS>(args)...);
            ++_index;
        }

        constexpr auto push_front(ValueType value) noexcept -> void {
            assert_true(_index < size);
            std::copy(_data.cbegin(), _data.cend() - 1, _data.begin() + 1);
            _data[0] = std::move(value);
            ++_index;
        }

        constexpr auto pop_front() noexcept -> ValueType {
            assert_true(_index > 0);
            auto result = std::move(_data[0]);
            std::copy(_data.cbegin() + 1, _data.cend(), _data.begin());
            --_index;
            return result;
        }

        constexpr auto insert(const usize index, ValueType value) noexcept -> void {
            assert_true(_index < size);
        }

        [[nodiscard]] constexpr auto get_capacity() const noexcept -> usize {
            return size;
        }

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            return _index;
        }

        [[nodiscard]] constexpr auto get_data() noexcept -> Pointer {
            return _data.data();
        }

        [[nodiscard]] constexpr auto get_data() const noexcept -> ConstPointer {
            return _data.data();
        }

        [[nodiscard]] constexpr auto begin() noexcept -> Iterator {
            return _data.begin();
        }

        [[nodiscard]] constexpr auto end() noexcept -> Iterator {
            return _data.begin() + _index;
        }

        [[nodiscard]] constexpr auto rbegin() noexcept -> ReverseIterator {
            return _data.rbegin();
        }

        [[nodiscard]] constexpr auto rend() noexcept -> ReverseIterator {
            return _data.rbegin() + _index;
        }

        [[nodiscard]] constexpr auto cbegin() const noexcept -> ConstIterator {
            return _data.cbegin();
        }

        [[nodiscard]] constexpr auto cend() const noexcept -> ConstIterator {
            return _data.cbegin() + _index;
        }

        [[nodiscard]] constexpr auto crbegin() const noexcept -> ConstReverseIterator {
            return _data.crbegin();
        }

        [[nodiscard]] constexpr auto crend() const noexcept -> ConstReverseIterator {
            return _data.crbegin() + _index;
        }

        [[nodiscard]] constexpr auto at(const usize index) noexcept -> Reference {
            return _data[index];
        }

        [[nodiscard]] constexpr auto at(const usize index) const noexcept -> ConstReference {
            return _data[index];
        }

        [[nodiscard]] constexpr auto operator[](const usize index) noexcept -> Reference {
            return at(index);
        }

        [[nodiscard]] constexpr auto operator[](const usize index) const noexcept -> ConstReference {
            return at(index);
        }
    };
}// namespace kstd