
#include <interpreter/argument.hpp>
#include <interpreter/interpreter.hpp>

#include "ansi.hpp"

const char *ansi::fg::black   = "\33[30m";
const char *ansi::fg::red     = "\33[31m";
const char *ansi::fg::green   = "\33[32m";
const char *ansi::fg::yellow  = "\33[33m";
const char *ansi::fg::blue    = "\33[34m";
const char *ansi::fg::magenta = "\33[35m";
const char *ansi::fg::cyan    = "\33[36m";
const char *ansi::fg::white   = "\33[37m";

const char *ansi::fg::bright_black   = "\33[30;1m";
const char *ansi::fg::bright_red     = "\33[31;1m";
const char *ansi::fg::bright_green   = "\33[32;1m";
const char *ansi::fg::bright_yellow  = "\33[33;1m";
const char *ansi::fg::bright_blue    = "\33[34;1m";
const char *ansi::fg::bright_magenta = "\33[35;1m";
const char *ansi::fg::bright_cyan    = "\33[36;1m";
const char *ansi::fg::bright_white   = "\33[37;1m";

const char *ansi::bg::black   = "\33[40m";
const char *ansi::bg::red     = "\33[41m";
const char *ansi::bg::green   = "\33[42m";
const char *ansi::bg::yellow  = "\33[43m";
const char *ansi::bg::blue    = "\33[44m";
const char *ansi::bg::magenta = "\33[45m";
const char *ansi::bg::cyan    = "\33[46m";
const char *ansi::bg::white   = "\33[47m";

const char *ansi::util::reset     = "\33[0m";
const char *ansi::util::bold      = "\33[1m";
const char *ansi::util::italic    = "\33[3m";
const char *ansi::util::underline = "\33[4m";
const char *ansi::util::reverse   = "\33[7m";
const char *ansi::util::clear     = "\33[2J";
const char *ansi::util::clearline = "\33[2K";
const char *ansi::util::up        = "\33[1A";
const char *ansi::util::down      = "\33[1B";
const char *ansi::util::right     = "\33[1C";
const char *ansi::util::left      = "\33[1D";
const char *ansi::util::nextline  = "\33[1E";
const char *ansi::util::prevline  = "\33[1F";

bool do_say(interpreter::Interpreter &args)
{
    std::string message = ustr::trim(args.substr(0));
    if (ustr::end_with(message, "?", false, 0)) {
        std::cout << "You ask '" << ansi::fg::yellow;
    } else {
        std::cout << "You say '" << ansi::fg::cyan;
    }
    std::cout << ansi::util::italic << message << ansi::util::reset << "'\n";
    return true;
}

bool do_look(interpreter::Interpreter &args)
{
    // Remove ignored words.
    args.remove_ignored_words();

    // Error checking.
    if (args[0].has_prefix_all() || args[0].has_quantity()) {
        std::cerr << ansi::fg::red << "[Arg. 1] You cannot specify a quantity.\n" << ansi::util::reset;
        return false;
    }
    // Error checking (it is safe even if the container is not provided).
    if (args[1].has_prefix_all() || args[1].has_quantity()) {
        std::cerr << ansi::fg::red << "[Arg. 2] You cannot specify a quantity.\n" << ansi::util::reset;
        return false;
    }

    // The object.
    std::cout << "You look";
    if (args[0].has_index()) {
        std::size_t index = args[0].get_index();
        std::cout << " the " << ansi::fg::magenta << index << ustr::get_ordinal(index) << ansi::util::reset;
    }
    std::cout << " " << ansi::fg::green << args[0] << ansi::util::reset << " ";

    // The container (if provided).
    if (args.size() == 2) {
        std::cout << "in";
        if (args[1].has_index()) {
            std::size_t index = args[1].get_index();
            std::cout << " the " << ansi::fg::magenta << index << ustr::get_ordinal(index) << ansi::util::reset;
        }
        std::cout << " " << ansi::fg::green << args[1] << ansi::util::reset << " ";
    }
    std::cout << "\n";
    return true;
}

bool do_take(interpreter::Interpreter &args)
{
    // Remove ignored words.
    args.remove_ignored_words();

    // Error checking.
    if (!args[0].has_only_one_prefix()) {
        std::cerr << ansi::fg::red << "[Arg. 1] You cannot specify both quantity and index.\n" << ansi::util::reset;
        return false;
    }
    if ((args.size() == 2) && (!args[1].has_only_one_prefix())) {
        std::cerr << ansi::fg::red << "[Arg. 2] You cannot specify both quantity and index.\n" << ansi::util::reset;
        return false;
    }

    // The object.
    std::cout << "You take";
    if (args[0].has_prefix_all()) {
        std::cout << " " << ansi::fg::magenta << "all" << ansi::util::reset;
    } else if (args[0].has_quantity()) {
        std::cout << " " << ansi::fg::magenta << args[0].get_quantity() << ansi::util::reset << " per";
    } else if (args[0].has_index()) {
        std::size_t index = args[0].get_index();
        std::cout << " the " << ansi::fg::magenta << index << ustr::get_ordinal(index) << ansi::util::reset;
    }
    std::cout << " " << ansi::fg::green << args[0] << ansi::util::reset << " ";

    // The container (if provided).
    if (args.size() == 2) {
        std::cout << "from";
        if (args[1].has_index()) {
            std::size_t index = args[1].get_index();
            std::cout << " the " << ansi::fg::magenta << index << ustr::get_ordinal(index) << ansi::util::reset;
        }
        std::cout << " " << ansi::fg::green << args[1] << ansi::util::reset << " ";
    }
    std::cout << "\n";
    return true;
}

bool do_put(interpreter::Interpreter &args)
{
    // Remove ignored words.
    args.remove_ignored_words();

    // Check if the user provided the container.
    if (args.size() != 2) {
        std::cerr << ansi::fg::red << "You must provide the container.\n" << ansi::util::reset;
        return false;
    }
    // Error checking.
    if (args[1].has_prefix_all() || args[1].has_quantity()) {
        std::cerr << ansi::fg::red << "[Arg. 2] You cannot specify a quantity.\n" << ansi::util::reset;
        return false;
    }

    // Run the command.
    std::cout << "You put";
    if (args[0].has_prefix_all()) {
        std::cout << " " << ansi::fg::magenta << "all" << ansi::util::reset;
    } else if (args[0].has_quantity()) {
        std::cout << " " << ansi::fg::magenta << args[0].get_quantity() << ansi::util::reset << " per";
    } else if (args[0].has_index()) {
        std::size_t index = args[0].get_index();
        std::cout << " the " << ansi::fg::magenta << index << ustr::get_ordinal(index) << ansi::util::reset;
    }
    std::cout << " " << ansi::fg::green << args[0] << ansi::util::reset << " ";

    // Container.
    std::cout << "in";
    if (args[1].has_index()) {
        std::size_t index = args[1].get_index();
        std::cout << " the " << ansi::fg::magenta << index << ustr::get_ordinal(index) << ansi::util::reset;
    }
    std::cout << " " << ansi::fg::green << args[1] << ansi::util::reset << " ";
    std::cout << "\n";
    return true;
}

bool do_configure(interpreter::Interpreter &args)
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
        std::cout << "You selected " << ansi::fg::magenta << "name" << ansi::util::reset;
    } else if (option == option_type_address) {
        std::cout << "You selected " << ansi::fg::magenta << "address" << ansi::util::reset;
    } else {
        std::cout << ansi::fg::red << "Selection is not valid" << ansi::util::reset;
    }
    return false;
}

bool handle_input(interpreter::Interpreter &args)
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

void test_input(interpreter::Interpreter &args, const char *input)
{
    std::cout << "> " << input << "\n";
    // Parse the input.
    args.parse(input, false);
    // Handle the input.
    handle_input(args);
    std::cout << "\n";
}

int main(int, char **)
{
    // Create the interpreter.
    interpreter::Interpreter args;

    test_input(args, "take pen");
    test_input(args, "take 2*pen");
    test_input(args, "take 2.pen");
    test_input(args, "take 2*2.pen");
    test_input(args, "take all.pen");
    test_input(args, "take all*pen");
    test_input(args, "take pen box");
    test_input(args, "take pen from box");
    test_input(args, "take pen from the box");
    test_input(args, "take 2*pen from 2.box");
    test_input(args, "take all*pen from 2.box");
    test_input(args, "say how are you today?");
    test_input(args, "say two quantities are in the golden ratio if ...");

#if 1

    std::cout << "\nProvide some imputs, type `" << ansi::fg::red << "quit" << ansi::util::reset << "` to stop...\n\n";
    // Create the input.
    std::string input;
    while (input != "quit") {
        std::cout << "> ";
        std::getline(std::cin, input);
        // Parse the input.
        args.parse(input.c_str(), false);
        // Handle the input.
        handle_input(args);
        std::cout << "\n";
    }

#endif

    return 0;
}