
#include "lib.h"
#include <cstddef>
#include <stdexcept>
#include <vector>



Node *get_leftmost (Node *_root);

void AST::middle_work() {

    Node *leftmost = get_leftmost(root);
    nullable_traversal(leftmost);

    try { firstpos_traversal(leftmost); }
    catch (std::runtime_error &err) {}
    try { lastpos_traversal(leftmost); }
    catch (std::runtime_error &err) {}
    try { followpos_traversal(leftmost); }
    catch (std::runtime_error &err) {}

    for (auto entry : nullable) 
        std::cout << entry;
    std::cout << std::endl << std::endl;

    for (auto entry : firstpos) {
        for (auto pos : entry) {
            std::cout << pos << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (auto entry1 : lastpos) {
        for (auto pos1 : entry1) {
            std::cout << pos1 << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (auto entry1 : followpos) {
        for (auto pos1 : entry1) {
            std::cout << pos1 << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

Node *get_leftmost (Node *_root) {
    Node *node = _root;
    Node *prev = node;
    
    while (node) 
        {prev = node; node = node->left;}

    return prev;
}

bool is_leaf(Node *node) { return !(node->left) && !(node->right); }

void AST::nullable_traversal (Node *start) {

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

Node *get_nonnull_child (Node *par) {
    if (par->left) return par->left;
    if (par->right) return par->right;
    return nullptr;
}

void AST::firstpos_traversal (Node *start) {

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

    if (start == root) {
        throw std::runtime_error("выходим");
    }

    if (start->par && start == start->par->left) 
        firstpos_traversal(get_leftmost(start->par->right));

    firstpos_traversal(start->par);
}

void AST::lastpos_traversal (Node *start) {

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

    if (start == root) {
        throw std::runtime_error("выходим");
    }

    if (start->par && start == start->par->left) 
        lastpos_traversal(get_leftmost(start->par->right));

    lastpos_traversal(start->par);
}

void AST::followpos_traversal (Node *start) {

    if (!start) return; 

    else if (start->op == '_') {
        for (auto entry : lastpos.at(start->left->num-1))
            followpos.at(entry-1) += firstpos.at(start->right->num-1);
    }

    else if (start->op == '*') {
        for (auto entry : lastpos.at(start->num-1))
            followpos.at(entry-1) += firstpos.at(start->num-1);
    }

    if (start == root) {
        throw std::runtime_error("выходим");
    }

    if (start->par && start == start->par->left) 
        followpos_traversal(get_leftmost(start->par->right));

    followpos_traversal(start->par);
}


//  GRAMMAR - make AST

std::string unary = "*?";
std::string binary = "|_";
std::string paren = "()";


bool is_unary (char ch) {return unary.find(ch) != std::string::npos;}
bool is_binary (char ch) {return binary.find(ch) != std::string::npos;}
bool is_paren (char ch) {return paren.find(ch) != std::string::npos;};

bool non_special(char ch) {
    return !is_unary(ch) && !is_binary(ch) && !is_paren(ch);
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
    
    // Parse any printable ASCII character
    if (std::isprint(c) && non_special(c)) {
        s++; // Move past the parsed character
        return {insert(nullptr, nullptr, c), s};
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


//  GRAPHVIZ

void printBT_Base(Node *node, std::ofstream &file);

void AST::printBT() {

    std::cout << nodeCount << std::endl;

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
