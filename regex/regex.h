#pragma once
#include <string>
#include "../ast/ast.h"
#include "../dfa_sets/dfa_sets.h"

class Regex {

public:
    void compile(std::string regex) {
        std::string regex_str = Tokenizer().tokenize(regex);

        AST ast(regex_str.begin().base());
        ast.printAST();
        ast.printLeafMap();
        DFA_sets sets(ast.getData());
        
        DFA dfa = sets.makeDFA(ast.getData());
        dfa.printDFA();
    }
};