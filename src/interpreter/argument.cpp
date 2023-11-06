/// @file argument.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-11-03
///
/// @copyright Copyright (c) 2023
///

#include "interpreter/argument.hpp"

/// @brief Sets the given bit.
/// @param value the value to modify.
/// @param bit the bit to manipulate.
/// @return the modified value.
template <typename T>
static inline T bitmask_set(T value, T bitmask)
{
    return value | bitmask;
}

/// @brief Clears the given bit.
/// @param value the value to modify.
/// @param bit the bit to manipulate.
/// @return the modified value.
template <typename T>
static inline T bitmask_clear(T value, T bitmask)
{
    return value & ~bitmask;
}

/// @brief Flips the given bit.
/// @param value the value to modify.
/// @param bit the bit to manipulate.
/// @return the modified value.
template <typename T>
static inline T bitmask_flip(T value, T bitmask)
{
    return value ^ bitmask;
}

/// @brief Checks if the given bit is 1.
/// @param value the value to modify.
/// @param bit the bit to check.
/// @return true if the bit is 1, false otherwise.
template <typename T>
static inline bool bitmask_check(T value, T bitmask)
{
    return value & bitmask;
}

#define FLAG_ALL      0u ///< The 'all.' prefix was specified.
#define FLAG_QUANTITY 1u ///< The '<quantity>*' postfix was specified.
#define FLAG_INDEX    2u ///< The '<index>.' postfix was specified.

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
    // First, evaluate the quantity.
    this->evaluate_quantity();
    // Then, evaluate the index.
    this->evaluate_index();
}

void Argument::parse(const std::string &_original)
{
    original = _original;
    content  = _original;
    index    = 1;
    quantity = 1;
    prefix   = 0;
    // First, evaluate the quantity.
    this->evaluate_quantity();
    // Then, evaluate the index.
    this->evaluate_index();
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

std::size_t Argument::get_index() const
{
    return index;
}

std::size_t Argument::get_quantity() const
{
    return quantity;
}

bool Argument::has_only_one_prefix() const
{
    if (!bitmask_check(prefix, FLAG_ALL | FLAG_QUANTITY | FLAG_INDEX)) {
        return false;
    }
    if (!bitmask_check(prefix, FLAG_ALL) && !bitmask_check(prefix, FLAG_QUANTITY) && !bitmask_check(prefix, FLAG_INDEX)) {
        return false;
    }
    return true;
}

bool Argument::has_prefix_all() const
{
    return bitmask_check(prefix, FLAG_ALL);
}

bool Argument::has_quantity() const
{
    return bitmask_check(prefix, FLAG_QUANTITY);
}

bool Argument::has_index() const
{
    return bitmask_check(prefix, FLAG_INDEX);
}

bool Argument::means_all() const
{
    return interpreter::config::means_all(original);
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
            prefix = bitmask_set(prefix, FLAG_INDEX);
        }
        // Remove the digits.
        content = content.substr(pos + 1, content.size());
    } else {
        if (interpreter::config::means_all(digits)) {
            // Remove the quantity.
            content = content.substr(pos + 1, content.size());
            // Set the prefix flag.
            prefix = bitmask_set(prefix, FLAG_ALL);
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
            prefix = bitmask_set(prefix, FLAG_QUANTITY);
        }
        // Remove the digits.
        content = content.substr(pos + 1, content.size());
    } else {
        if (interpreter::config::means_all(digits)) {
            // Remove the quantity.
            content = content.substr(pos + 1, content.size());
            // Set the prefix flag.
            prefix = bitmask_set(prefix, FLAG_ALL);
        }
    }
}

} // namespace interpreter
