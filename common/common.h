#pragma once
#include <cstddef>
#include <map>
#include <set>
#include <vector>
#include <ostream>
#include <algorithm>

using leafMap = std::map<char, std::set<std::size_t>>;

struct Node {
	Node* left = nullptr;
	Node* right = nullptr;
    Node* par = nullptr;
	char op;
    int num = 0;
    std::size_t true_num = 0;

    Node(Node* _left, Node* _right, char oper, std::size_t _num) 
    : left(_left), right(_right), op(oper), num(_num) {}
};

Node *get_leftmost (Node *_root);
bool is_leaf(Node *node);
Node *get_nonnull_child (Node *par);

struct ASTdata {
    Node *root = nullptr;
    std::size_t nodeCount = 0;
    std::size_t leafCount = 0;
    leafMap char_map;
};


//  USEFUL FUNCTIONS

template<typename T> 
std::set<T> operator+ 
(std::set<T> s1, std::set<T> s2) {
    s1.insert(s2.begin(), s2.end());
    return s1;
}

template<typename T> 
void operator+= 
(std::set<T> &s1, std::set<T> &s2) {
    s1 = s1+s2;
}

template<typename T> 
std::ostream &operator<<
(std::ostream &os, const std::set<T> &set) {
    os << ' ';
    for (const T &e : set) {
        os << e << ' ';
    }
    if (!set.size()) os << "NULL";
    return os << '\n';
}

template<typename T> 
std::ostream &operator<<
(std::ostream &os, const std::vector<T> &vec) {
    os << ' ';
    for (const T &e : vec) {
        os << e << ' ';
    }
    if (!vec.size()) os << "NULL";
    return os << '\n';
}


template<typename T1, typename T2>
std::ostream &operator<<
(std::ostream &os, const std::pair<T1, T2> &para) {
    return os << para.first << ' ' <<para.second;
}

template<typename T1, typename T2> 
std::ostream &operator<<
(std::ostream &os, const std::map<T1, T2> &mapa) {
    os << ' ';
    for (const auto &e : mapa) {
        os << e.first << ' ' << e.second << '\n';
    }
    return os << '\n';
}

