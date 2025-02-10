/// @file test_say_with_question.cpp
/// @brief Test for the "say" command with a question mark.
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#include "test_common.hpp"

int main()
{
    interpreter::Interpreter args;
    std::string input = "say How are you?";
    std::string output;

    capture_stdout([&]() { test_input(args, input); }, output);

    if (output.find("You ask") == std::string::npos || output.find("How are you?") == std::string::npos) {
        std::cerr << "Test failed!" << std::endl;
        return 1;
    }

    return 0;
}
