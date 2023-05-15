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

namespace kstd {
    namespace {
        template<typename T, typename COUNTER_TYPE>
        struct RcInner final {
            T* value;
            COUNTER_TYPE count;

            constexpr RcInner() noexcept :
                    value(),
                    count() {
            }

            ~RcInner() noexcept = default;
        };
    }

    template<typename T, typename COUNTER_TYPE, typename ALLOCATOR> //
    struct BasicRc final {
        using ElementType = T;
        using CounterType = COUNTER_TYPE;
        using Allocator = ALLOCATOR;
        using InnerType = RcInner<ElementType, CounterType>;
        using Self = BasicRc<ElementType, CounterType, Allocator>;
        using Pointer = ElementType*;
        using ConstPointer = const ElementType*;
        using Reference = ElementType&;
        using ConstReference = const ElementType&;
        using SizeType = usize;

        private:

        InnerType* _inner;

        public:

        constexpr BasicRc() noexcept :
                _inner() {
        }

        explicit constexpr BasicRc(Pointer value) noexcept :
                _inner(new InnerType()) {
            _inner->value = value;
            _inner->count = 1;
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
            drop();
        }

        constexpr auto drop() noexcept -> void {
            if (_inner != nullptr) {
                if (_inner->count > 0) {
                    --_inner->count;
                }
                else {
                    Allocator allocator;
                    allocator.destroy(_inner->value);
                    delete _inner;
                }
            }
        }

        constexpr auto reset(Pointer pointer) noexcept -> void {
            drop();

            if (_inner == nullptr) {
                _inner = new InnerType();
            }

            _inner->value = pointer;
            _inner->count = 1;
        }

        constexpr auto operator =(const Self& other) noexcept -> Self& {
            if (this == &other) {
                return *this;
            }

            reset(*other);
            ++_inner->count;
            return *this;
        }

        constexpr auto operator =(Self&& other) noexcept -> Self& {
            reset(*other);
            return *this;
        }

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool {
            return _inner != nullptr;
        }

        [[nodiscard]] constexpr auto get_count() const noexcept -> SizeType {
            if (_inner == nullptr) {
                return 0;
            }

            return static_cast<SizeType>(_inner->count);
        }

        [[nodiscard]] constexpr operator bool() const noexcept { // NOLINT
            return has_value();
        }

        [[nodiscard]] constexpr auto operator *() noexcept -> Reference {
            return *_inner->value;
        }

        [[nodiscard]] constexpr auto operator *() const noexcept -> ConstReference {
            return *_inner->value;
        }

        [[nodiscard]] constexpr auto operator ->() noexcept -> Pointer {
            return _inner->value;
        }

        [[nodiscard]] constexpr auto operator ->() const noexcept -> ConstPointer {
            return _inner->value;
        }
    };

    template<typename T, typename ALLOCATOR = Allocator<T>> //
    using Rc = BasicRc<T, usize, ALLOCATOR>;

    template<typename T, typename ALLOCATOR = Allocator<T>> //
    using Arc = BasicRc<T, Atomic<usize>, ALLOCATOR>;

    template<typename T, typename ALLOCATOR = Allocator<T>, typename... ARGS>
    [[nodiscard]] constexpr auto make_rc(ARGS&& ... args) noexcept -> Rc<T> {
        ALLOCATOR allocator;
        return Rc<T, ALLOCATOR>(allocator.construct(forward<ARGS>(args)...));
    }

    template<typename T, typename ALLOCATOR = Allocator<T>, typename... ARGS>
    [[nodiscard]] constexpr auto make_arc(ARGS&& ... args) noexcept -> Arc<T> {
        ALLOCATOR allocator;
        return Arc<T, ALLOCATOR>(allocator.construct(forward<ARGS>(args)...));
    }
}