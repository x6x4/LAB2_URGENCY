#include <cstddef>
#include <functional>
#include <ostream>
#include <string>
#include <set>

class AST;

class Node {
    friend AST;
    std::size_t key = 0;
    char val = 0;

    Node(std::size_t _key, char _val) : key(_key), val(_val) {};

public:
    auto getKey() const {return key;} 

    friend std::ostream &operator<<(std::ostream &os, Node node) {
        return os << node.key << ' ' << node.val << '\n';
    }

};
    
bool operator<(const Node &x, const Node &y);

/*
(a|b(cd)*)?#
                  #
?
( a|b(cd)* )

a|b (cd)*

a b    cd
      c   d

*/
class AST {

public:
    
    void make_tree (std::string my_regex) {

        for (auto my_it = my_regex.end() - 1; *my_it; my_it++) {

            if (*my_it == ')' || *my_it == '(') continue;

            if (unary.find(*my_it) != std::string::npos) 
                insert(*my_it);
            else if (binary.find(*my_it) != std::string::npos)
                ;
            else 
                ;
        }

        //  in the end
        insert('#');
    }


private:
    std::set<Node, std::less<Node>> tree;
    std::size_t last_node = 0;

    std::string unary = "*?";
    std::string binary = "|\0";
    

    void insert (char New) {
        tree.insert(Node(++last_node, New));
    }

    friend std::ostream &operator<<(std::ostream &os, AST ast) {
        for (auto node : ast.tree)
            os << node;

        return os;
    }
};






