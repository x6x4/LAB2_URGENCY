
#include "regex/regex.h"
#include <iostream>

int main() {
    std::string str;
    std::cin >> str;

    Regex().compile(str);
}