#pragma once
#include "../common/common.h"


using state = std::vector<std::size_t>;
using tran_table = std::map<std::pair<state, char>, state>;

class DFA {

public:
    DFA(std::vector<state> _Dstate, tran_table _Dtran) :
        Dstate(_Dstate), Dtran(_Dtran) {};

private:    
    std::vector<state> Dstate;
    tran_table Dtran;
    
    friend std::ostream &operator<< (std::ostream &os, const DFA &dfa) {
        os << dfa.Dstate << '\n';
        os << dfa.Dtran << '\n'; 
        return os;     
    };
};


using node_list = std::vector<std::size_t>;

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
    void firstpos_traversal (Node *start, Node *_root);
    std::vector<node_list> lastpos;
    void lastpos_traversal (Node *start, Node *_root);
    std::vector<node_list> followpos;
    void followpos_traversal (Node *start, Node *_root);

    friend std::ostream &operator<< (std::ostream &os, const DFA_sets &sets);
};

