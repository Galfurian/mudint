
#include <interpreter/argument.hpp>
#include <interpreter/interpreter.hpp>

#include "colors.hpp"

bool do_say(interpreter::Interpreter &args)
{
    std::cout << "You say '" << KCYN << args.substr(0) << RST << "'\n";
    return true;
}

bool do_look(interpreter::Interpreter &args)
{
    // Remove fill words.
    args.removeFillWords();

    // Error checking.
    if (args[0].hasPrefixAll() || args[0].hasQuantity()) {
        std::cerr << KRED << "[Arg. 1] You cannot specify a quantity.\n"
                  << RST;
        return false;
    }
    // Error checking (it is safe even if the container is not provided).
    if (args[1].hasPrefixAll() || args[1].hasQuantity()) {
        std::cerr << KRED << "[Arg. 2] You cannot specify a quantity.\n"
                  << RST;
        return false;
    }

    // The object.
    std::cout << "You look";
    if (args[0].hasIndex()) {
        size_t index = args[0].getIndex();
        std::cout << " the " << KMAG << index << ustr::get_ordinal(index) << RST;
    }
    std::cout << " " << KGRN << args[0] << RST << " ";

    // The container (if provided).
    if (args.size() == 2) {
        std::cout << "in";
        if (args[1].hasIndex()) {
            size_t index = args[1].getIndex();
            std::cout << " the " << KMAG << index << ustr::get_ordinal(index) << RST;
        }
        std::cout << " " << KGRN << args[1] << RST << " ";
    }
    std::cout << "\n";
    return true;
}

bool do_take(interpreter::Interpreter &args)
{
    // Remove fill words.
    args.removeFillWords();

    // Error checking.
    if (args[1].hasPrefixAll() || args[1].hasQuantity()) {
        std::cerr << KRED << "[Arg. 2] You cannot specify a quantity.\n"
                  << RST;
        return false;
    }

    // The object.
    std::cout << "You take";
    if (args[0].hasPrefixAll()) {
        std::cout << " " << KMAG << "all" << RST;
    } else if (args[0].hasQuantity()) {
        std::cout << " " << KMAG << args[0].getQuantity() << RST << " per";
    } else if (args[0].hasIndex()) {
        size_t index = args[0].getIndex();
        std::cout << " the " << KMAG << index << ustr::get_ordinal(index) << RST;
    }
    std::cout << " " << KGRN << args[0] << RST << " ";

    // The container (if provided).
    if (args.size() == 2) {
        std::cout << "from";
        if (args[1].hasIndex()) {
            size_t index = args[1].getIndex();
            std::cout << " the " << KMAG << index << ustr::get_ordinal(index) << RST;
        }
        std::cout << " " << KGRN << args[1] << RST << " ";
    }
    std::cout << "\n";
    return true;
}

bool do_put(interpreter::Interpreter &args)
{
    // Remove fill words.
    args.removeFillWords();

    // Check if the user provided the container.
    if (args.size() != 2) {
        std::cerr << KRED << "You must provide the container.\n"
                  << RST;
        return false;
    }
    // Error checking.
    if (args[1].hasPrefixAll() || args[1].hasQuantity()) {
        std::cerr << KRED << "[Arg. 2] You cannot specify a quantity.\n"
                  << RST;
        return false;
    }

    // Run the command.
    std::cout << "You put";
    if (args[0].hasPrefixAll()) {
        std::cout << " " << KMAG << "all" << RST;
    } else if (args[0].hasQuantity()) {
        std::cout << " " << KMAG << args[0].getQuantity() << RST << " per";
    } else if (args[0].hasIndex()) {
        std::size_t index = args[0].getIndex();
        std::cout << " the " << KMAG << index << ustr::get_ordinal(index) << RST;
    }
    std::cout << " " << KGRN << args[0] << RST << " ";

    // Container.
    std::cout << "in";
    if (args[1].hasIndex()) {
        std::size_t index = args[1].getIndex();
        std::cout << " the " << KMAG << index << ustr::get_ordinal(index) << RST;
    }
    std::cout << " " << KGRN << args[1] << RST << " ";
    std::cout << "\n";
    return true;
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
    return false;
}

void test_input(interpreter::Interpreter &args, const std::string &input)
{
    std::cout << "> " << input << "\n";
    // Parse the input.
    args.parse(input);
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
    test_input(args, "take all.pen");
    test_input(args, "take all*pen");
    test_input(args, "take pen box");
    test_input(args, "take pen from box");
    test_input(args, "take pen from the box");
    test_input(args, "take 2*pen from 2.box");
    test_input(args, "say how are you today?");
    test_input(args, "say two quantities are in the golden ratio if ...");

#if 1

    std::cout << "\nProvide some imputs, type `" << FRED("quit")
              << "` to stop...\n\n";
    // Create the input.
    std::string input;
    while (input != "quit") {
        std::cout << "> ";
        std::getline(std::cin, input);
        // Parse the input.
        args.parse(input);
        // Handle the input.
        handle_input(args);
        std::cout << "\n";
    }

#endif

    return 0;
}