---
layout: default
---

# MUDInt

## Multi User Dungeon Interpreter

[![Ubuntu](https://github.com/Galfurian/ustr/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/Galfurian/ustr/actions/workflows/ubuntu.yml)
[![Windows](https://github.com/Galfurian/ustr/actions/workflows/windows.yml/badge.svg)](https://github.com/Galfurian/ustr/actions/workflows/windows.yml)
[![MacOS](https://github.com/Galfurian/ustr/actions/workflows/macos.yml/badge.svg)](https://github.com/Galfurian/ustr/actions/workflows/macos.yml)
[![Documentation](https://github.com/Galfurian/ustr/actions/workflows/documentation.yml/badge.svg)](https://github.com/Galfurian/ustr/actions/workflows/documentation.yml)

This repository provides a C++ library for interpreting and managing player inputs in a text-based Multi-User Dungeon (MUD). The library is designed to parse player commands, manage arguments, and handle structured input for a seamless gaming experience.

## Features

- **Argument Parsing**: Extract and manage arguments from player commands.
- **Command Handling**: Execute predefined game commands such as say, look, take, and more.
- **Error Checking**: Robust error handling for invalid input scenarios.
- **Customizability**: Easily extend the interpreter for new commands or argument structures.

## Files

### `argument.hpp`

Defines the Argument class for parsing and managing individual arguments within player input.

Key Methods:

- `length()`, `empty()`: Check argument length and emptiness.
- `get_content()`, `set_content()`: Manage cleaned content (without index or quantity).
- `get_index()`, `get_quantity()`: Retrieve parsed indices or quantities.
- `has_prefix_all()`, `has_quantity()`, `has_index()`: Determine argument prefixes.
- `is_abbreviation_of()`, `is_number()`: Validate argument types.
- `to_number<T>()`: Convert argument content to numeric values.

### `interpreter.hpp`

Defines the Interpreter class for managing player input and coordinating with Argument objects.

Key Methods:

- `parse()`: Parse raw input into structured arguments.
- `find()`, `substr()`: Search for or reconstruct parts of the input.
- `remove_ignored_words()`: Filter out filler words.
- `dump()`: Print debug information about parsed arguments.

### Example Implementation

The example program demonstrates:

- Command definitions (do_say, do_look, do_take, do_put, do_configure).
- Input handling via handle_input() function.
- Sample command execution flow using the interpreter.

## Usage

### 1. Including the Library

Include the headers in your project:

```cpp
#include <interpreter/argument.hpp>
#include <interpreter/interpreter.hpp>
```

### 2. Parsing Input

Initialize the interpreter and parse input:

```cpp
interpreter::Interpreter args;
args.parse("take 2*pen from box", false);
```

### 3. Handling Commands

Define command functions, e.g., do_take:

```cpp
bool do_take(interpreter::Interpreter &args) {
    args.remove_ignored_words();
    if (!args[0].has_only_one_prefix()) {
        std::cerr << "Error: Invalid argument format.\n";
        return false;
    }
    std::cout << "You take " << args[0].get_content() << "\n";
    return true;
}
```

Use `handle_input()` to route commands based on input:

```cpp
if (args[0] == "take") {
    args.erase(0); // Remove command name
    return do_take(args);
}
```

### 4. Running the Example

Run the provided example program to test commands interactively:

```bash
> take pen
> take 2*pen from box
> say Hello, world!
> quit
```

### 5. Example Commands

- Say: `say Hello!` — Outputs: `You say 'Hello!'.`
- Look: `look box` — Outputs: `You look box.`
- Take: `take 2*pen from box` — Outputs: `You take 2 pens from box.`
- Put: `put pen in box` — Outputs: `You put pen in box.`
- Configure: `configure name` — Outputs: `You selected name.`

### 6. Customization

Add new commands by implementing corresponding functions and updating `handle_input()`.
Customize argument prefixes or ignored words by modifying the `interpreter::config` namespace.

## Dependencies

- Standard C++ libraries (`<vector>`, `<string>`, `<iostream>`, etc.).
- `ustr` utility for string manipulation, it's already fetched by CMake, no need to worry.

## Contributing

Contributions are welcome! If you have suggestions, bug reports, or new features
to propose, feel free to open an issue or submit a pull request.

Steps to Contribute:

- Fork the repository.
- Create a new branch: `git checkout -b my-feature-branch`
- Commit your changes: `git commit -m 'Add some feature'`
- Push the branch: `git push origin my-feature-branch`
- Open a pull request.

## License

`MUDInt` is licensed under the MIT License.
