#include <iostream>
#include <string>
#include <exception>

void throwInteger() {
    throw 404;
}

void throwString() {
    throw std::string("Custom string exception");
}

int main() {
    std::cout << "Integer exception:\n";

    try {
        throwInteger();
    }
    catch (const std::exception& error) {
        std::cout << "std::exception caught: "
            << error.what() << '\n';
    }
    catch (...) {
        std::cout << "Unknown exception caught\n";
    }

    std::cout << "\nString exception:\n";

    try {
        throwString();
    }
    catch (const std::exception& error) {
        std::cout << "std::exception caught: "
            << error.what() << '\n';
    }
    catch (...) {
        std::cout << "Unknown exception caught\n";
    }

    return 0;
}