#include <iostream>
#include <stack>


struct Node {
    Node* left;
    Node* right;
    
    int value;

    Node(int v): value(v), left(nullptr), right(nullptr) {}

    ~Node() {
        std::stack<Node*> toDelete;
        
        toDelete.push(left);
        toDelete.push(right);

        Node* temp = nullptr;
        while (!toDelete.empty()) {
            temp = toDelete.top();
            toDelete.pop();
            if (temp) {
                if (temp->left) {
                    toDelete.push(temp->left);
                    temp->left = nullptr;
                }
                if (temp->right) {
                    toDelete.push(temp->right);
                    temp->right = nullptr;
                }
                
                delete temp;
            }
        }
    }
};


Node* add(Node* head, int value) {
    if (!head)
        return new Node(value);
    
    Node* temp = head;
    while (temp) {
        if (value > temp->value) {
            if (temp->right) {
                temp = temp->right;
            } else {
                temp->right = new Node(value);
                break;
            }
        } else if (value < temp->value) {
            if (temp->left) {
                temp = temp->left;
            } else {
                temp->left = new Node(value);
                break;
            }
        } else {
            break;
        }
    }

    return head;
}


void traversePostorder(Node* tree) {
    std::stack<Node*> toDo;

    Node* temp = nullptr;
    while (tree || !toDo.empty()) {
        if (tree) {
            toDo.push(tree);
            tree = tree->left;
        } else {
            Node* node = toDo.top();

            if (node->right && temp != node->right) {
                tree = node->right;
            } else {
                std::cout << node->value << " ";
                temp = node;
                toDo.pop();
            }
        }
    }
}


int main() {
    Node* tree = nullptr;

    size_t n = 0;
    std::cin >> n;

    size_t num = 0;
    for (int i = 0; i < n; i++) {
        std::cin >> num;
        tree = add(tree, num);
    }
    
    traversePostorder(tree);

    delete tree;
}