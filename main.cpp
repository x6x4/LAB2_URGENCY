
#include "regex/regex.h"
#include <iostream>

int main() {
    std::string str, str_to_match;
    std::cin >> str;

    auto regex = Regex(str);
    while (1) {
        std::cin >> str_to_match;
        std::cout << regex.match(str_to_match) << std::endl;
    }
}