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
bool non_special(char ch);


class Tokenizer {

public: 
    std::string tokenize(std::string my_regex) {

        std::vector<std::size_t> alt;
        std::size_t sum = 0;

        for (auto sIt = my_regex.begin(); sIt != my_regex.end(); sIt++) {
            if (sIt != my_regex.end() -1)
                if (non_special(*sIt) && non_special(*(sIt+1))) {
                    my_regex.insert(sIt+1, '_');
                    sIt++;
                    continue;
                }
        }

        return my_regex;
    }
};

class AST;

class AST_Node {
    friend AST;
    std::size_t key = 0;
    char val = 0;

public:
    AST_Node(std::size_t _key, char _val) : key(_key), val(_val) {};

    auto getKey() const {return key;} 

    friend std::ostream &operator<<(std::ostream &os, AST_Node node) {
        return os << node.key << ' ' << node.val;
    }

};
    
bool operator<(const AST_Node &x, const AST_Node &y);

class BT;

struct BT_Node {

    //friend BT;

    AST_Node val;
    BT_Node *left = nullptr;
    BT_Node *right = nullptr;

public:
    BT_Node(AST_Node _val) : val(_val) {};
};

class BT {
friend AST;
    BT_Node *root = new BT_Node({0, '_'});

public:
    void insert_r(BT_Node **cur, AST_Node _val) {
        if (!(*cur)) *cur = new BT_Node(_val);

        else if (*cur) (*cur)->right = new BT_Node(_val);
    }
    void insert_l(BT_Node **cur, AST_Node _val) {
        if (!(*cur)) *cur = new BT_Node(_val);

        else if (*cur) (*cur)->left = new BT_Node(_val);
    }

    void printBT() {
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

    void printBT_Base(BT_Node *node, std::ofstream &file) {
        if (!node) { return;}

        file << reinterpret_cast<uintptr_t>(node) << " [label=\"" << node->val << "\"];" << std::endl;

        if (node->left != nullptr) {
            file << reinterpret_cast<uintptr_t>(node) << " -> " << reinterpret_cast<uintptr_t>(node->left) << ";" << std::endl;
            printBT_Base(node->left, file);
        }

        if (node->right != nullptr) {
            file << reinterpret_cast<uintptr_t>(node) << " -> " << reinterpret_cast<uintptr_t>(node->right) << ";" << std::endl;
            printBT_Base(node->right, file);
        }
    }
};



class AST {

public:
    
    void make_tree (std::string my_regex) {
        make_nonaugmented_tree(my_regex);
        insert_r(&tree.root, '#');
    }

    void make_nonaugmented_tree(std::string my_regex) {
        static BT_Node** cur = &tree.root;
        static BT_Node** par = cur;
        static BT_Node** prev_cur = cur;
        auto pos = my_regex.find('|');
        if (pos != std::string::npos) {

            insert_l(cur, '|');
            par = cur;

            cur = &((*cur)->left);
            prev_cur = cur;
            make_nonaugmented_tree(my_regex.substr(0, pos));
            cur = prev_cur;
            make_nonaugmented_tree(my_regex.substr(pos+1));
            return;
        }
        
        if ((*par)->left)
            insert_r(cur, my_regex[0]);
        else 
            insert_l(cur, my_regex[0]);
    }


private:
    BT tree;
    std::size_t last_node = 0;

    

    void insert_r (BT_Node **cur, char New) {
        tree.insert_r(cur, (AST_Node(++last_node, New)));
    }
    void insert_l (BT_Node **cur, char New) {
        tree.insert_l(cur, (AST_Node(++last_node, New)));
    }

    friend std::ostream &operator<<(std::ostream &os, AST ast) {
        ast.tree.printBT();
        return os;
    }
};






