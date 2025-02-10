/// @file config.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Contains the configuration variables and functions.
/// @copyright
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#include "interpreter/config.hpp"

#include <ustr/check.hpp>

namespace interpreter
{

namespace config
{

std::vector<std::string> list_of_all     = {"all"};
std::vector<std::string> list_of_ingnore = {"in", "from", "with", "and", "the", "on", "at", "to", "a", "an"};
std::string list_of_symbols_multiplier   = "*";
std::string list_of_symbols_index        = ".";

bool means_all(const std::string &word)
{
    return ustr::word_is_among(word, interpreter::config::list_of_all, false, false, false, true);
}

bool must_ignore(const std::string &word)
{
    return ustr::word_is_among(word, interpreter::config::list_of_ingnore, false, false, false, true);
}

} // namespace config

} // namespace interpreter
