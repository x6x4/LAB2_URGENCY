#include "dfa_sets.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

using dstates = std::map<state, std::size_t>;

std::pair<bool, std::size_t> dstates_find 
    (const dstates& _dstates, const state& key) {
        for (const auto &d : _dstates) {
            if (d.first == key) return {true, d.second};
        }
        return {false, 0};
}

DFA DFA_sets::makeDFA(const ASTdata& data) {

    state s0 = firstpos.at(data.root->num-1);
    
    std::map<state, std::size_t> unmarked = {{s0, 0}};
    std::map<state, std::size_t> marked;

    tran_table Dtran;
    std::size_t cur_state = 0;
    fstates Fstates;
    std::cout << "Followpos:\n" << followpos << std::endl;
    
    while (!unmarked.empty()) {
        auto cur_state_entry = *(unmarked.begin());
        if (cur_state_entry.first.size() > 10) break;
        marked.insert(cur_state_entry);
        unmarked.erase(cur_state_entry.first);
        
        std::cout << "T:" << cur_state_entry.first << std::endl;

        for (const auto &cur_char_leaves : data.char_map) {
            state U;
            std::cout << "Char leaves:" << cur_char_leaves;
            for (const auto &cur_leave : cur_state_entry.first) {
                if (cur_char_leaves.second.find(cur_leave) != cur_char_leaves.second.end()) {
                    U += followpos.at(cur_leave-1);
                }
            }
            std::cout << "U:" << U;

            if (!U.empty()) {

                auto U_entry = marked.find(U);

                if (U_entry == marked.end())
                    U_entry = unmarked.insert({U, ++cur_state}).first;

                if (cur_state_entry.first.find(data.leafCount) != cur_state_entry.first.end())
                    Fstates.insert(cur_state_entry.second);
                if (U.find(data.leafCount) != U.end())
                    Fstates.insert(cur_state);

                std::cout << "F:" << Fstates;
                
                Dtran.insert({{cur_state_entry.second, cur_char_leaves.first}, 
                    U_entry->second});
            }
        }
    }

    std::cout << std::endl << Dtran;

    return DFA(Dtran, Fstates);
}


void DFA_sets::calculate(Node *_root) {
    Node *leftmost = get_leftmost(_root);
    nullable_traversal(leftmost);

    try { firstpos_traversal(leftmost); }
    catch (std::runtime_error &err) {}
    try { lastpos_traversal(leftmost); }
    catch (std::runtime_error &err) {}
    try { followpos_traversal(leftmost); }
    catch (std::runtime_error &err) {}
}

void DFA_sets::nullable_traversal (Node *start) {

    if (!start) return; 
    if (is_leaf(start)) nullable.at(start->num-1) = false;
    else if (start->op == '*') nullable.at(start->num-1) = true;
    else if (start->op == '|') nullable.at(start->num-1) = 
        nullable.at(start->left->num-1) || nullable.at(start->right->num-1);
    else if (start->op == '_') nullable.at(start->num-1) = 
        nullable.at(start->left->num-1) && nullable.at(start->right->num-1);

    if (start->par && start == start->par->left) 
        nullable_traversal(get_leftmost(start->par->right));

    nullable_traversal(start->par);
}

void DFA_sets::firstpos_traversal (Node *start) {

    if (!start) return; 

    if (is_leaf(start)) firstpos.at(start->num-1).insert(start->true_num);

    else if (start->op == '*') {
        Node* child = get_nonnull_child(start);
        firstpos.at(start->num-1) = firstpos.at(child->num - 1);
    }

    else if (start->op == '|') 
        firstpos.at(start->num-1) = 
            firstpos.at(start->left->num-1) + firstpos.at(start->right->num-1);

    else if (start->op == '_') {

        if (nullable.at(start->left->num-1))
            firstpos.at(start->num-1) = 
                firstpos.at(start->left->num-1) + firstpos.at(start->right->num-1);
        else 
            firstpos.at(start->num-1) = firstpos.at(start->left->num-1);
    }

    if (start->par && start == start->par->left) 
        firstpos_traversal(get_leftmost(start->par->right));

    firstpos_traversal(start->par);
}

void DFA_sets::lastpos_traversal (Node *start) {

    if (!start) return; 

    if (is_leaf(start)) lastpos.at(start->num-1).insert(start->true_num);

    else if (start->op == '*') {
        Node* child = get_nonnull_child(start);
        lastpos.at(start->num-1) = lastpos.at(child->num - 1);
    }

    else if (start->op == '|') lastpos.at(start->num-1) = 
        lastpos.at(start->left->num-1) + lastpos.at(start->right->num-1);

    else if (start->op == '_') {

        if (nullable.at(start->right->num-1))
            lastpos.at(start->num-1) = 
                lastpos.at(start->left->num-1) + lastpos.at(start->right->num-1);
        else 
            lastpos.at(start->num-1) = lastpos.at(start->right->num-1);
    }

    if (start->par && start == start->par->left) 
        lastpos_traversal(get_leftmost(start->par->right));

    lastpos_traversal(start->par);
}

void DFA_sets::followpos_traversal (Node *start) {

    if (!start) return; 

    else if (start->op == '_') {
        for (auto entry : lastpos.at(start->left->num-1))
            followpos.at(entry-1) += firstpos.at(start->right->num-1);
    }

    else if (start->op == '*') {
        for (auto entry : lastpos.at(start->num-1))
            followpos.at(entry-1) += firstpos.at(start->num-1);
    }

    if (start->par && start == start->par->left) 
        followpos_traversal(get_leftmost(start->par->right));

    followpos_traversal(start->par);
}


std::ostream &operator<< (std::ostream &os, const DFA_sets &sets) {
    os << "Nullable:\n";
    os << sets.nullable << std::endl;

    os << "Firstpos:\n";
    os << sets.firstpos;

    os << "Lastpos:\n";
    os << sets.lastpos;

    os << "Followpos:\n";
    os << sets.followpos;

    return os;
}



void DFA::printDFA() {
    std::ofstream file("dfa_output.dot");
    if (!file) {
        std::cout << "Error creating file" << std::endl;
        return;
    }

    file << "digraph DFA {" << std::endl;
    file << "rankdir=\"LR\"" << std::endl;
    file << "node [shape=circle];" << std::endl;

    printDFA_Base(file);

    file << "}" << std::endl;

    file.close();

    system("dot -Tpng dfa_output.dot -o dfa_output.png");
}

void DFA::printDFA_Base(std::ofstream &file) {


    for (const auto &entry : Dtran) {
        file << entry.first.first << " -> " << entry.second;
        file << " [label=\"" << entry.first.second 
            << "\"]" << ";" << std::endl;
    }

    for (const auto &entry : FStates) 
        file << entry << "[label=\"" << entry << "\"" 
        << "shape=doublecircle" << "]" << ";" << std::endl;



    
}
