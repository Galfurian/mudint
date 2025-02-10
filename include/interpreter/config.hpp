/// @file config.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Contains the configuration variables and functions.
/// @copyright
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#pragma once

#include <string>
#include <vector>

namespace interpreter
{

/// @brief MUD interpreter configuration.
namespace config
{

/// @brief The list of words meaning "all".
extern std::vector<std::string> list_of_all;
/// @brief The list of words to ingnore.
extern std::vector<std::string> list_of_ingnore;
/// @brief The list of symbols for specifying a quantity.
extern std::string list_of_symbols_multiplier;
/// @brief The list of symbols for specifying an index.
extern std::string list_of_symbols_index;

/// @brief Checks if the given word means all.
/// @param word the word to check.
/// @return true if it means all, false otherwise.
auto means_all(const std::string &word) -> bool;

/// @brief Checks if the given word must be ignored.
/// @param word the word to check.
/// @return true if it must be ignored, false otherwise.
auto must_ignore(const std::string &word) -> bool;

} // namespace config

} // namespace interpreter
