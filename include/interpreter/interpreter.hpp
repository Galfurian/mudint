/// @file interpreter.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Defines the interpreter class.

#pragma once

#include "argument.hpp"

#include <iomanip>
#include <iostream>

namespace interpreter
{

/// @brief Allows to simply handle players inputs.
class Interpreter {
private:
    /// The original string.
    std::string original;
    /// List of arguments.
    std::vector<Argument> arguments;

public:
    /// @brief Iterator for arguments.
    typedef std::vector<Argument>::iterator iterator;
    /// @brief Constant iterator for arguments.
    typedef std::vector<Argument>::const_iterator const_iterator;

    /// @brief Constructor.
    explicit Interpreter() = default;

    /// @brief Constructor.
    /// @param input the string containing the input from the user.
    /// @param ignore if we should ignore the list of ignored words.
    explicit Interpreter(const std::string &input, bool ignore);

    /// @brief Destructor.
    ~Interpreter() = default;

    /// @brief Provides the original input string.
    /// @return a copy of the original string.
    std::string get_original() const;

    /// @brief Returns the number of arguments.
    /// @return the number of arguments.
    std::size_t size() const;

    /// @brief Checks if the vector of arguments is empty.
    /// @return true if there are no arguments.
    /// @return false if there are arguments.
    bool empty() const;

    /// @brief Returns an interator to the start of the list of argments.
    /// @return the start iterator.
    inline iterator begin();

    /// @brief Returns an interator to the start of the list of argments.
    /// @return the start iterator.
    inline const_iterator begin() const;

    /// @brief Returns an interator to the end of the list of argments.
    /// @return the end iterator.
    inline iterator end();

    /// @brief Returns an interator to the end of the list of argments.
    /// @return the end iterator.
    inline const_iterator end() const;

    /// @brief Allows to retrieve const reference to argument at given position.
    /// @param position the index at which we retrieve the argument.
    /// @return the retrieved argument
    Argument &get(const std::size_t &position);

    /// @brief Parse the input string.
    /// @param input the input string.
    /// @param ignore if we should ignore the list of ignored words.
    void parse(std::string const &input, bool ignore);

    /// @brief Finds the argument that mathes the input string.
    /// @param s the input string.
    /// @param exact if true, the string must match, otherwise it can just begin with it.
    /// @return the found argument, NULL if it was not found.
    const Argument *find(std::string const &s, bool exact = false) const;

    /// @brief Returns the string from the given argument to the end.
    /// @param _start the starting index.
    /// @param _end the final index, by default is the end of the argument list.
    /// @return the recontruscted string.
    std::string substr(std::size_t _start, std::size_t _end = std::string::npos) const;

    /// @brief Erase the argument at the given position.
    /// @param position the position where the argument should be removed.
    void erase(const std::size_t &position);

    /// @brief Permanently removes the fill words.
    void remove_ignored_words();

    /// @brief Prints a log of all the contained arguments.
    void dump() const;

    /// @brief Allows to retrieve const reference to argument at given position.
    /// @param position the index at which we retrieve the argument.
    /// @return the retrieved argument
    Argument &operator[](const std::size_t &position);

    /// @brief Allows to retrieve const reference to argument at given position.
    /// @param position the index at which we retrieve the argument.
    /// @return the retrieved argument
    const Argument &operator[](const std::size_t &position) const;
};

} // namespace interpreter
