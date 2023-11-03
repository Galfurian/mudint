/// @file argument.hpp

#pragma once

#include <climits>
#include <iostream>

#include <ustr/check.hpp>
#include <ustr/manipulate.hpp>
#include <ustr/utility.hpp>

namespace interpreter
{

/// @brief JSON parser configuration.
namespace config
{
/// @brief The words for all, by default "all".
extern std::vector<std::string> all;
} // namespace config

/// @brief Allows to easily manage input arguments from players.
class Argument {
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
    explicit Argument(const std::string &_original);

    void parse(const std::string &_original);

    size_t length() const;

    bool empty() const;

    /// Provides the original argument.
    std::string getOriginal() const;

    /// Provides the content with both index and quantity removed.
    std::string getContent() const;

    /// Provides the index.
    std::size_t getIndex() const;

    /// Provides the quantity.
    std::size_t getQuantity() const;

    /// @brief Checks if there is only one prefix, or there is no prefix.
    bool hasOnlyOnePrefix() const;

    /// Checks if the prefix "all." was specified.
    bool hasPrefixAll() const;

    /// Checks if the prefix "<quantity>*" was specified.
    bool hasQuantity() const;

    /// Checks if the prefix "<index>." was specified.
    bool hasIndex() const;

    /// Checks if the prefix "all." was present.
    bool meansAll() const;

    /// Sets the content of the argument.
    void setString(std::string const &s);

    bool operator==(const std::string &rhs) const;

    char operator[](size_t rhs) const;

    char &operator[](size_t rhs);

    friend std::ostream &operator<<(std::ostream &lhs, const Argument &rhs);

private:
    /// Evaluates the index.
    void evaluateIndex();

    /// Evaluates the quantity.
    void evaluateQuantity();
};

} // namespace interpreter