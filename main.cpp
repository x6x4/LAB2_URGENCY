
#include "regex/regex.h"
#include <iostream>

int main() {
    std::string str;
    std::cin >> str;

    auto regex = Regex(str);
    //std::cout << regex.match("abc") << std::endl;
}