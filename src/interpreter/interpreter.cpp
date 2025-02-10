/// @file interpreter.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Implements the interpreter functions.
/// @copyright
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#include "interpreter/interpreter.hpp"

namespace interpreter
{

Interpreter::Interpreter(const char *input, bool ignore) { this->parse(input, ignore); }

auto Interpreter::get_original() const -> std::string { return original; }

auto Interpreter::size() const -> size_t { return arguments.size(); }

auto Interpreter::empty() const -> bool { return arguments.empty(); }

auto Interpreter::begin() -> Interpreter::iterator { return arguments.begin(); }

auto Interpreter::begin() const -> Interpreter::const_iterator { return arguments.begin(); }

auto Interpreter::end() -> Interpreter::iterator { return arguments.end(); }

auto Interpreter::end() const -> Interpreter::const_iterator { return arguments.end(); }

auto Interpreter::get(const std::size_t &position) -> Argument & { return arguments.at(position); }

void Interpreter::parse(const char *input, bool ignore)
{
    original.clear();
    arguments.clear();
    if (input != nullptr) {
        // Save the original string.
        original = input;
        // Clean the current list of arguments.
        arguments.clear();
        // Parse again the input.
        std::vector<std::string> words = ustr::split(input, " ");
        // Prepare the iterator.
        std::vector<std::string>::const_iterator it;
        // Iterate the words and add them to the list of arguments.
        for (it = words.begin(); it != words.end(); ++it) {
            if (!ignore || !interpreter::config::must_ignore(*it)) {
                arguments.emplace_back(*it);
            }
        }
    }
}

auto Interpreter::find(std::string const &s, bool exact) const -> const Argument *
{
    for (const auto &argument : arguments) {
        if (exact) {
            if (argument.get_content() == s) {
                return &argument;
            }
        } else {
            if (ustr::begin_with(argument.get_content(), s, false, 0)) {
                return &argument;
            }
        }
    }
    return nullptr;
}

auto Interpreter::substr(std::size_t _start, std::size_t _end) const -> std::string
{
    if (_start >= this->size()) {
        return original;
    }
    if (_end == std::string::npos) {
        _end = arguments.size();
    }
    std::string result;
    for (std::size_t it = _start; it < _end; ++it) {
        result.append(arguments[it].get_original());
        if (it != (arguments.size() - 1)) {
            result.push_back(' ');
        }
    }
    return result;
}

void Interpreter::erase(const std::size_t &position)
{
    if (position < arguments.size()) {
        auto it = arguments.begin();
        std::advance(it, position);
        arguments.erase(it);
    } else {
        std::cerr << "Position out of bound!\n";
    }
}

void Interpreter::remove_ignored_words()
{
    auto it = arguments.begin();
    while (it != arguments.end()) {
        if (interpreter::config::must_ignore(it->get_content())) {
            it = arguments.erase(it);
        } else {
            ++it;
        }
    }
}

void Interpreter::dump() const
{
    for (std::size_t it = 0; it < arguments.size(); ++it) {
        const interpreter::Argument &argument = arguments[it];
        std::cout << std::setw(2) << std::right << it << " | ";
        std::cout << std::setw(12) << std::left << argument.get_original();
        std::cout << std::setw(12) << std::left << argument.get_content() << " | ";
        if (argument.has_index()) {
            std::cout << " Index: " << std::setw(2) << argument.get_index() << " ";
        }
        if (argument.has_quantity()) {
            std::cout << " Quantity: " << std::setw(2) << argument.get_quantity() << " ";
        }
        if (argument.has_prefix_all()) {
            std::cout << " Quantity: ALL ";
        }
        std::cout << "\n";
    }
}

auto Interpreter::operator[](const std::size_t &position) -> Argument &
{
    if (position >= arguments.size()) {
        static Argument empty("");
        return empty;
    }
    return arguments[position];
}

auto Interpreter::operator[](const std::size_t &position) const -> const Argument &
{
    if (position >= arguments.size()) {
        static Argument empty("");
        return empty;
    }
    return arguments[position];
}

} // namespace interpreter
