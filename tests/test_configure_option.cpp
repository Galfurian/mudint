/// @file test_configure_option.cpp
/// @brief Test for the "configure" option functionality.
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#include "test_common.hpp"

int main()
{
    interpreter::Interpreter args;
    std::string input = "configure name";
    std::string output;

    capture_stdout([&]() { test_input(args, input); }, output);

    if (output.find("You selected name") == std::string::npos) {
        std::cerr << "Test failed!" << std::endl;
        return 1;
    }

    return 0;
}
