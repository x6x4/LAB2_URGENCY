
#include "lib.h"

std::string unary = "*?";
std::string binary = "|\0";


bool is_unary (char ch) {return unary.find(ch) != std::string::npos;}
bool is_binary (char ch) {return binary.find(ch) != std::string::npos;}

bool non_special(char ch) {
    return !is_unary(ch) && !is_binary(ch);
}