/// @file interpreter.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Defines the interpreter class.
/// @copyright
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#pragma once

#include "argument.hpp"

#include <iomanip>
#include <iostream>

enum : unsigned char {
    MUDINT_MAJOR_VERSION = 1, ///< Major version of the library.
    MUDINT_MINOR_VERSION = 0, ///< Minor version of the library.
    MUDINT_MICRO_VERSION = 0  ///< Micro version of the library.
};

namespace interpreter
{

/// @brief Allows to simply handle players inputs.
class Interpreter
{
private:
    /// The original string.
    std::string original;
    /// List of arguments.
    std::vector<Argument> arguments;

public:
    /// @brief Iterator for arguments.
    using iterator       = std::vector<Argument>::iterator;
    /// @brief Constant iterator for arguments.
    using const_iterator = std::vector<Argument>::const_iterator;

    /// @brief Constructor.
    explicit Interpreter() = default;

    /// @brief Constructor.
    /// @param input the string containing the input from the user.
    /// @param ignore if we should ignore the list of ignored words.
    explicit Interpreter(const char *input, bool ignore);

    /// @brief Copy constructor.
    /// @param other The instance to copy from.
    Interpreter(const Interpreter &other) = default;

    /// @brief Copy assignment operator.
    /// @param other The instance to copy from.
    /// @return Reference to the instance.
    auto operator=(const Interpreter &other) -> Interpreter & = default;

    /// @brief Move constructor.
    /// @param other The instance to move from.
    Interpreter(Interpreter &&other) noexcept = default;

    /// @brief Move assignment operator.
    /// @param other The instance to move from.
    /// @return Reference to the instance.
    auto operator=(Interpreter &&other) noexcept -> Interpreter & = default;

    /// @brief Destructor.
    ~Interpreter() = default;

    /// @brief Provides the original input string.
    /// @return a copy of the original string.
    auto get_original() const -> std::string;

    /// @brief Returns the number of arguments.
    /// @return the number of arguments.
    auto size() const -> std::size_t;

    /// @brief Checks if the vector of arguments is empty.
    /// @return true if there are no arguments.
    /// @return false if there are arguments.
    auto empty() const -> bool;

    /// @brief Returns an interator to the start of the list of argments.
    /// @return the start iterator.
    inline auto begin() -> iterator;

    /// @brief Returns an interator to the start of the list of argments.
    /// @return the start iterator.
    inline auto begin() const -> const_iterator;

    /// @brief Returns an interator to the end of the list of argments.
    /// @return the end iterator.
    inline auto end() -> iterator;

    /// @brief Returns an interator to the end of the list of argments.
    /// @return the end iterator.
    inline auto end() const -> const_iterator;

    /// @brief Allows to retrieve const reference to argument at given position.
    /// @param position the index at which we retrieve the argument.
    /// @return the retrieved argument
    auto get(const std::size_t &position) -> Argument &;

    /// @brief Parse the input string.
    /// @param input the input string.
    /// @param ignore if we should ignore the list of ignored words.
    void parse(const char *input, bool ignore);

    /// @brief Finds the argument that mathes the input string.
    /// @param s the input string.
    /// @param exact if true, the string must match, otherwise it can just begin with it.
    /// @return the found argument, NULL if it was not found.
    auto find(std::string const &s, bool exact = false) const -> const Argument *;

    /// @brief Returns the string from the given argument to the end.
    /// @param _start the starting index.
    /// @param _end the final index, by default is the end of the argument list.
    /// @return the recontruscted string.
    auto substr(std::size_t _start, std::size_t _end = std::string::npos) const -> std::string;

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
    auto operator[](const std::size_t &position) -> Argument &;

    /// @brief Allows to retrieve const reference to argument at given position.
    /// @param position the index at which we retrieve the argument.
    /// @return the retrieved argument
    auto operator[](const std::size_t &position) const -> const Argument &;
};

} // namespace interpreter
