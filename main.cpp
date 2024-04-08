
#include "lib.h"
#include <iostream>



int main() {

    Tokenizer t;
    std::string str = t.tokenize("(a|b)*abb");
    //std::string str = t.tokenize("ah|g*|be");
    std::cout << str << std::endl;
    AST tree (str);
    tree.printBT();
    tree.middle_work();
}