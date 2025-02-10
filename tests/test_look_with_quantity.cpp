/// @file test_look_with_quantity.cpp
/// @brief Test for the "look" command with quantity.
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#include "test_common.hpp"

int main()
{
    interpreter::Interpreter args;
    std::string input = "look 2*box";
    std::string output;
    std::string error;

    capture_stdout([&]() { test_input(args, input); }, output);
    capture_stdout([&]() { test_input(args, input); }, error);

    if (error.find("You cannot specify a quantity.") == std::string::npos) {
        std::cerr << "Test failed!" << std::endl;
        return 1;
    }

    return 0;
}
