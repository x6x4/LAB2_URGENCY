#pragma once
#include "../common/common.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>


class Tokenizer {

public: 
    std::string tokenize(std::string my_regex);
};


class AST {
    ASTdata data;

public:

    AST(char *regex_ptr) {
        data.root = parse_regex(regex_ptr).first;
        
        Node *augment = insert(nullptr, nullptr, '#');
        data.root = insert(data.root, augment, '_');
    }

    const ASTdata &getData () { return data; }

    void printAST();
    void printLeafMap();

private:

    Node *insert(Node* _left, Node* _right, char oper);

    std::pair<Node*,char*> parse_regex(char* start);
    std::pair<Node*, char*> parse_alt(char* start);
    std::pair<Node*, char*> parse_concat(char* start);
    std::pair<Node*, char*> parse_R(char* start);
};

