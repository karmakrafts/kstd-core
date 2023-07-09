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
 * @since 09/07/2023
 */

#pragma once

#include "assert.hpp"
#include "result.hpp"
#include <exception>
#include <memory>
#include <utility>

namespace kstd {
    template<typename T, typename... ARGS>
    [[nodiscard]] constexpr auto try_construct(ARGS&&... args) noexcept -> Result<T> {
        try {
            return T(std::forward<ARGS>(args)...);
        }
        catch(const std::exception& error) {
            return Error(std::string_view(error.what()));
        }
    }

    template<typename T, typename... ARGS>
    [[nodiscard]] constexpr auto try_new(ARGS&&... args) noexcept -> Result<T*> {
        try {
            auto* address = new T(std::forward<ARGS>(args)...);
            if(address == nullptr) {
                return {};// Return empty result instead of nullptr
            }
            return address;
        }
        catch(const std::exception& error) {
            return Error(std::string_view(error.what()));
        }
    }

    template<typename T, typename... ARGS>
    [[nodiscard]] constexpr auto try_make_unique(ARGS&&... args) noexcept -> Result<std::unique_ptr<T>> {
        try {
            return {std::make_unique<T>(std::forward<ARGS>(args)...)};
        }
        catch(const std::exception& error) {
            return Error(std::string_view(error.what()));
        }
    }

    template<typename T, typename... ARGS>
    [[nodiscard]] constexpr auto try_make_shared(ARGS&&... args) noexcept -> Result<std::shared_ptr<T>> {
        try {
            return std::make_shared<T>(std::forward<ARGS>(args)...);
        }
        catch(const std::exception& error) {
            return Error(std::string_view(error.what()));
        }
    }
}// namespace kstd