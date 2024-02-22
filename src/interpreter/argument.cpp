/// @file argument.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-11-03
///
/// @copyright Copyright (c) 2023
///

#include "interpreter/argument.hpp"

#define FLAG_ALL      (1u << 1u) ///< The 'all.' prefix was specified.
#define FLAG_QUANTITY (1u << 2u) ///< The '<quantity>*' postfix was specified.
#define FLAG_INDEX    (1u << 3u) ///< The '<index>.' postfix was specified.

namespace interpreter
{

namespace config
{

std::vector<std::string> list_of_all = {
    "all"
};
std::vector<std::string> list_of_ingnore = {
    "in", "from", "with", "and", "the", "on", "at", "to", "a", "an"
};
std::string list_of_symbols_multiplier = "*";
std::string list_of_symbols_index      = ".";

bool means_all(const std::string &word)
{
    return ustr::word_is_among(word, interpreter::config::list_of_all, false, false, false, true);
}

bool must_ignore(const std::string &word)
{
    return ustr::word_is_among(word, interpreter::config::list_of_ingnore, false, false, false, true);
}

} // namespace config

Argument::Argument(const std::string &_original)
    : original(_original), content(_original), index(1), quantity(1),
      prefix(0)
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

size_t Argument::length() const
{
    return content.length();
}

bool Argument::empty() const
{
    return content.empty();
}

std::string Argument::get_original() const
{
    return original;
}

std::string Argument::get_content() const
{
    return content;
}

void Argument::set_content(const std::string &_content)
{
    content = _content;
}

std::size_t Argument::get_index() const
{
    return index;
}

void Argument::set_index(std::size_t _index)
{
    index = _index;
}

unsigned Argument::map_to_option(const std::vector<Option> &options, bool sensitive, bool begins_with, bool ends_with, bool exact_match) const
{
    std::vector<Option>::const_iterator it;
    for (it = options.begin(); it != options.end(); ++it) {
        if (ustr::word_is_among(content, it->names, sensitive, begins_with, ends_with, exact_match)) {
            return it->option;
        }
    }
    return 0;
}

std::size_t Argument::get_quantity() const
{
    return quantity;
}

void Argument::set_quantity(std::size_t _quantity)
{
    quantity = _quantity;
}

bool Argument::has_only_one_prefix() const
{
    if ((this->has_prefix_all() + this->has_quantity() + this->has_index()) > 1) {
        return false;
    }
    return true;
}

bool Argument::has_prefix_all() const
{
    return (prefix & FLAG_ALL) == FLAG_ALL;
}

bool Argument::has_quantity() const
{
    return (prefix & FLAG_QUANTITY) == FLAG_QUANTITY;
}

bool Argument::has_index() const
{
    return (prefix & FLAG_INDEX) == FLAG_INDEX;
}

bool Argument::means_all() const
{
    return interpreter::config::means_all(original);
}

bool Argument::is_abbreviation_of(const std::string &full_string, bool sensitive, std::size_t min_length) const
{
    return ustr::is_abbreviation_of(content, full_string, sensitive, min_length);
}

bool Argument::is_number() const
{
    return ustr::is_number(original);
}

bool Argument::operator==(const std::string &rhs) const
{
    return content == rhs;
}

char Argument::operator[](std::size_t pos) const
{
    return content[pos];
}

char &Argument::operator[](std::size_t pos)
{
    return content[pos];
}

std::ostream &operator<<(std::ostream &lhs, const Argument &rhs)
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
        std::size_t number = ustr::to_number<std::size_t>(digits);
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
        std::size_t number = ustr::to_number<std::size_t>(digits);
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
