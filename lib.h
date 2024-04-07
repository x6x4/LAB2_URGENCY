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
	Node* left;
	Node* right;
	char op;

    Node(Node* _left, Node* _right, char oper) 
    : left(_left), right(_right), op(oper) {}
};

std::pair<Node*,char*> parse_regex(char* start);
std::pair<Node*, char*> parse_alt(char* start);
std::pair<Node*, char*> parse_concat(char* start);
std::pair<Node*, char*> parse_R(char* start);


void printBT_Base(Node *node, std::ofstream &file);
void printBT(Node *root);


