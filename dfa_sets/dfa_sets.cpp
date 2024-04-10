#include "dfa_sets.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

using dstates = std::vector<std::pair<state, std::size_t>>;

std::pair<bool, std::size_t> dstates_find (const dstates& _dstates, const state& key) {
    for (const auto &d : _dstates) {
        if (d.first == key) return {true, d.second};
    }
    return {false, 0};
}

DFA DFA_sets::makeDFA(const ASTdata& data) {

    state s0 = firstpos.at(data.root->num-1);
    
    std::vector<std::pair<state, std::size_t>> unmarked = {{s0, 0}};
    std::vector<std::pair<state, std::size_t>> marked = {{s0, 0}};

    tran_table Dtran;
    std::size_t cur_state = 0;
    fstates Fstates;
    std::cout << "Followpos:\n" << followpos << std::endl;
    
    while (!unmarked.empty()) {
        auto T = *(unmarked.end()-1);
        marked.push_back(T);
        unmarked.pop_back();
        
        std::cout << "T:" << T.first << std::endl;

        for (const auto &leaf : data.leaf_map) {
            state U;
            for (const auto &pos : T.first) {
                if (vec_find(leaf.second, pos)) {
                    U += followpos.at(pos-1);
                }
            }
            std::cout << "U:" << U;

            if (!U.empty()) {

                std::cout << "F:" << Fstates;

                auto U_entry = dstates_find(unmarked, U);
                auto U_entry2 = dstates_find(marked, U);
                auto U_entry3 = U_entry2.first ? U_entry2 : U_entry;
                if (!U_entry3.first) {
                    unmarked.push_back({U, ++cur_state});
                    U_entry3.second = cur_state;
                }

                if (vec_find(T.first, data.leafCount))
                    Fstates.push_back(T.second);
                if (vec_find(U, data.leafCount))
                    Fstates.push_back(cur_state);
                
                Dtran.insert({{T.second, leaf.first}, 
                    U_entry3.second});
            }
        }
    }

    std::cout << std::endl << Dtran;

    return DFA(Dtran, Fstates);
}


void DFA_sets::calculate(Node *_root) {
    Node *leftmost = get_leftmost(_root);
    nullable_traversal(leftmost);

    try { firstpos_traversal(leftmost, _root); }
    catch (std::runtime_error &err) {}
    try { lastpos_traversal(leftmost, _root); }
    catch (std::runtime_error &err) {}
    try { followpos_traversal(leftmost, _root); }
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

    return;
}

void DFA_sets::firstpos_traversal (Node *start, Node *_root) {

    if (!start) return; 

    if (is_leaf(start)) firstpos.at(start->num-1).push_back(start->true_num);

    else if (start->op == '*') {
        Node* child = get_nonnull_child(start);
        firstpos.at(start->num-1) = firstpos.at(child->num - 1);
    }

    else if (start->op == '|') firstpos.at(start->num-1) = 
        firstpos.at(start->left->num-1) + firstpos.at(start->right->num-1);

    else if (start->op == '_') {

        if (nullable.at(start->left->num-1))
            firstpos.at(start->num-1) = 
                firstpos.at(start->left->num-1) + firstpos.at(start->right->num-1);
        else 
            firstpos.at(start->num-1) = firstpos.at(start->left->num-1);
    }

    if (start == _root) {
        throw std::runtime_error("выходим");
    }

    if (start->par && start == start->par->left) 
        firstpos_traversal(get_leftmost(start->par->right), _root);

    firstpos_traversal(start->par, _root);
}

void DFA_sets::lastpos_traversal (Node *start, Node *_root) {

    if (!start) return; 

    if (is_leaf(start)) lastpos.at(start->num-1).push_back(start->true_num);

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

    if (start == _root) {
        throw std::runtime_error("выходим");
    }

    if (start->par && start == start->par->left) 
        lastpos_traversal(get_leftmost(start->par->right), _root);

    lastpos_traversal(start->par, _root);
}

void DFA_sets::followpos_traversal (Node *start, Node *_root) {

    if (!start) return; 

    else if (start->op == '_') {
        for (auto entry : lastpos.at(start->left->num-1))
            followpos.at(entry-1) += firstpos.at(start->right->num-1);
    }

    else if (start->op == '*') {
        for (auto entry : lastpos.at(start->num-1))
            followpos.at(entry-1) += firstpos.at(start->num-1);
    }

    if (start == _root) {
        throw std::runtime_error("выходим");
    }

    if (start->par && start == start->par->left) 
        followpos_traversal(get_leftmost(start->par->right), _root);

    followpos_traversal(start->par, _root);
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
