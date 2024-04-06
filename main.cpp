
#include "lib.h"
#include <iostream>

int main() {

    AST tree;
    //tree.make_tree("(a|b(cd)*)?#");
    tree.make_tree("(d)?");
    std::cout << tree;
}