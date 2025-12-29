#include "CliUtil.hpp"
#include <iostream>
#include <sstream>

namespace cli {

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

int readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);

        std::stringstream ss(line);
        int x;
        char extra;
        if (ss >> x && !(ss >> extra)) return x;

        std::cout << "Invalid number. Try again.\n";
    }
}

} // namespace cli
