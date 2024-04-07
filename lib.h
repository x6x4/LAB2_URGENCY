#include <array>
#include <cstddef>
#include <functional>
#include <ostream>
#include <string>
#include <set>


extern std::string unary;
extern std::string binary;

bool is_unary (char ch);
bool is_binary (char ch);
bool non_special(char ch);

class TokenizedString {
    std::string str;
    std::vector<std::size_t> alt;

public:
    TokenizedString(std::string _str, std::vector<std::size_t> _alt) : str(_str), alt(_alt) {};
    
    friend std::ostream &operator<<(std::ostream &os, const TokenizedString &tstr) {
        os << tstr.str << ' ';
        for (auto num : tstr.alt)
            os << num << ' ';
        return os << '\n';
    }

};

class Tokenizer {

public: 
    TokenizedString tokenize(std::string my_regex) {

        std::vector<std::size_t> alt;
        std::size_t sum = 0;

        for (auto sIt = my_regex.begin(); sIt != my_regex.end(); sIt++) {
            if (sIt != my_regex.end() -1)
                if (non_special(*sIt) && non_special(*(sIt+1))) {
                    my_regex.insert(sIt+1, '_');
                    sIt++;
                    continue;
                }

            if (*sIt == '|') {
                std::size_t n = sIt - my_regex.begin();
                n = n - sum - alt.size();
                alt.push_back(n);
                sum+=n;
            }
        }

        TokenizedString tstr = {my_regex, alt};
        return tstr;
    }
};

class AST;

class AST_Node {
    friend AST;
    std::size_t key = 0;
    char val = 0;

    AST_Node(std::size_t _key, char _val) : key(_key), val(_val) {};

public:
    auto getKey() const {return key;} 

    friend std::ostream &operator<<(std::ostream &os, AST_Node node) {
        return os << node.key << ' ' << node.val << '\n';
    }

};
    
bool operator<(const AST_Node &x, const AST_Node &y);

class BT;

class BT_Node {

    friend BT;

    AST_Node val;
    BT_Node *left = nullptr;
    BT_Node *right = nullptr;

public:
    BT_Node(AST_Node _val) : val(_val) {};
};

class BT {
friend AST;
    BT_Node *root = nullptr;

public:
    void insert_r(BT_Node *cur, AST_Node _val) {
        if (!root) root = new BT_Node(_val);

        else if (cur) cur->right = new BT_Node(_val);
    }
    void insert_l(BT_Node *cur, AST_Node _val) {
        if (!root) root = new BT_Node(_val);

        else if (cur) cur->left = new BT_Node(_val);
    }
};


/*
(a|b_(c_d)*)?#
                  #
?
( a|b(cd)* )

a|b (cd)*

a b    cd
      c   d

a_(c_d)*

a|b_c
*/

class AST {

public:
    
    void make_tree (TokenizedString my_regex) {

       
    }


private:
    BT tree;
    std::size_t last_node = 0;

    

    void insert_r (BT_Node *cur, char New) {
        tree.insert_r(cur, (AST_Node(++last_node, New)));
    }
    void insert_l (BT_Node *cur, char New) {
        tree.insert_l(cur, (AST_Node(++last_node, New)));
    }

    friend std::ostream &operator<<(std::ostream &os, AST ast) {
        /*for (auto node : ast.tree)
            os << node;
        */
        return os;
    }
};






