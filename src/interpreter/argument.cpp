/// @file argument.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Implements the argument class.
/// @copyright
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#include "interpreter/argument.hpp"

enum : unsigned char {
    FLAG_ALL      = (1U << 1U), ///< The `all.` prefix was specified.
    FLAG_QUANTITY = (1U << 2U), ///< The `<quantity>*` postfix was specified.
    FLAG_INDEX    = (1U << 3U)  ///< The `<index>.` postfix was specified.
};

namespace interpreter
{

Argument::Argument(const std::string &_original)
    : original(_original)
    , content(_original)
    , index(1)
    , quantity(1)
    , prefix(0)
{
    // Evaluate all the prefix.
    this->evaluate_all_prefix();
}

void Argument::parse(const std::string &_original)
{
    original = _original;
    content  = _original;
    index    = 1;
    quantity = 1;
    prefix   = 0;
    // Evaluate all the prefix.
    this->evaluate_all_prefix();
}

auto Argument::length() const -> size_t { return content.length(); }

auto Argument::empty() const -> bool { return content.empty(); }

auto Argument::get_original() const -> std::string { return original; }

auto Argument::get_content() const -> std::string { return content; }

void Argument::set_content(const std::string &_content) { content = _content; }

auto Argument::get_index() const -> std::size_t { return index; }

void Argument::set_index(std::size_t _index) { index = _index; }

auto Argument::get_quantity() const -> std::size_t { return quantity; }

void Argument::set_quantity(std::size_t _quantity) { quantity = _quantity; }

auto Argument::has_only_one_prefix() const -> bool
{
    return (static_cast<int>(this->has_prefix_all()) + static_cast<int>(this->has_quantity()) +
            static_cast<int>(this->has_index())) <= 1;
}

auto Argument::has_prefix_all() const -> bool { return (prefix & FLAG_ALL) == FLAG_ALL; }

auto Argument::has_quantity() const -> bool { return (prefix & FLAG_QUANTITY) == FLAG_QUANTITY; }

auto Argument::has_index() const -> bool { return (prefix & FLAG_INDEX) == FLAG_INDEX; }

auto Argument::means_all() const -> bool { return interpreter::config::means_all(original); }

auto Argument::is_abbreviation_of(const std::string &full_string, bool sensitive, std::size_t min_length) const -> bool
{
    return ustr::is_abbreviation_of(content, full_string, sensitive, min_length);
}

auto Argument::is_number() const -> bool { return ustr::is_number(original); }

auto Argument::operator==(const std::string &rhs) const -> bool { return content == rhs; }

auto Argument::operator[](std::size_t pos) const -> char { return content[pos]; }

auto Argument::operator[](std::size_t pos) -> char & { return content[pos]; }

auto operator<<(std::ostream &lhs, const Argument &rhs) -> std::ostream &
{
    lhs << rhs.content;
    return lhs;
}

void Argument::evaluate_all_prefix()
{
    // Get the position of index and quantity.
    std::string::size_type index_pos    = content.find_first_of(interpreter::config::list_of_symbols_index);
    std::string::size_type quantity_pos = content.find_first_of(interpreter::config::list_of_symbols_multiplier);
    // Evaluate which one comes first.
    if (index_pos < quantity_pos) {
        this->evaluate_index();
        this->evaluate_quantity();
    } else {
        this->evaluate_quantity();
        this->evaluate_index();
    }
}

void Argument::evaluate_index()
{
    // If the entire string is a number, skip it.
    if (ustr::is_number(content)) {
        return;
    }
    // Otherwise try to find a number if there is one.
    std::string::size_type pos = content.find_first_of(interpreter::config::list_of_symbols_index);
    if (pos == std::string::npos) {
        return;
    }
    // Extract the digits.
    std::string digits = content.substr(0, pos);
    // Check the digits.
    if (ustr::is_number(digits)) {
        // Get the number and set it.
        auto number = ustr::to_number<std::size_t>(digits);
        if (number < INT_MAX) {
            // Set the number.
            index = number;
            // Set the prefix flag.
            prefix |= FLAG_INDEX;
        }
        // Remove the digits.
        content = content.substr(pos + 1, content.size());
    } else {
        if (interpreter::config::means_all(digits)) {
            // Remove the quantity.
            content = content.substr(pos + 1, content.size());
            // Set the prefix flag.
            prefix |= FLAG_ALL;
        }
    }
}

void Argument::evaluate_quantity()
{
    // If the entire string is a number, skip it.
    if (ustr::is_number(content)) {
        return;
    }
    // Otherwise try to find a number if there is one.
    std::string::size_type pos = content.find_first_of(interpreter::config::list_of_symbols_multiplier);
    if (pos == std::string::npos) {
        return;
    }
    // Extract the digits.
    std::string digits = content.substr(0, pos);
    // Check the digits.
    if (ustr::is_number(digits)) {
        // Get the number and set it.
        auto number = ustr::to_number<std::size_t>(digits);
        if (number < INT_MAX) {
            // Set the number.
            quantity = number;
            // Set the prefix flag.
            prefix |= FLAG_QUANTITY;
        }
        // Remove the digits.
        content = content.substr(pos + 1, content.size());
    } else {
        if (interpreter::config::means_all(digits)) {
            // Remove the quantity.
            content = content.substr(pos + 1, content.size());
            // Set the prefix flag.
            prefix |= FLAG_ALL;
        }
    }
}

} // namespace interpreter
