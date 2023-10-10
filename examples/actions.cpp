
#include "interpreter/argument.hpp"
#include "interpreter/argument_handler.hpp"
#include "interpreter/ustr.hpp"

#include "colors.hpp"

using namespace interpreter;

bool do_say(ArgumentHandler &handler) {
  std::cout << "You say '" << KCYN << handler.substr(1) << RST << "'\n";
  return true;
}

bool do_look(ArgumentHandler &handler) {
  // Remove fill words.
  handler.removeFillWords();

  // Error checking.
  if (handler[1].hasPrefixAll() || handler[1].hasQuantity()) {
    std::cerr << KRED << "[Arg. 1] You cannot specify a quantity.\n" << RST;
    return false;
  }
  // Error checking (it is safe even if the container is not provided).
  if (handler[2].hasPrefixAll() || handler[2].hasQuantity()) {
    std::cerr << KRED << "[Arg. 2] You cannot specify a quantity.\n" << RST;
    return false;
  }

  // The object.
  std::cout << "You look";
  if (handler[1].hasIndex()) {
    size_t index = handler[1].getIndex();
    std::cout << " the " << KMAG << index << ustr::get_ordinal(index) << RST;
  }
  std::cout << " " << KGRN << handler[1] << RST << " ";

  // The container (if provided).
  if (handler.size() == 3) {
    std::cout << "in";
    if (handler[2].hasIndex()) {
      size_t index = handler[2].getIndex();
      std::cout << " the " << KMAG << index << ustr::get_ordinal(index) << RST;
    }
    std::cout << " " << KGRN << handler[2] << RST << " ";
  }
  std::cout << "\n";
  return true;
}

bool do_take(ArgumentHandler &handler) {
  // Remove fill words.
  handler.removeFillWords();

  // Error checking.
  if (handler[2].hasPrefixAll() || handler[2].hasQuantity()) {
    std::cerr << KRED << "[Arg. 2] You cannot specify a quantity.\n" << RST;
    return false;
  }

  // The object.
  std::cout << "You take";
  if (handler[1].hasPrefixAll()) {
    std::cout << " " << KMAG << "all" << RST;
  } else if (handler[1].hasQuantity()) {
    std::cout << " " << KMAG << handler[1].getQuantity() << RST << " per";
  } else if (handler[1].hasIndex()) {
    size_t index = handler[1].getIndex();
    std::cout << " the " << KMAG << index << ustr::get_ordinal(index) << RST;
  }
  std::cout << " " << KGRN << handler[1] << RST << " ";

  // The container (if provided).
  if (handler.size() == 3) {
    std::cout << "from";
    if (handler[2].hasIndex()) {
      size_t index = handler[2].getIndex();
      std::cout << " the " << KMAG << index << ustr::get_ordinal(index) << RST;
    }
    std::cout << " " << KGRN << handler[2] << RST << " ";
  }
  std::cout << "\n";
  return true;
}

bool do_put(ArgumentHandler &handler) {
  // Remove fill words.
  handler.removeFillWords();

  // Check if the user provided the container.
  if (handler.size() != 3) {
    std::cerr << KRED << "You must provide the container.\n" << RST;
    return false;
  }
  // Error checking.
  if (handler[2].hasPrefixAll() || handler[2].hasQuantity()) {
    std::cerr << KRED << "[Arg. 2] You cannot specify a quantity.\n" << RST;
    return false;
  }

  // Run the command.
  std::cout << "You put";
  if (handler[1].hasPrefixAll()) {
    std::cout << " " << KMAG << "all" << RST;
  } else if (handler[1].hasQuantity()) {
    std::cout << " " << KMAG << handler[1].getQuantity() << RST << " per";
  } else if (handler[1].hasIndex()) {
    size_t index = handler[1].getIndex();
    std::cout << " the " << KMAG << index << ustr::get_ordinal(index) << RST;
  }
  std::cout << " " << KGRN << handler[1] << RST << " ";

  // Container.
  std::cout << "in";
  if (handler[2].hasIndex()) {
    size_t index = handler[2].getIndex();
    std::cout << " the " << KMAG << index << ustr::get_ordinal(index) << RST;
  }
  std::cout << " " << KGRN << handler[2] << RST << " ";
  std::cout << "\n";
  return true;
}

bool handle_input(const std::string &input) {
  // Create the interpreter.
  ArgumentHandler handler;
  // Parse the input.
  handler.parse(input);
  // Handle the input.
  if (handler[0] == "say")
    return do_say(handler);
  if (handler[0] == "look")
    return do_look(handler);
  if (handler[0] == "take")
    return do_take(handler);
  if (handler[0] == "put")
    return do_put(handler);
  return false;
}

void test_input(const std::string &input) {
  std::cout << "> " << input << "\n";
  handle_input(input);
  std::cout << "\n";
}

int main() {
  test_input("take pen");
  test_input("take 2*pen");
  test_input("take 2.pen");
  test_input("take all.pen");
  test_input("take all*pen");
  test_input("take pen box");
  test_input("take pen from box");
  test_input("take pen from the box");
  test_input("take 2*pen from 2.box");
  test_input("say how are you today?");
  test_input("say two quantities are in the golden ratio if ...");

#if 1

  std::cout << "\nProvide some imputs, type `" << FRED("quit")
            << "` to stop...\n\n";
  std::string input;
  while (input != "quit") {
    std::cout << "> ";
    std::getline(std::cin, input);
    handle_input(input);
    std::cout << "\n";
  }

#endif

  return 0;
}