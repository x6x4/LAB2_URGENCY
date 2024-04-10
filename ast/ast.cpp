#include "ast.h"
#include <fstream>


std::string unary = "*?";
std::string binary = "|_";
std::string paren = "()";

bool is_unary (char ch) {return unary.find(ch) != std::string::npos;}
bool is_binary (char ch) {return binary.find(ch) != std::string::npos;}
bool is_paren (char ch) {return paren.find(ch) != std::string::npos;};

bool non_special(char ch) {
    return !is_unary(ch) && !is_binary(ch) && !is_paren(ch);
}

std::string Tokenizer::tokenize(std::string my_regex) {
    std::string tokenized;

    for (size_t i = 0; i < my_regex.length(); i++) {
        tokenized.push_back(my_regex[i]);
        
        if (i + 1 < my_regex.length() && (
            (non_special(my_regex[i]) && non_special(my_regex[i + 1])) ||
            (is_unary(my_regex[i]) && non_special(my_regex[i + 1])) ||
            (is_unary(my_regex[i]) && my_regex[i + 1] == '(') || 
            (non_special(my_regex[i]) && my_regex[i + 1] == '(') ||
            (my_regex[i] == ')' && non_special(my_regex[i + 1])) ||
            (my_regex[i] == ')' && my_regex[i + 1] == '(')
            )) {
            tokenized.push_back('_');
        }
    }

    return tokenized;
}


//  GRAMMAR - make AST

Node *AST::insert(Node* _left, Node* _right, char oper) {
    Node *newNode = new Node(_left, _right, oper, ++data.nodeCount);
    if (_left) _left->par = newNode;
    if (_right) _right->par = newNode;
    if (non_special(oper)) {
        newNode->true_num = ++data.leafCount;
    
        auto entry = data.leaf_map.find(oper);
        if (entry != data.leaf_map.end()) {
            entry->second.push_back(newNode->true_num);
        } else {
            data.leaf_map.insert({oper, {newNode->true_num}});
        }
    }
    return newNode;
}

//  return root
std::pair<Node*,char*> AST::parse_regex(char* start) {
	auto [left, s] = parse_alt(start);
	// Check
	if (*s == '|') {
		s++;
		auto [right, s2] = parse_regex(s);

		return {insert(left, right, '|'), s2};
	} else return {left, s};
}

std::pair<Node*, char*> AST::parse_alt(char* start) {
    auto [left, s] = parse_concat(start);
    
    while (*s == '_') {
        s++;  // Move past the concatenation symbol
        auto [right, s2] = parse_concat(s);
        left = insert(left, right, '_');
        s = s2;
    }

    return {left, s};
}

std::pair<Node*, char*> AST::parse_concat(char* start) {
    auto [left, s] = parse_R(start);
    if (*s == '*' || *s == '?') {
        char op = *s;
        s++;
        return {insert(left, nullptr, op), s};
    } else return {left, s};
}


std::pair<Node*, char*> AST::parse_R(char* start) {
    char* s = start;
    char c = *s;
    
    if (std::isprint(c) && non_special(c)) {
        s++; 
        return {insert(nullptr, nullptr, c), s};
    } else if (c == '(') {
        s++; 
        auto [expr, s2] = parse_regex(s);
        if (*s2 != ')') {
            throw std::runtime_error("Skobka gde pidor\n");
        } else {
            s = s2 + 1; 
            return {expr, s};
        }
    } else {
        throw std::runtime_error("Nu ty i nasral\n");
    }
}


//  DEBUG OUTPUT

void AST::printLeafMap() {
    std::cout << data.leaf_map << std::endl;
}

//  GRAPHVIZ

void printBT_Base(Node *node, std::ofstream &file);

void AST::printAST() {

    std::ofstream file("tree_output/tree_output.dot");
    if (!file) {
        std::cout << "Error creating file" << std::endl;
        return;
    }

    file << "digraph Tree {" << std::endl;
    file << "node [shape=circle];" << std::endl;

    printBT_Base(data.root, file);

    file << "}" << std::endl;

    file.close();

    system("dot -Tpng tree_output/tree_output.dot -o tree_output/tree_output.png");
}

void printBT_Base(Node *node, std::ofstream &file) {
    if (!node) { return;}

    file << reinterpret_cast<uintptr_t>(node) << " [label=\"";
    if (node->true_num) file << node->true_num << ' ';
    file << node->op << "\"];" << std::endl;

    if (node->left != nullptr) {
        file << reinterpret_cast<uintptr_t>(node) << " -> " << reinterpret_cast<uintptr_t>(node->left) << ";" << std::endl;
        printBT_Base(node->left, file);
    }

    if (node->right != nullptr) {
        file << reinterpret_cast<uintptr_t>(node) << " -> " << reinterpret_cast<uintptr_t>(node->right) << ";" << std::endl;
        printBT_Base(node->right, file);
    }
}
