#pragma once
#include <cstddef>
#include <map>
#include <vector>
#include <ostream>
#include <algorithm>

using leafMap = std::map<char, std::vector<std::size_t>>;

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
    leafMap leaf_map;
};


//  USEFUL FUNCTIONS

template<typename T> 
std::vector<T> operator+ 
(std::vector<T> v1, std::vector<T> v2) {
    v1.insert(v1.end(), v2.begin(), v2.end());
    return v1;
}

template<typename T> 
void operator+= 
(std::vector<T> &v1, std::vector<T> &v2) {
    v1 = v1+v2;
}

template<typename T> 
std::ostream &operator<<
(std::ostream &os, const std::vector<T> &vec) {
    os << ' ';
    for (const T &e : vec) {
        os << e << ' ';
    }
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

template<typename T>
bool vec_find (const std::vector<T>& vec, const T& key) {
    return (std::find(vec.begin(), vec.end(), key) != vec.end());
}

