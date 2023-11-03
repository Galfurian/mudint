/// @file interpreter.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Implements the interpreter functions.

#include "interpreter/interpreter.hpp"

namespace interpreter
{

std::string Interpreter::getOriginal() const
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

Argument &Interpreter::get(const size_t &position)
{
    return arguments.at(position);
}

void Interpreter::parse(std::string const &input, bool skip_fill_words)
{
    if (!input.empty()) {
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
            if (skip_fill_words && this->isFillWord(*it)) {
                continue;
            }
            arguments.push_back(Argument(*it));
        }
    }
}

const Argument *Interpreter::find(std::string const &s, bool exact) const
{
    for (const_iterator it = arguments.begin(); it != arguments.end(); ++it) {
        if (exact) {
            if (it->getContent() == s) {
                return &(*it);
            }
        } else {
            if (ustr::begin_with(it->getContent(), s, false, 0)) {
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
    for (size_t it = _start; it < _end; ++it) {
        result.append(arguments[it].getOriginal());
        if (it != (arguments.size() - 1)) {
            result.push_back(' ');
        }
    }
    return result;
}

void Interpreter::erase(const size_t &position)
{
    if (position < arguments.size()) {
        std::vector<Argument>::iterator it = arguments.begin();
        std::advance(it, position);
        arguments.erase(it);
    } else {
        std::cerr << "Position out of bound!\n";
    }
}

void Interpreter::removeFillWords()
{
    iterator it = arguments.begin();
    while (it != arguments.end()) {
        if (this->isFillWord(it->getContent())) {
            it = arguments.erase(it);
        } else {
            ++it;
        }
    }
}

void Interpreter::dump() const
{
    for (size_t it = 0; it < arguments.size(); ++it) {
        const interpreter::Argument &argument = arguments[it];
        std::cout << std::setw(2) << std::right << it << " | ";
        std::cout << std::setw(12) << std::left << argument.getOriginal();
        std::cout << std::setw(12) << std::left << argument.getContent() << " | ";
        if (argument.hasIndex()) {
            std::cout << " Index: " << std::setw(2) << argument.getIndex() << " ";
        }
        if (argument.hasQuantity()) {
            std::cout << " Quantity: " << std::setw(2) << argument.getQuantity()
                      << " ";
        }
        if (argument.hasPrefixAll()) {
            std::cout << " Quantity: ALL ";
        }
        std::cout << "\n";
    }
}

Argument &Interpreter::operator[](const size_t &position)
{
    if (position >= arguments.size()) {
        static Argument empty("");
        return empty;
    }
    return arguments[position];
}

const Argument &Interpreter::operator[](const size_t &position) const
{
    if (position >= arguments.size()) {
        static Argument empty("");
        return empty;
    }
    return arguments[position];
}

bool Interpreter::isFillWord(const std::string &argument) const
{
    static const std::string fill_words[] = {
        "in", "from", "with", "and", "the", "on", "at", "to", "da", "su",
        "a", "e", "il", "la", "lo", "un", "uno", "una", "con", "\0"
    };
    std::string argument_lower = ustr::to_lower(argument);
    for (int i = 0; fill_words[i] != "\0"; i++) {
        if (argument_lower == fill_words[i]) {
            return true;
        }
    }
    return false;
}

} // namespace interpreter
