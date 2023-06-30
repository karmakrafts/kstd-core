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
 * @since 16/05/2023
 */

#pragma once

#include <array>

#include "libc.hpp"
#include "source_location.hpp"

namespace kstd {
    namespace {
        constexpr usize BUFFER_SIZE = 1024;

        class AssertionMessage final {
            char* _data;

            public:
            explicit inline AssertionMessage(const char* data) noexcept :
                    _data() {
                const auto size = libc::strlen(data) + 1;
                _data = static_cast<char*>(libc::malloc(size));// NOLINT
                libc::memset(_data, 0, size);
                libc::strncpy(_data, data, size);// Copy data to heap memory
            }

            inline AssertionMessage(const AssertionMessage& other) noexcept :
                    AssertionMessage(other._data) {
            }

            inline AssertionMessage(AssertionMessage&& other) noexcept :
                    AssertionMessage(other._data) {
            }

            inline auto operator=(const AssertionMessage& other) noexcept -> AssertionMessage& = delete;

            inline auto operator=(AssertionMessage&& other) noexcept -> AssertionMessage& = delete;

            ~AssertionMessage() noexcept {
                libc::free(_data);// NOLINT: Free heap memory when object is destroyed
            }

            [[nodiscard]] constexpr auto get_data() const noexcept -> const char* {
                return _data;
            }
        };

        [[nodiscard]] inline auto
        get_default_assertion_message(const SourceLocation& location = current_location()) noexcept
                -> AssertionMessage {
            std::array<char, BUFFER_SIZE> buffer {};
            auto* buffer_data = buffer.data();
            libc::sprintf(buffer_data, "%s:%zu [%s]", location.get_file(), location.get_line(),
                          location.get_function());
            return AssertionMessage(buffer_data);
        }
    }// namespace

    inline auto assert_true(bool condition, const char* message = get_default_assertion_message().get_data()) noexcept
            -> void {
#ifdef BUILD_DEBUG
        if(!condition) {
            libc::fprintf(libc::iob::err, "Assertion failed in %s\n", message);
            libc::exit(4);
        }
#endif
    }

    inline auto assert_false(bool condition, const char* message = get_default_assertion_message().get_data()) noexcept
            -> void {
#ifdef BUILD_DEBUG
        if(condition) {
            libc::fprintf(libc::iob::err, "Assertion failed in %s\n", message);
            libc::exit(4);
        }
#endif
    }
}// namespace kstd