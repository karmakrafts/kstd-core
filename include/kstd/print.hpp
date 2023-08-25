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
 * @since 12/08/2023
 */

#pragma once

#include <fmt/format.h>
#include <string>
#include <type_traits>

#include "libc.hpp"

namespace kstd {
    template<typename T>
    constexpr auto print(const T& value) noexcept {
        if constexpr(std::is_same_v<std::remove_const_t<std::remove_all_extents_t<T>>, std::string>) {
            libc::fprintf(libc::iob::out, "%s", value.c_str());
        }
        else if constexpr(std::is_same_v<T, const char*> || std::is_same_v<T, char*>) {
            libc::fprintf(libc::iob::out, "%s", value);
        }
        else {
            libc::fprintf(libc::iob::out, "%s", std::to_string(value));
        }
    }

    template<typename HEAD, typename... TAIL>
    constexpr auto print(fmt::format_string<HEAD, TAIL...> fmt, HEAD&& head, TAIL&&... tail) noexcept {
        const auto formatted = fmt::format(std::move(fmt), std::forward<HEAD>(head), std::forward<TAIL>(tail)...);
        libc::fprintf(libc::iob::out, "%s", formatted.c_str());
    }

    template<typename T>
    constexpr auto println(const T& value) noexcept {
        if constexpr(std::is_same_v<std::remove_const_t<std::remove_all_extents_t<T>>, std::string>) {
            libc::fprintf(libc::iob::out, "%s\n", value.c_str());
        }
        else if constexpr(std::is_same_v<T, const char*> || std::is_same_v<T, char*>) {
            libc::fprintf(libc::iob::out, "%s\n", value);
        }
        else {
            libc::fprintf(libc::iob::out, "%s\n", std::to_string(value));
        }
    }

    template<typename HEAD, typename... TAIL>
    constexpr auto println(fmt::format_string<HEAD, TAIL...> fmt, HEAD&& head, TAIL&&... tail) noexcept {
        const auto formatted = fmt::format(std::move(fmt), std::forward<HEAD>(head), std::forward<TAIL>(tail)...);
        libc::fprintf(libc::iob::out, "%s\n", formatted.c_str());
    }

    template<typename T>
    constexpr auto print_error(const T& value) noexcept {
        if constexpr(std::is_same_v<std::remove_const_t<std::remove_all_extents_t<T>>, std::string>) {
            libc::fprintf(libc::iob::err, "%s", value.c_str());
        }
        else if constexpr(std::is_same_v<T, const char*> || std::is_same_v<T, char*>) {
            libc::fprintf(libc::iob::err, "%s", value);
        }
        else {
            libc::fprintf(libc::iob::err, "%s", std::to_string(value));
        }
    }

    template<typename HEAD, typename... TAIL>
    constexpr auto print_error(fmt::format_string<HEAD, TAIL...> fmt, HEAD&& head, TAIL&&... tail) noexcept {
        const auto formatted = fmt::format(std::move(fmt), std::forward<HEAD>(head), std::forward<TAIL>(tail)...);
        libc::fprintf(libc::iob::err, "%s", formatted.c_str());
    }

    template<typename T>
    constexpr auto println_error(T value) noexcept {
        if constexpr(std::is_same_v<std::remove_const_t<std::remove_all_extents_t<T>>, std::string>) {
            libc::fprintf(libc::iob::err, "%s\n", value.c_str());
        }
        else if constexpr(std::is_same_v<T, const char*> || std::is_same_v<T, char*>) {
            libc::fprintf(libc::iob::err, "%s\n", value);
        }
        else {
            libc::fprintf(libc::iob::err, "%s\n", std::to_string(value));
        }
    }

    template<typename HEAD, typename... TAIL>
    constexpr auto println_error(fmt::format_string<HEAD, TAIL...> fmt, HEAD&& head, TAIL&&... tail) noexcept {
        const auto formatted = fmt::format(std::move(fmt), std::forward<HEAD>(head), std::forward<TAIL>(tail)...);
        libc::fprintf(libc::iob::err, "%s\n", formatted.c_str());
    }
}// namespace kstd