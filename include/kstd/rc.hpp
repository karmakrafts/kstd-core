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
            T value;
            COUNTER_TYPE count;

            template<typename... ARGS>
            explicit constexpr RcInner(ARGS&& ... args) noexcept :
                    value(forward<ARGS>(args)...),
                    count() {
            }

            ~RcInner() noexcept = default;
        };

        template<typename T, typename COUNTER_TYPE>
        struct NullableRcInner final {
            T* value;
            COUNTER_TYPE count;

            explicit constexpr NullableRcInner(T* pointer) noexcept :
                    value(pointer),
                    count() {
            }

            ~NullableRcInner() noexcept = default;
        };
    }

    template<typename T, typename COUNTER_TYPE, template<typename> typename ALLOCATOR = Allocator> //
    struct BasicRc final {
        using ElementType = T;
        using CounterType = COUNTER_TYPE;
        using InnerType = RcInner<ElementType, CounterType>;
        using Self = BasicRc<ElementType, CounterType, ALLOCATOR>;

        private:

        InnerType* _inner;

        public:

        template<typename... ARGS>
        explicit constexpr BasicRc(ARGS&& ... args) noexcept :
                _inner(ALLOCATOR<InnerType>().construct(forward<ARGS>(args)...)) {
        }

        constexpr BasicRc(const Self& other) noexcept :
                _inner(other._inner) {
            ++_inner->count;
        }

        constexpr BasicRc(Self&& other) noexcept :
                _inner(other._inner) {
            // Count stays the same
        }

        ~BasicRc() noexcept {
            if (_inner->count > 0) {
                --_inner->count;
            }
            else {
                ALLOCATOR<InnerType>().destroy(_inner);
                _inner = nullptr; // Avoid dangling pointer
            }
        }

        constexpr auto operator =(const Self& other) noexcept -> Self& = delete;

        constexpr auto operator =(Self&& other) noexcept -> Self& = delete;

        [[nodiscard]] constexpr auto get_count() const noexcept -> usize {
            assert_true(_inner != nullptr);
            return static_cast<usize>(_inner->count);
        }

        [[nodiscard]] constexpr auto operator *() noexcept -> T& {
            assert_true(_inner != nullptr);
            return _inner->value;
        }

        [[nodiscard]] constexpr auto operator *() const noexcept -> const T& {
            assert_true(_inner != nullptr);
            return _inner->value;
        }

        [[nodiscard]] constexpr auto operator ->() noexcept -> T* {
            assert_true(_inner != nullptr);
            return &_inner->value;
        }

        [[nodiscard]] constexpr auto operator ->() const noexcept -> const T* {
            assert_true(_inner != nullptr);
            return &_inner->value;
        }

        [[nodiscard]] constexpr auto operator ==(const Self& other) const noexcept -> bool {
            return _inner == other._inner;
        }

        [[nodiscard]] constexpr auto operator !=(const Self& other) const noexcept -> bool {
            return _inner != other._inner;
        }
    };

    template<typename T, typename COUNTER_TYPE, template<typename> typename ALLOCATOR = Allocator, typename DELETER = Deleter<T, ALLOCATOR<T>>> //
    struct BasicNullableRc final {
        using ElementType = T;
        using CounterType = COUNTER_TYPE;
        using InnerType = NullableRcInner<ElementType, CounterType>;
        using Allocator = ALLOCATOR<ElementType>;
        using Deleter = DELETER;
        using Self = BasicNullableRc<ElementType, CounterType, ALLOCATOR>;

        private:

        InnerType* _inner;

        public:

        constexpr BasicNullableRc() noexcept :
                _inner(nullptr) {
        }

        explicit constexpr BasicNullableRc(ElementType* pointer) noexcept :
                _inner(pointer) {
        }

        constexpr BasicNullableRc(const Self& other) noexcept :
                _inner(other._inner) {
            ++_inner->count;
        }

        constexpr BasicNullableRc(Self&& other) noexcept :
                _inner(other._inner) {
            // Count stays the same
        }

        ~BasicNullableRc() noexcept {
            drop();
        }

        constexpr auto operator =(const Self& other) noexcept -> Self& = delete;

        constexpr auto operator =(Self&& other) noexcept -> Self& = delete;

        constexpr auto drop() noexcept -> void {
            if (_inner != nullptr) {
                if (_inner->count > 0) {
                    --_inner->count;
                }
                else {
                    Deleter()(_inner->value);
                    ALLOCATOR<InnerType>().destroy(_inner);
                    _inner = nullptr; // Avoid dangling pointer
                }
            }
        }

        [[nodiscard]] constexpr auto get_count() const noexcept -> usize {
            if (_inner == nullptr) {
                return 0;
            }

            return static_cast<usize>(_inner->count);
        }

        [[nodiscard]] constexpr auto operator *() noexcept -> T& {
            assert_true(_inner != nullptr);
            return _inner->value;
        }

        [[nodiscard]] constexpr auto operator *() const noexcept -> const T& {
            assert_true(_inner != nullptr);
            return _inner->value;
        }

        [[nodiscard]] constexpr auto operator ->() noexcept -> T* {
            assert_true(_inner != nullptr);
            return &_inner->value;
        }

        [[nodiscard]] constexpr auto operator ->() const noexcept -> const T* {
            assert_true(_inner != nullptr);
            return &_inner->value;
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

    template<typename T, template<typename> typename ALLOCATOR = Allocator> //
    using Rc = BasicRc<T, usize, ALLOCATOR>;

    template<typename T, template<typename> typename ALLOCATOR = Allocator> //
    using NullableRc = BasicNullableRc<T, usize, ALLOCATOR>;

    template<typename T, template<typename> typename ALLOCATOR = Allocator> //
    using Arc = BasicRc<T, Atomic<usize>, ALLOCATOR>;

    template<typename T, template<typename> typename ALLOCATOR = Allocator> //
    using NullableArc = BasicNullableRc<T, Atomic<usize>, ALLOCATOR>;

    template<typename T, template<typename> typename ALLOCATOR = Allocator, typename... ARGS>
    [[nodiscard]] constexpr auto make_rc(ARGS&& ... args) noexcept -> Rc<T, ALLOCATOR> {
        return Rc<T, ALLOCATOR>(forward<ARGS>(args)...);
    }

    template<typename T, template<typename> typename ALLOCATOR = Allocator, typename... ARGS>
    [[nodiscard]] constexpr auto make_nullable_rc(ARGS&& ... args) noexcept -> NullableRc<T, ALLOCATOR> {
        return NullableRc<T, ALLOCATOR>(ALLOCATOR<T>().construct(forward<ARGS>(args)...));
    }

    template<typename T, template<typename> typename ALLOCATOR = Allocator, typename... ARGS>
    [[nodiscard]] constexpr auto make_arc(ARGS&& ... args) noexcept -> Arc<T, ALLOCATOR> {
        return Arc<T, ALLOCATOR>(forward<ARGS>(args)...);
    }

    template<typename T, template<typename> typename ALLOCATOR = Allocator, typename... ARGS>
    [[nodiscard]] constexpr auto make_nullable_arc(ARGS&& ... args) noexcept -> NullableArc<T, ALLOCATOR> {
        return NullableArc<T, ALLOCATOR>(ALLOCATOR<T>().construct(forward<ARGS>(args)...));
    }
}