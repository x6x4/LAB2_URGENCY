
#include "lib.h"
#include <iostream>



int main() {

    Tokenizer t;
    std::cout << t.tokenize("cba|bc*e|f|k");

    //AST tree;
    //tree.make_tree("(a|b(cd)*)?#");
    //tree.make_tree("(d)?");
    //std::cout << tree;
}