/// @file ansi.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#pragma once

/// @brief ANSI escape codes.
namespace ansi
{
/// @brief Foreground colors.
namespace fg
{
// Normal colors.
extern const char *black;   ///< ANSI foreground color black.
extern const char *red;     ///< ANSI foreground color red.
extern const char *green;   ///< ANSI foreground color green.
extern const char *yellow;  ///< ANSI foreground color yellow.
extern const char *blue;    ///< ANSI foreground color blue.
extern const char *magenta; ///< ANSI foreground color magenta.
extern const char *cyan;    ///< ANSI foreground color cyan.
extern const char *white;   ///< ANSI foreground color white.
// Bright colors.
extern const char *bright_black;   ///< ANSI foreground color bright black.
extern const char *bright_red;     ///< ANSI foreground color bright red.
extern const char *bright_green;   ///< ANSI foreground color bright green.
extern const char *bright_yellow;  ///< ANSI foreground color bright yellow.
extern const char *bright_blue;    ///< ANSI foreground color bright blue.
extern const char *bright_magenta; ///< ANSI foreground color bright magenta.
extern const char *bright_cyan;    ///< ANSI foreground color bright cyan.
extern const char *bright_white;   ///< ANSI foreground color bright white.
} // namespace fg
/// @brief Background colors.
namespace bg
{
extern const char *black;   ///< ANSI background color black.
extern const char *red;     ///< ANSI background color red.
extern const char *green;   ///< ANSI background color green.
extern const char *yellow;  ///< ANSI background color yellow.
extern const char *blue;    ///< ANSI background color blue.
extern const char *magenta; ///< ANSI background color magenta.
extern const char *cyan;    ///< ANSI background color cyan.
extern const char *white;   ///< ANSI background color white.
} // namespace bg
/// @brief Utility escape codes.
namespace util
{
extern const char *reset;     ///< Reset all styles.
extern const char *bold;      ///< Turns on the bold text.
extern const char *italic;    ///< Turns on the italic text.
extern const char *underline; ///< Turns on the underline text.
extern const char *reverse;   ///< Turns on the reverse text.
extern const char *clear;     ///< Clears entire screen.
extern const char *clearline; ///< Clears entire line.
extern const char *up;        ///< Moves the cursor up by one position.
extern const char *down;      ///< Moves the cursor down by one position.
extern const char *right;     ///< Moves the cursor right by one position.
extern const char *left;      ///< Moves the cursor left by one position.
extern const char *nextline;  ///< Move cursor to beginning of line, 1 line down.
extern const char *prevline;  ///< Move cursor to beginning of line, 1 line up.
} // namespace util
} // namespace ansi
