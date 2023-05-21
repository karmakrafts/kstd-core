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
 * @since 12/05/2023
 */

#pragma once

#include "types.hpp"
#include "allocator.hpp"
#include "assert.hpp"

namespace kstd {
    namespace {
        template<typename T, typename COUNTER_TYPE>
        struct RcInner final {
            T _value;
            COUNTER_TYPE _count;

            explicit constexpr RcInner(T value) noexcept :
                    _value(utils::move_or_copy(value)),
                    _count(1) {
            }

            template<typename... ARGS>
            explicit constexpr RcInner(ARGS&& ... args) noexcept :
                    _value(T(utils::forward<ARGS>(args)...)),
                    _count(1) {
            }

            ~RcInner() noexcept = default;
        };
    }

    template<typename T, typename COUNTER_TYPE, // @formatter:off
        template<typename> typename ALLOCATOR = Allocator,
        template<typename> typename DELETER = Deleter> // @formatter:on
    struct BasicRc final {
        using ElementType = T;
        using CounterType = COUNTER_TYPE;
        using InnerType = RcInner<ElementType, CounterType>;
        using Self = BasicRc<ElementType, CounterType, ALLOCATOR, DELETER>;

        private:

        InnerType* _inner;

        public:

        explicit constexpr BasicRc(decltype(nullptr)) noexcept :
                _inner(nullptr) {
        }

        template<typename... ARGS>
        explicit constexpr BasicRc(ARGS&& ... args) noexcept :
                _inner(ALLOCATOR<InnerType>().construct(utils::forward<ARGS>(args)...)) {
        }

        constexpr BasicRc(const Self& other) noexcept :
                _inner(other._inner) {
            if (_inner != nullptr) {
                ++_inner->_count;
            }
        }

        constexpr BasicRc(Self&& other) noexcept :
                _inner(other._inner) {
            if (_inner != nullptr) {
                ++_inner->_count;
            }
        }

        ~BasicRc() noexcept {
            drop();
        }

        constexpr auto operator =(const Self& other) noexcept -> Self& = delete;

        constexpr auto operator =(Self&& other) noexcept -> Self& = delete;

        constexpr auto reset(ElementType* pointer) noexcept -> void {
            if (pointer == nullptr) {
                return;
            }

            drop();
            _inner = ALLOCATOR<InnerType>().construct(utils::move_or_copy(*pointer));
        }

        constexpr auto drop() noexcept -> void {
            if (_inner == nullptr) {
                return;
            }

            if (_inner->_count > 0) {
                --_inner->_count;
            }

            if (_inner->_count == 0) {
                DELETER<InnerType>()(_inner);
            }

            _inner = nullptr; // Avoid dangling pointer
        }

        [[nodiscard]] constexpr auto get_count() const noexcept -> usize {
            if (_inner == nullptr) {
                return 0;
            }

            return static_cast<usize>(_inner->_count);
        }

        [[nodiscard]] constexpr auto operator *() noexcept -> T& {
            assert_true(_inner != nullptr);
            return _inner->_value;
        }

        [[nodiscard]] constexpr auto operator *() const noexcept -> const T& {
            assert_true(_inner != nullptr);
            return _inner->_value;
        }

        [[nodiscard]] constexpr auto operator ->() noexcept -> T* {
            assert_true(_inner != nullptr);
            return &_inner->_value;
        }

        [[nodiscard]] constexpr auto operator ->() const noexcept -> const T* {
            assert_true(_inner != nullptr);
            return &_inner->_value;
        }

        [[nodiscard]] constexpr auto operator ==(const Self& other) const noexcept -> bool {
            return _inner == other._inner;
        }

        [[nodiscard]] constexpr auto operator ==(decltype(nullptr)) const noexcept -> bool {
            return _inner == nullptr;
        }

        [[nodiscard]] constexpr auto operator !=(const Self& other) const noexcept -> bool {
            return _inner != other._inner;
        }

        [[nodiscard]] constexpr auto operator !=(decltype(nullptr)) const noexcept -> bool {
            return _inner != nullptr;
        }
    };

    template<typename T, // @formatter:off
        template<typename> typename ALLOCATOR = Allocator,
        template<typename> typename DELETER = Deleter> // @formatter:on
    using Rc = BasicRc<T, usize, ALLOCATOR, DELETER>;

    template<typename T, // @formatter:off
        template<typename> typename ALLOCATOR = Allocator,
        template<typename> typename DELETER = Deleter> // @formatter:on
    using Arc = BasicRc<T, Atomic<usize>, ALLOCATOR, DELETER>;

    template<typename T, // @formatter:off
        template<typename> typename ALLOCATOR = Allocator,
        template<typename> typename DELETER = Deleter,
        typename... ARGS>
    [[nodiscard]] constexpr auto make_rc(ARGS&& ... args) noexcept -> Rc<T, ALLOCATOR, DELETER> { // @formatter:on
        return Rc<T, ALLOCATOR, DELETER>(utils::forward<ARGS>(args)...);
    }

    template<typename T, // @formatter:off
        template<typename> typename ALLOCATOR = Allocator,
        template<typename> typename DELETER = Deleter,
        typename... ARGS>
    [[nodiscard]] constexpr auto make_arc(ARGS&& ... args) noexcept -> Arc<T, ALLOCATOR, DELETER> { // @formatter:on
        return Arc<T, ALLOCATOR, DELETER>(utils::forward<ARGS>(args)...);
    }
}