/// @file argument_handler.hpp

#pragma once

#include "argument.hpp"

#include <iostream>
#include <iomanip>

namespace interpreter
{

/// @brief Allows to simply handle players inputs.
class ArgumentHandler {
private:
    /// The original string.
    std::string original;
    /// List of arguments.
    std::vector<Argument> arguments;

public:
    typedef std::vector<Argument>::iterator iterator;
    typedef std::vector<Argument>::const_iterator const_iterator;

    /// @brief Constructor.
    explicit ArgumentHandler()
        : original(), arguments()
    {
        // Nothing to do.
    }

    /// @brief Destructor.
    ~ArgumentHandler()
    {
        // Nothing to do.
    }

    /// @brief Parse the input string.
    /// @param input the input string.
    /// @param skip_fill_words if we should skip the fill words.
    inline void parse(std::string const &input, bool skip_fill_words = false)
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
                if (skip_fill_words && this->isFillWord(*it))
                    continue;
                arguments.push_back(Argument(*it));
            }
        }
    }

    /// @brief Provides the original input string.
    /// @return a copy of the original string.
    std::string getOriginal() const
    {
        return original;
    }

    /// @brief Returns the number of arguments.
    /// @return the number of arguments.
    size_t size() const
    {
        return arguments.size();
    }

    /// @brief Checks if the vector of arguments is empty.
    /// @return true if there are no arguments.
    /// @return false if there are arguments.
    bool empty() const
    {
        return arguments.empty();
    }

    /// @brief Returns an interator to the start of the list of argments.
    /// @return the start iterator.
    inline iterator begin()
    {
        return arguments.begin();
    }

    /// @brief Returns an interator to the start of the list of argments.
    /// @return the start iterator.
    inline const_iterator begin() const
    {
        return arguments.begin();
    }

    /// @brief Returns an interator to the end of the list of argments.
    /// @return the end iterator.
    inline iterator end()
    {
        return arguments.end();
    }

    /// @brief Returns an interator to the end of the list of argments.
    /// @return the end iterator.
    inline const_iterator end() const
    {
        return arguments.end();
    }

    /// @brief Finds the argument that mathes the input string.
    /// @param s the input string.
    /// @param exact if true, the string must match, otherwise it can just begin with it.
    /// @return the found argument, NULL if it was not found.
    const Argument *find(std::string const &s, bool exact = false)
    {
        for (const_iterator it = arguments.begin(); it != arguments.end(); ++it) {
            if (exact) {
                if (it->getContent() == s) {
                    return &(*it);
                }
            } else {
                if (ustr::beginWith(it->getContent(), s)) {
                    return &(*it);
                }
            }
        }
        return NULL;
    }

    /// @brief Allows to retrieve const reference to argument at given position.
    /// @param position the index at which we retrieve the argument.
    /// @return the retrieved argument
    Argument &get(const size_t &position)
    {
        return arguments.at(position);
    }

    /// @brief Allows to retrieve const reference to argument at given position.
    /// @param position the index at which we retrieve the argument.
    /// @return the retrieved argument
    Argument &operator[](const size_t &position)
    {
        if (position >= arguments.size()) {
            static Argument empty("");
            return empty;
        }
        return arguments[position];
    }

    /// @brief Allows to retrieve const reference to argument at given position.
    /// @param position the index at which we retrieve the argument.
    /// @return the retrieved argument
    const Argument &operator[](const size_t &position) const
    {
        if (position >= arguments.size()) {
            static Argument empty("");
            return empty;
        }
        return arguments[position];
    }

    /// @brief Returns the string from the given argument to the end.
    /// @param _start the starting index.
    /// @param _end the final index, by default is the end of the argument list.
    /// @return the recontruscted string.
    std::string substr(size_t _start, size_t _end = std::string::npos) const
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

    /// @brief Erase the argument at the given position.
    /// @param position the position where the argument should be removed.
    void erase(const size_t &position)
    {
        if (position < arguments.size()) {
            std::vector<Argument>::iterator it = arguments.begin();
            std::advance(it, position);
            arguments.erase(it);
        } else {
            std::cerr << "Position out of bound!\n";
        }
    }

    /// @brief Permanently removes the fill words.
    inline void removeFillWords()
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

    /// @brief Prints a log of all the contained arguments.
    void dump() const
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

private:
    /// @brief Checks if the given argument is a fill word.
    bool isFillWord(const std::string &argument)
    {
        static const std::string fill_words[] = {
            "in", "from", "with", "and", "the", "on", "at", "to", "da", "su",
            "a", "e", "il", "la", "lo", "un", "uno", "una", "con", "\0"
        };
        std::string argument_lower = ustr::toLower(argument);
        for (int i = 0; fill_words[i] != "\0"; i++) {
            if (argument_lower == fill_words[i]) {
                return true;
            }
        }
        return false;
    }
};

} // namespace interpreter
