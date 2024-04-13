#pragma once
#include <cstddef>
#include <string>
#include "../ast/ast.h"
#include "../dfa_sets/dfa_sets.h"

class Regex {

DFA dfa;

public:

    Regex(const std::string &regex) : dfa (compile(regex)) {}

    DFA compile(const std::string &regex) {
        std::string regex_str = Tokenizer().tokenize(regex);

        AST ast(regex_str.begin().base());
        ast.printAST();
        ast.printLeafMap();
        DFA_sets sets(ast.getData());
        
        DFA _dfa = sets.makeDFA(ast.getData());
        _dfa.printDFA();
        return _dfa;
    }

    //  method dfa - transition by symbol, return cur_state
    //  check for end state

    bool match(const std::string &regex) {

        auto cur = regex.begin();
        std::size_t cur_state = 0;

        //  iterative for 
        while (cur != regex.end()) {
            auto cur_tran = dfa.Dtran.find({cur_state, *cur});
            if (cur_tran == dfa.Dtran.end()) return false;
            cur_state = cur_tran->second;
            cur++;
        }
        if (dfa.FStates.find(cur_state) != dfa.FStates.end()) return true;

        return false;
    }
};