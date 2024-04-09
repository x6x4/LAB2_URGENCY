#include "common.h"


Node *get_leftmost (Node *_root) {
    Node *node = _root;
    Node *prev = node;
    
    while (node) 
        {prev = node; node = node->left;}

    return prev;
}

bool is_leaf(Node *node) { return !(node->left) && !(node->right); }

Node *get_nonnull_child (Node *par) {
    if (par->left) return par->left;
    if (par->right) return par->right;
    return nullptr;
}