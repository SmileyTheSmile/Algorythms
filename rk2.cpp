#include <iostream>

struct Node {
    Node* left;
    Node* right;
    
    int value;

    Node(int v): value(v) {
        left = nullptr;
        right = nullptr;
    }
};

Node* insert(Node* node, int value) {
    if (!node)
        return new Node(value);

    if (value < node->value)
        node->left = insert(node->left, value);
    else if (value > node->value)
        node->right = insert(node->right, value);

    return node;
}

bool checkNode(Node* node) {
    if (!node)
        return false;
        
    if (!checkNode(node->left))
        return false;
    
    if (((node->left) && (node->left->value != node->value)) ||
        ((node->right) && (node->right->value != node->value)) ||
        ((node->right && node->left) && (node->right->value != node->left->value)))
        return false;
        
    if (!checkNode(node->right))
        return false;
        
    return true;
}


int main()
{
    Node* tree = nullptr;
    
    int num = 0;
    while (std::cin >> num) {
        insert(tree, num);
    }
    
    std::cout << checkNode(tree);
}