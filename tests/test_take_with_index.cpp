/// @file test_take_with_index.cpp
/// @brief Test for the "take" command with an index.
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#include "test_common.hpp"

int main()
{
    interpreter::Interpreter args;
    std::string input = "take 2.pen";
    std::string output;

    capture_stdout([&]() { test_input(args, input); }, output);

    if (output.find("You take") == std::string::npos || output.find("2.pen") == std::string::npos) {
        std::cerr << "Test failed!" << std::endl;
        return 1;
    }

    return 0;
}
