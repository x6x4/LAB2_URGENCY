#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <set>


extern std::string unary;
extern std::string binary;

bool is_unary (char ch);
bool is_binary (char ch);
bool is_paren (char ch);
bool non_special(char ch);


class Tokenizer {

public: 
    std::string tokenize(std::string my_regex) {
        std::string tokenized;

        for (size_t i = 0; i < my_regex.length(); i++) {
            tokenized.push_back(my_regex[i]);
            
            if (i + 1 < my_regex.length() &&
                ((non_special(my_regex[i]) && non_special(my_regex[i + 1])) ||
                (is_unary(my_regex[i]) && non_special(my_regex[i + 1])))) {
                tokenized.push_back('_');
            }
        }

        return tokenized;
    }
};


struct Node {
	Node* left = nullptr;
	Node* right = nullptr;
    Node* par = nullptr;
	char op;
    int num = 0;
    int true_num = 0;

    Node(Node* _left, Node* _right, char oper, std::size_t _num) 
    : left(_left), right(_right), op(oper), num(_num) {}
};

using node_list = std::vector<std::size_t>;

class AST {
    Node *root = nullptr;
    std::size_t nodeCount = 0;
    std::size_t leafCount = 0;

public:

    AST(std::string regex) {
        root = parse_regex(regex.begin().base()).first;
        Node *augment = insert(nullptr, nullptr, '#');
        root = insert(root, augment, '_');
        nullable = std::vector<bool>(nodeCount);
        firstpos = std::vector<node_list>(nodeCount);
        lastpos = std::vector<node_list>(nodeCount);
        followpos = std::vector<node_list>(leafCount);
    }
        
    void printBT();
    void middle_work();

private:

    std::vector<bool> nullable;
    void nullable_traversal (Node *start);
    std::vector<node_list> firstpos;
    void firstpos_traversal (Node *start);
    std::vector<node_list> lastpos;
    void lastpos_traversal (Node *start);
    std::vector<node_list> followpos;
    void followpos_traversal (Node *start);

    Node *insert(Node* _left, Node* _right, char oper) {
        Node *newNode = new Node(_left, _right, oper, ++nodeCount);
        if (_left) _left->par = newNode;
        if (_right) _right->par = newNode;
        if (non_special(oper)) 
            newNode->true_num = ++leafCount;
        return newNode;
    }

    std::pair<Node*,char*> parse_regex(char* start);
    std::pair<Node*, char*> parse_alt(char* start);
    std::pair<Node*, char*> parse_concat(char* start);
    std::pair<Node*, char*> parse_R(char* start);
};

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
