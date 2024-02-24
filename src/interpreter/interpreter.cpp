/// @file interpreter.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Implements the interpreter functions.

#include "interpreter/interpreter.hpp"

namespace interpreter
{

Interpreter::Interpreter(const char *input, bool ignore)
    : original(),
      arguments()
{
    if (input) {
        this->parse(input, ignore);
    }
}

std::string Interpreter::get_original() const
{
    return original;
}

size_t Interpreter::size() const
{
    return arguments.size();
}

bool Interpreter::empty() const
{
    return arguments.empty();
}

Interpreter::iterator Interpreter::begin()
{
    return arguments.begin();
}

Interpreter::const_iterator Interpreter::begin() const
{
    return arguments.begin();
}

Interpreter::iterator Interpreter::end()
{
    return arguments.end();
}

Interpreter::const_iterator Interpreter::end() const
{
    return arguments.end();
}

Argument &Interpreter::get(const std::size_t &position)
{
    return arguments.at(position);
}

void Interpreter::parse(const char *input, bool ignore)
{
    if (input) {
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
                arguments.push_back(Argument(*it));
            }
        }
    }
}

const Argument *Interpreter::find(std::string const &s, bool exact) const
{
    for (const_iterator it = arguments.begin(); it != arguments.end(); ++it) {
        if (exact) {
            if (it->get_content() == s) {
                return &(*it);
            }
        } else {
            if (ustr::begin_with(it->get_content(), s, false, 0)) {
                return &(*it);
            }
        }
    }
    return NULL;
}

std::string Interpreter::substr(std::size_t _start, std::size_t _end) const
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
        std::vector<Argument>::iterator it = arguments.begin();
        std::advance(it, position);
        arguments.erase(it);
    } else {
        std::cerr << "Position out of bound!\n";
    }
}

void Interpreter::remove_ignored_words()
{
    iterator it = arguments.begin();
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
            std::cout << " Quantity: " << std::setw(2) << argument.get_quantity()
                      << " ";
        }
        if (argument.has_prefix_all()) {
            std::cout << " Quantity: ALL ";
        }
        std::cout << "\n";
    }
}

Argument &Interpreter::operator[](const std::size_t &position)
{
    if (position >= arguments.size()) {
        static Argument empty("");
        return empty;
    }
    return arguments[position];
}

const Argument &Interpreter::operator[](const std::size_t &position) const
{
    if (position >= arguments.size()) {
        static Argument empty("");
        return empty;
    }
    return arguments[position];
}

} // namespace interpreter
