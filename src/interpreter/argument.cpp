/// @file argument.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-11-03
///
/// @copyright Copyright (c) 2023
///

#include "interpreter/argument.hpp"

namespace interpreter
{

namespace config
{

std::vector<std::string> all = {
    "all"
};

} // namespace config

Argument::Argument(const std::string &_original)
    : original(_original), content(_original), index(1), quantity(1),
      prefix(false, false, false)
{
    // First, evaluate the quantity.
    this->evaluateQuantity();
    // Then, evaluate the index.
    this->evaluateIndex();
}

void Argument::parse(const std::string &_original)
{
    original = _original, content = _original, index = 1, quantity = 1;
    prefix = prefix_t(false, false, false);
    // First, evaluate the quantity.
    this->evaluateQuantity();
    // Then, evaluate the index.
    this->evaluateIndex();
}

size_t Argument::length() const
{
    return content.length();
}

bool Argument::empty() const
{
    return content.empty();
}

std::string Argument::getOriginal() const
{
    return original;
}

std::string Argument::getContent() const
{
    return content;
}

std::size_t Argument::getIndex() const
{
    return index;
}

std::size_t Argument::getQuantity() const
{
    return quantity;
}

bool Argument::hasOnlyOnePrefix() const
{
    return ((prefix.all + prefix.quantity + prefix.index) == 1) ||
           !(prefix.all & prefix.quantity & prefix.index);
}

bool Argument::hasPrefixAll() const
{
    return prefix.all;
}

bool Argument::hasQuantity() const
{
    return prefix.quantity;
}

bool Argument::hasIndex() const
{
    return prefix.index;
}

bool Argument::meansAll() const
{
    return ustr::word_is_among(original, config::all, false, false, false, true);
}

void Argument::setString(std::string const &s)
{
    original = content = s;
    // First, evaluate the quantity.
    this->evaluateQuantity();
    // Then, evaluate the index.
    this->evaluateIndex();
}

bool Argument::operator==(const std::string &rhs) const
{
    return content == rhs;
}

char Argument::operator[](size_t rhs) const
{
    return content[rhs];
}

char &Argument::operator[](size_t rhs)
{
    return content[rhs];
}

std::ostream &operator<<(std::ostream &lhs, const Argument &rhs)
{
    lhs << rhs.content;
    return lhs;
}

void Argument::evaluateIndex()
{
    // If the entire string is a number, skip it.
    if (ustr::is_number(content)) {
        return;
    }
    // Otherwise try to find a number if there is one.
    std::string::size_type pos = content.find('.');
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
        }
        // Set the prefix flag.
        prefix.index = true;
        // Remove the digits.
        content = content.substr(pos + 1, content.size());
    } else {
        std::string lower_digits = ustr::to_lower(digits);
        if (ustr::word_is_among(lower_digits, config::all, false, false, false, true)) {
            // Set the prefix flag.
            prefix.all = true;
            // Remove the quantity.
            content = content.substr(pos + 1, content.size());
        }
    }
}

void Argument::evaluateQuantity()
{
    // If the entire string is a number, skip it.
    if (ustr::is_number(content)) {
        return;
    }
    // Otherwise try to find a number if there is one.
    std::string::size_type pos = content.find('*');
    if (pos == std::string::npos) {
        return;
    }
    // Extract the digits.
    std::string digits = content.substr(0, pos);
    // Check the digits.
    if (ustr::is_number(digits)) {
        // Set the prefix flag.
        prefix.quantity = true;
        // Get the number and set it.
        std::size_t number = ustr::to_number<std::size_t>(digits);
        if (number < INT_MAX) {
            quantity = number;
        }
        // Remove the digits.
        content = content.substr(pos + 1, content.size());
    } else {
        std::string lower_digits = ustr::to_lower(digits);
        if ((lower_digits == "all") || (lower_digits == "tutto")) {
            // Set the prefix flag.
            prefix.all = true;
            // Remove the quantity.
            content = content.substr(pos + 1, content.size());
        }
    }
}

} // namespace interpreter
