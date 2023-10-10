/// @file ustr.hpp

#pragma once

#include <limits.h>
#include <vector>
#include <string>

namespace interpreter::ustr
{

/// @brief Checks if the given string is a number.
inline bool isNumber(const std::string &str)
{
    if (str.empty())
        return false;
    return str.find_first_not_of("+-0123456789") == std::string::npos;
}

/// @brief Transform a string into a numeric value.
/// @param source The string to turn into a number.
/// @return The number.
template <typename T>
T toNumber(const std::string &source)
{
    char *pEnd;
    return static_cast<T>(strtol(source.c_str(), &pEnd, 10));
}

/// @brief Converte la stringa data in Lower-Case.
/// @param s La stringa da convertire.
/// @return Una nuova stringa completamente in Lower-Case.
inline std::string toLower(const std::string &s)
{
    std::string sr;
    for (std::string::const_iterator i = s.begin(); i != s.end(); ++i)
        sr.push_back(static_cast<char>(std::tolower(*i)));
    return sr;
}

/// @brief Check if the source string is an abbreviation of the master string.
/// @param source Source string.
/// @param master Master string.
/// @return <b>True</b> if the source string is an abbreviation of the master
/// string,<br>
///         <b>False</b> otherwise.
bool isAbbrev(const std::string &master, const std::string &source, size_t minLength = 2)
{
    return source.size() >= minLength && source.size() <= master.size() &&
           std::equal(source.begin(), source.end(), master.begin());
}

/// @brief Given a string and a delimiter, the string is splitted
///         by using the delimiter.
/// @param source    The source string.
/// @param delimiter The delimiter which has to be used.
/// @return The splitted string.
inline std::vector<std::string> split(const std::string &s,
                                      const std::string &delimiter)
{
    std::vector<std::string> result;
    std::string::size_type curr = 0, next = 0;
    while ((next = s.find_first_of(delimiter, curr)) != std::string::npos) {
        if (next - curr > 0)
            result.push_back(s.substr(curr, next - curr));
        curr = next + 1;
    }
    std::string last(s, curr);
    if (!last.empty())
        result.push_back(last);
    return result;
}

/// @brief Check if the source string begin with a given prefix.
/// @param source Source string.
/// @param prefix Prefix string.
/// @return <b>True</b> if the source begin with the given prefix,<br>
///         <b>False</b> otherwise.
bool beginWith(const std::string &source, const std::string &prefix, bool sensitive = false, int n = -1)
{
    if (&prefix == &source)
        return true;
    if (prefix.length() > source.length())
        return false;
    if (source.empty() || prefix.empty())
        return false;
    std::string::const_iterator it0 = source.begin(), it1 = prefix.begin();
    if (sensitive) {
        while ((it1 != prefix.end()) && ((*it1) == (*it0))) {
            if ((n > 0) && (--n <= 0))
                return true;
            ++it0, ++it1;
        }
    } else {
        while ((it1 != prefix.end()) && (tolower(*it1) == tolower(*it0))) {
            if ((n > 0) && (--n <= 0))
                return true;
            ++it0, ++it1;
        }
    }
    return it1 == prefix.end();
}

template <typename T>
const char *get_ordinal(T n)
{
    static const char suffixes[][3] = { "th", "st", "nd", "rd" };
    auto ord                        = n % 100;
    if (ord / 10 == 1) {
        ord = 0;
    }
    ord = ord % 10;
    if (ord > 3) {
        ord = 0;
    }
    return suffixes[ord];
}

} // namespace interpreter::ustr
