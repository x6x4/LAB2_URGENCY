
#include "lib.h"
#include <stdexcept>

std::string unary = "*?";
std::string binary = "|_";
std::string paren = "()";


bool is_unary (char ch) {return unary.find(ch) != std::string::npos;}
bool is_binary (char ch) {return binary.find(ch) != std::string::npos;}
bool is_paren (char ch) {return paren.find(ch) != std::string::npos;};

bool non_special(char ch) {
    return !is_unary(ch) && !is_binary(ch) && !is_paren(ch);
}

void printBT(Node *root) {
    std::ofstream file("tree_output.dot");
    if (!file) {
        std::cout << "Error creating file" << std::endl;
        return;
    }

    file << "digraph Tree {" << std::endl;
    file << "node [shape=circle];" << std::endl;

    printBT_Base(root, file);

    file << "}" << std::endl;

    file.close();

    system("dot -Tpng tree_output.dot -o tree_output.png");
}

void printBT_Base(Node *node, std::ofstream &file) {
    if (!node) { return;}

    file << reinterpret_cast<uintptr_t>(node) << " [label=\"" << node->op << "\"];" << std::endl;

    if (node->left != nullptr) {
        file << reinterpret_cast<uintptr_t>(node) << " -> " << reinterpret_cast<uintptr_t>(node->left) << ";" << std::endl;
        printBT_Base(node->left, file);
    }

    if (node->right != nullptr) {
        file << reinterpret_cast<uintptr_t>(node) << " -> " << reinterpret_cast<uintptr_t>(node->right) << ";" << std::endl;
        printBT_Base(node->right, file);
    }
}

//  return root
std::pair<Node*,char*> parse_regex(char* start) {
	auto [left, s] = parse_alt(start);
	// Check
	if (*s == '|') {
		s++;
		auto [right, s2] = parse_regex(s);

		return {new Node(left, right, '|'), s2};
	} else return {left, s};
}

std::pair<Node*, char*> parse_alt(char* start) {
    auto [left, s] = parse_concat(start);
    
    while (*s == '_') {
        s++;  // Move past the concatenation symbol
        auto [right, s2] = parse_concat(s);
        left = new Node(left, right, '_');
        s = s2;
    }

    return {left, s};
}

std::pair<Node*, char*> parse_concat(char* start) {
    auto [left, s] = parse_R(start);
    if (*s == '*' || *s == '?') {
        char op = *s;
        s++;
        return {new Node(left, nullptr, op), s};
    } else return {left, s};
}


std::pair<Node*, char*> parse_R(char* start) {
    char* s = start;
    char c = *s;
    
    // Parse any printable ASCII character
    if (std::isprint(c) && non_special(c)) {
        s++; // Move past the parsed character
        return {new Node(nullptr, nullptr, c), s};
    } else if (c == '(') {
        // Parse expression within parentheses
        s++; // Skip '('
        auto [expr, s2] = parse_regex(s);
        if (*s2 != ')') {
            // Handle error for missing closing parenthesis
            throw std::runtime_error("Skobka gde pidor\n");
        } else {
            s = s2 + 1; // Skip ')'
            return {expr, s};
        }
    } else {
        // Handle error for invalid character
        throw std::runtime_error("Nu ty i nasral\n");
    }
}