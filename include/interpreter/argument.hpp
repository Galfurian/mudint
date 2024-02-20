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
bool means_all(const std::string &word);

/// @brief Checks if the given word must be ignored.
/// @param word the word to check.
/// @return true if it must be ignored, false otherwise.
bool must_ignore(const std::string &word);

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
    std::size_t length() const;

    /// @brief Check if the `content`, not the `original` string, is empty.
    /// @return true if the content is empty, false otherwise.
    bool empty() const;

    /// @brief Provides the original argument.
    /// @return the original string.
    std::string get_original() const;

    /// @brief Provides the `content` with both index and quantity removed.
    /// @return the cleaned content.
    std::string get_content() const;

    /// @brief Forces the content to a given string.
    /// @param _content the new value.
    void set_content(const std::string &_content);

    /// @brief Provides the index extracted from the `original`.
    /// @return the extracted index.
    std::size_t get_index() const;

    /// @brief Forces a new index.
    /// @param _index the new value.
    void set_index(std::size_t _index);

    /// @brief Provides the quantity extracted from the `original`.
    /// @return the extracted quantity.
    std::size_t get_quantity() const;

    /// @brief Forces a new quantity.
    /// @param _quantity the new value.
    void set_quantity(std::size_t _quantity);

    /// @brief Checks if there is only one prefix, or there is no prefix.
    /// @return true if only one prefix is provided, false otherwise.
    bool has_only_one_prefix() const;

    /// @brief Checks if the prefix means `all`.
    /// @return true if the prefix means `all`, false otherwise.
    bool has_prefix_all() const;

    /// @brief Checks if the prefix is a quantity.
    /// @return true if the prefix is a quantity, false otherwise.
    bool has_quantity() const;

    /// @brief Checks if the prefix is an index.
    /// @return true if the prefix is an index, false otherwise.
    bool has_index() const;

    /// @brief Checks if the whole argument means `all`.
    /// @return true if the whole argument means `all`, false otherwise.
    bool means_all() const;

    /// @brief Checks if the argument is an abbreviation of the given full string.
    /// @param full_string the full string.
    /// @param sensitive enables case-sensitive check.
    /// @param min_length the minimum number of characters for the prefix.
    /// @return true if the argument is an abbreviation equally long or longher than min_length, false otherwise.
    bool is_abbreviation_of(const std::string &full_string, bool sensitive, std::size_t min_length) const;

    /// @brief Checks if the `original` content was actually a number.
    /// @return true if it is a number, false otherwise.
    bool is_number() const;

    /// @brief Transforms the `original` content into an integer value.
    /// @return the number.
    template <typename T>
    T to_number() const
    {
        return ustr::to_number<T>(original);
    }

    /// @brief Check if the `content`, not the `original` string, is equal to a given string.
    /// @param rhs the string to check.
    /// @return true if they are equal, false otherwise.
    bool operator==(const std::string &rhs) const;

    /// @brief Access the character at the given position.
    /// @param pos the position.
    /// @return the character at the given position.
    char operator[](std::size_t pos) const;

    /// @brief Access the character at the given position.
    /// @param pos the position.
    /// @return the character at the given position.
    char &operator[](std::size_t pos);

    /// @brief Sends to the output stream the argument.
    /// @param lhs the stream.
    /// @param rhs the argument.
    /// @return the stream.
    friend std::ostream &operator<<(std::ostream &lhs, const Argument &rhs);

private:
    /// Evaluates the index.
    void evaluate_index();

    /// Evaluates the quantity.
    void evaluate_quantity();
};

} // namespace interpreter
