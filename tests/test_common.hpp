/// @file test_common.hpp
/// @brief Common setup for all tests.
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#pragma once

#include <interpreter/argument.hpp>
#include <interpreter/interpreter.hpp>

#include <functional>
#include <iostream>
#include <sstream>

static inline bool do_say(interpreter::Interpreter &args)
{
    std::string message = ustr::trim(args.substr(0));
    if (ustr::end_with(message, "?", false, 0)) {
        std::cout << "You ask '" << message << "'\n";
    } else {
        std::cout << "You say '" << message << "'\n";
    }
    return true;
}

static inline bool do_look(interpreter::Interpreter &args)
{
    // Remove ignored words.
    args.remove_ignored_words();

    // Error checking.
    if (args[0].has_prefix_all() || args[0].has_quantity()) {
        std::cout << "[Arg. 1] You cannot specify a quantity.\n";
        return false;
    }
    // Error checking (it is safe even if the container is not provided).
    if (args[1].has_prefix_all() || args[1].has_quantity()) {
        std::cout << "[Arg. 2] You cannot specify a quantity.\n";
        return false;
    }

    // The object.
    std::cout << "You look";
    if (args[0].has_index()) {
        std::size_t index = args[0].get_index();
        std::cout << " the " << index << ustr::get_ordinal(index);
    }
    std::cout << " " << args[0] << " ";

    // The container (if provided).
    if (args.size() == 2) {
        std::cout << "in";
        if (args[1].has_index()) {
            std::size_t index = args[1].get_index();
            std::cout << " the " << index << ustr::get_ordinal(index);
        }
        std::cout << " " << args[1] << " ";
    }
    std::cout << "\n";
    return true;
}

static inline bool do_take(interpreter::Interpreter &args)
{
    // Remove ignored words.
    args.remove_ignored_words();

    // Error checking.
    if (!args[0].has_only_one_prefix()) {
        std::cout << "[Arg. 1] You cannot specify both quantity and index.\n";
        return false;
    }
    if ((args.size() == 2) && (!args[1].has_only_one_prefix())) {
        std::cout << "[Arg. 2] You cannot specify both quantity and index.\n";
        return false;
    }

    // The object.
    std::cout << "You take";
    if (args[0].has_prefix_all()) {
        std::cout << " " << "all";
    } else if (args[0].has_quantity()) {
        std::cout << " " << args[0].get_quantity() << " per";
    } else if (args[0].has_index()) {
        std::size_t index = args[0].get_index();
        std::cout << " the " << index << ustr::get_ordinal(index);
    }
    std::cout << " " << args[0] << " ";

    // The container (if provided).
    if (args.size() == 2) {
        std::cout << "from";
        if (args[1].has_index()) {
            std::size_t index = args[1].get_index();
            std::cout << " the " << index << ustr::get_ordinal(index);
        }
        std::cout << " " << args[1] << " ";
    }
    std::cout << "\n";
    return true;
}

static inline bool do_put(interpreter::Interpreter &args)
{
    // Remove ignored words.
    args.remove_ignored_words();

    // Check if the user provided the container.
    if (args.size() != 2) {
        std::cout << "You must provide the container.\n";
        return false;
    }
    // Error checking.
    if (args[1].has_prefix_all() || args[1].has_quantity()) {
        std::cout << "[Arg. 2] You cannot specify a quantity.\n";
        return false;
    }

    // Run the command.
    std::cout << "You put";
    if (args[0].has_prefix_all()) {
        std::cout << " " << "all";
    } else if (args[0].has_quantity()) {
        std::cout << " " << args[0].get_quantity() << " per";
    } else if (args[0].has_index()) {
        std::size_t index = args[0].get_index();
        std::cout << " the " << index << ustr::get_ordinal(index);
    }
    std::cout << " " << args[0] << " ";

    // Container.
    std::cout << "in";
    if (args[1].has_index()) {
        std::size_t index = args[1].get_index();
        std::cout << " the " << index << ustr::get_ordinal(index);
    }
    std::cout << " " << args[1] << " ";
    std::cout << "\n";
    return true;
}

static inline bool do_configure(interpreter::Interpreter &args)
{
    // Set the list of options.
    enum option_type_t {
        option_type_none,
        option_type_name,
        option_type_address,
    };
    std::vector<interpreter::Option> option_list = {
        {option_type_name, {"name"}},
        {option_type_address, {"address"}},
    };
    unsigned option = args[0].map_to_option(option_list, [](const std::string &content, const std::string &name) {
        return ustr::is_abbreviation_of(content, name, false, 3);
    });

    if (option == option_type_name) {
        std::cout << "You selected " << "name";
    } else if (option == option_type_address) {
        std::cout << "You selected " << "address";
    } else {
        std::cout << "Selection is not valid";
    }
    return false;
}

static inline bool handle_input(interpreter::Interpreter &args)
{
    // Handle the input.
    if (args[0] == "say") {
        args.erase(0);
        return do_say(args);
    }
    if (args[0] == "look") {
        args.erase(0);
        return do_look(args);
    }
    if (args[0] == "take") {
        args.erase(0);
        return do_take(args);
    }
    if (args[0] == "put") {
        args.erase(0);
        return do_put(args);
    }
    if (ustr::is_abbreviation_of(args[0].get_content(), "configure", false, 3)) {
        args.erase(0);
        return do_configure(args);
    }
    return false;
}

static inline void test_input(interpreter::Interpreter &args, const std::string &input)
{
    std::cout << "> " << input << "\n";
    // Parse the input.
    args.parse(input.c_str(), false);
    // Handle the input.
    handle_input(args);
    std::cout << "\n";
}

static inline void capture_stdout(const std::function<void()> &func, std::string &output)
{
    std::ostringstream capture;
    std::streambuf *old_cout_buf = std::cout.rdbuf(capture.rdbuf()); // Redirect std::cout
    func();
    output = capture.str();
    std::cout.rdbuf(old_cout_buf); // Restore std::cout
}
