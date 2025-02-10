/// @file argument.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Definition of argument.
/// @copyright
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#pragma once

#include <climits>
#include <iostream>

#include <ustr/check.hpp>
#include <ustr/manipulate.hpp>
#include <ustr/utility.hpp>

#include "config.hpp"

namespace interpreter
{

/// @brief Represents a selectable option with associated names.
///
/// @details Each option has a unique identifier and a list of names (aliases)
/// that can be used to reference it. This structure is used to map input
/// arguments to their corresponding options based on string comparisons.
struct Option {
    unsigned option;                ///< Unique identifier for the option.
    std::vector<std::string> names; ///< List of names or aliases for the option.
};

/// @brief Allows to easily manage input arguments from players.
class Argument
{
private:
    /// The original argument string.
    std::string original;
    /// The string with both the index and the quantity removed.
    std::string content;
    /// The provided index.
    std::size_t index;
    /// The provided quantity.
    std::size_t quantity;
    /// Struct containing all the flags associated with this argument.
    unsigned prefix;

public:
    /// @brief Constructor.
    /// @param _original the orginal content of the argument.
    explicit Argument(const std::string &_original);

    /// @brief Parse the given string and store the details inside the argument.
    /// @param _original the orginal content of the argument.
    void parse(const std::string &_original);

    /// @brief The length of the `content` not the `original` string.
    /// @return the length.
    auto length() const -> std::size_t;

    /// @brief Check if the `content`, not the `original` string, is empty.
    /// @return true if the content is empty, false otherwise.
    auto empty() const -> bool;

    /// @brief Provides the original argument.
    /// @return the original string.
    auto get_original() const -> std::string;

    /// @brief Provides the `content` with both index and quantity removed.
    /// @return the cleaned content.
    auto get_content() const -> std::string;

    /// @brief Forces the content to a given string.
    /// @param _content the new value.
    void set_content(const std::string &_content);

    /// @brief Provides the index extracted from the `original`.
    /// @return the extracted index.
    auto get_index() const -> std::size_t;

    /// @brief Forces a new index.
    /// @param _index the new value.
    void set_index(std::size_t _index);

    /// @brief Checks if the argument maps to one of the options.
    /// @param options the list of options.
    /// @param function binary comparison function.
    /// @return true if at least one of the conditions are true.
    template <typename Fun>
    auto map_to_option(const std::vector<Option> &options, Fun function) const -> unsigned
    {
        std::vector<Option>::const_iterator option_it;
        std::vector<std::string>::const_iterator name_it;
        for (option_it = options.begin(); option_it != options.end(); ++option_it) {
            for (name_it = option_it->names.begin(); name_it != option_it->names.end(); ++name_it) {
                if (function(content, *name_it)) {
                    return option_it->option;
                }
            }
        }
        return 0;
    }

    /// @brief Provides the quantity extracted from the `original`.
    /// @return the extracted quantity.
    auto get_quantity() const -> std::size_t;

    /// @brief Forces a new quantity.
    /// @param _quantity the new value.
    void set_quantity(std::size_t _quantity);

    /// @brief Checks if there is only one prefix, or there is no prefix.
    /// @return true if only one prefix is provided, false otherwise.
    auto has_only_one_prefix() const -> bool;

    /// @brief Checks if the prefix means `all`.
    /// @return true if the prefix means `all`, false otherwise.
    auto has_prefix_all() const -> bool;

    /// @brief Checks if the prefix is a quantity.
    /// @return true if the prefix is a quantity, false otherwise.
    auto has_quantity() const -> bool;

    /// @brief Checks if the prefix is an index.
    /// @return true if the prefix is an index, false otherwise.
    auto has_index() const -> bool;

    /// @brief Checks if the whole argument means `all`.
    /// @return true if the whole argument means `all`, false otherwise.
    auto means_all() const -> bool;

    /// @brief Checks if the argument is an abbreviation of the given full string.
    /// @param full_string the full string.
    /// @param sensitive enables case-sensitive check.
    /// @param min_length the minimum number of characters for the prefix.
    /// @return true if the argument is an abbreviation equally long or longher than min_length, false otherwise.
    auto is_abbreviation_of(const std::string &full_string, bool sensitive = false, std::size_t min_length = 1) const
        -> bool;

    /// @brief Checks if the `original` content was actually a number.
    /// @return true if it is a number, false otherwise.
    auto is_number() const -> bool;

    /// @brief Transforms the `original` content into an integer value.
    /// @return the number.
    template <typename T>
    auto to_number() const -> T
    {
        return ustr::to_number<T>(original);
    }

    /// @brief Check if the `content`, not the `original` string, is equal to a given string.
    /// @param rhs the string to check.
    /// @return true if they are equal, false otherwise.
    auto operator==(const std::string &rhs) const -> bool;

    /// @brief Access the character at the given position.
    /// @param pos the position.
    /// @return the character at the given position.
    auto operator[](std::size_t pos) const -> char;

    /// @brief Access the character at the given position.
    /// @param pos the position.
    /// @return the character at the given position.
    auto operator[](std::size_t pos) -> char &;

    /// @brief Sends to the output stream the argument.
    /// @param lhs the stream.
    /// @param rhs the argument.
    /// @return the stream.
    friend auto operator<<(std::ostream &lhs, const Argument &rhs) -> std::ostream &;

private:
    /// Evaluates the index and quantity.
    void evaluate_all_prefix();

    /// Evaluates the index.
    void evaluate_index();

    /// Evaluates the quantity.
    void evaluate_quantity();
};

} // namespace interpreter
