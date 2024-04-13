#pragma once
#include "../common/common.h"
#include <set>


using state = std::set<std::size_t>;
using tran_table = std::map<std::pair<std::size_t, char>, std::size_t>;
using fstates = std::set<std::size_t>;

class Regex;

class DFA {
friend Regex;
public:
    DFA(tran_table _Dtran, fstates _FStates) : Dtran(_Dtran), FStates(_FStates) {};

    void printDFA();

private:    
    tran_table Dtran;
    fstates FStates;

    void printDFA_Base(std::ofstream &file);
    
    friend std::ostream &operator<< (std::ostream &os, const DFA &dfa) {
        os << dfa.Dtran << '\n'; 
        return os;     
    };
};


using node_list = std::set<std::size_t>;

class DFA_sets {

public:
    DFA makeDFA(const ASTdata& data);

    DFA_sets(const ASTdata& data) {
        nullable = std::vector<bool>(data.nodeCount);
        firstpos = std::vector<node_list>(data.nodeCount);
        lastpos = std::vector<node_list>(data.nodeCount);
        followpos = std::vector<node_list>(data.leafCount);

        calculate(data.root);
    }

private:

    void calculate(Node *_root);

    std::vector<bool> nullable;
    void nullable_traversal (Node *start);
    std::vector<node_list> firstpos;
    void firstpos_traversal (Node *start);
    std::vector<node_list> lastpos;
    void lastpos_traversal (Node *start);
    std::vector<node_list> followpos;
    void followpos_traversal (Node *start);

    friend std::ostream &operator<< (std::ostream &os, const DFA_sets &sets);
};

