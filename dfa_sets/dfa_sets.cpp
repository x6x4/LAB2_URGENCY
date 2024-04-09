#include "dfa_sets.h"
#include <algorithm>

DFA DFA_sets::makeDFA(const ASTdata& data) {

    state s0 = firstpos.at(data.root->num-1);
    std::map<state, bool> Dstates = {{s0, false}};
    tran_table Dtran;
    
    for (auto &T : Dstates) {
        if (!T.second) {
            T.second = true;

            for (const auto &char_entry : data.leaf_map) {
                state U;
                for (const auto &pos : T.first) {
                    if (std::find(char_entry.second.begin(), 
                        char_entry.second.end(), pos)!= char_entry.second.end()) {
                            U += followpos.at(pos-1);
                    }
                }

                if (!U.empty()) {
                    if (Dstates.find(U) == Dstates.end()) {
                        Dstates.insert({U, false});
                    }
                    Dtran.insert({{T.first, char_entry.first}, U});
                }
            }
        }
    }

    std::vector<state> Dstate;
    for (const auto &e : Dstates) 
        Dstate.push_back(e.first);

    return DFA(Dstate, Dtran);
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
    os << sets.firstpos << std::endl;

    os << "Lastpos:\n";
    os << sets.lastpos << std::endl;

    os << "Followpos:\n";
    os << sets.followpos << std::endl;

    return os;
}
