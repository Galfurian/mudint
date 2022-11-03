/// @file argument.hpp

#pragma once

#include <iostream>

#include "ustr.hpp"

namespace interpreter
{

/// @brief Allows to easily manage input arguments from players.
class Argument {
private:
    /// The original argument string.
    std::string original;
    /// The string with both the index and the quantity removed.
    std::string content;
    /// The provided index.
    int index;
    /// The provided quantity.
    int quantity;
    /// Struct containing all the flags associated with this argument.
    struct prefix_t {
        /// The 'all.' prefix was specified.
        bool all;
        /// The '<quantity>*' postfix was specified.
        bool quantity;
        /// The '<index>.' postfix was specified.
        bool index;
        /// @brief Constructor.
        prefix_t(bool _all, bool _quantity, bool _index)
            : all(_all), quantity(_quantity), index(_index)
        {
        }
    } prefix;

public:
    /// @brief Constructor.
    explicit Argument(const std::string &_original)
        : original(_original), content(_original), index(1), quantity(1),
          prefix(false, false, false)
    {
        // First, evaluate the quantity.
        this->evaluateQuantity();
        // Then, evaluate the index.
        this->evaluateIndex();
    }

    inline void parse(const std::string &_original)
    {
        original = _original, content = _original, index = 1, quantity = 1;
        prefix = prefix_t(false, false, false);
        // First, evaluate the quantity.
        this->evaluateQuantity();
        // Then, evaluate the index.
        this->evaluateIndex();
    }

    inline size_t length() const
    {
        return content.length();
    }

    inline bool empty() const
    {
        return content.empty();
    }

    /// Provides the original argument.
    inline std::string getOriginal() const
    {
        return original;
    }

    /// Provides the content with both index and quantity removed.
    inline std::string getContent() const
    {
        return content;
    }

    /// Provides the index.
    inline int getIndex() const
    {
        return index;
    }

    /// Provides the quantity.
    inline int getQuantity() const
    {
        return quantity;
    }

    /// @brief Checks if there is only one prefix, or there is no prefix.
    inline bool hasOnlyOnePrefix() const
    {
        return ((prefix.all + prefix.quantity + prefix.index) == 1) ||
               !(prefix.all & prefix.quantity & prefix.index);
    }

    /// Checks if the prefix "all." was specified.
    inline bool hasPrefixAll() const
    {
        return prefix.all;
    }

    /// Checks if the prefix "<quantity>*" was specified.
    inline bool hasQuantity() const
    {
        return prefix.quantity;
    }

    /// Checks if the prefix "<index>." was specified.
    inline bool hasIndex() const
    {
        return prefix.index;
    }

    /// Checks if the prefix "all." was present.
    inline bool meansAll() const
    {
        std::string lower_original = ustr::toLower(original);
        return lower_original == "all" || lower_original == "tutto";
    }

    /// Sets the content of the argument.
    inline void setString(std::string const &s)
    {
        original = content = s;
        // First, evaluate the quantity.
        this->evaluateQuantity();
        // Then, evaluate the index.
        this->evaluateIndex();
    }

    template <typename T>
    inline T toNumber() const
    {
        return ustr::toNumber<T>(content);
    }

    inline bool isNumber() const
    {
        return ustr::isNumber(content);
    }

    inline bool isAbbrev(std::string const &master, size_t minLength = 3) const
    {
        return ustr::isAbbrev(master, content, minLength);
    }

    inline bool operator==(const std::string &rhs) const
    {
        return content == rhs;
    }

    inline char operator[](size_t rhs) const
    {
        return content[rhs];
    }

    inline char &operator[](size_t rhs)
    {
        return content[rhs];
    }

    inline friend std::ostream &operator<<(std::ostream &lhs, const Argument &rhs)
    {
        lhs << rhs.content;
        return lhs;
    }

private:
    /// Evaluates the index.
    void evaluateIndex()
    {
        // If the entire string is a number, skip it.
        if (ustr::isNumber(content)) {
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
        if (ustr::isNumber(digits)) {
            // Get the number and set it.
            int number = ustr::toNumber<int>(digits);
            if ((number >= 0) && (number < INT_MAX)) {
                // Set the number.
                index = number;
            }
            // Set the prefix flag.
            prefix.index = true;
            // Remove the digits.
            content = content.substr(pos + 1, content.size());
        } else {
            std::string lower_digits = ustr::toLower(digits);
            if ((lower_digits == "all") || (lower_digits == "tutto")) {
                // Set the prefix flag.
                prefix.all = true;
                // Remove the quantity.
                content = content.substr(pos + 1, content.size());
            }
        }
    }

    /// Evaluates the quantity.
    void evaluateQuantity()
    {
        // If the entire string is a number, skip it.
        if (ustr::isNumber(content)) {
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
        if (ustr::isNumber(digits)) {
            // Set the prefix flag.
            prefix.quantity = true;
            // Get the number and set it.
            int number = ustr::toNumber<int>(digits);
            if ((number >= 0) && (number < INT_MAX)) {
                quantity = number;
            }
            // Remove the digits.
            content = content.substr(pos + 1, content.size());
        } else {
            std::string lower_digits = ustr::toLower(digits);
            if ((lower_digits == "all") || (lower_digits == "tutto")) {
                // Set the prefix flag.
                prefix.all = true;
                // Remove the quantity.
                content = content.substr(pos + 1, content.size());
            }
        }
    }
};

} // namespace interpreter