
#include "lib.h"
#include <iostream>



int main() {

    Tokenizer t;
    //std::cout << t.tokenize("cba|bc*e|f|k");
    std::string str = t.tokenize("");
    std::cout << str << std::endl;
    auto pair = parse_regex(str.begin().base());
    printBT(pair.first);
    //tree.make_tree("(d)?");
    //std::cout << tree;
}