
Node* huffmanTableToTree(std::map<char, int>& table) {
    Node* tree = nullptr;
    for(auto const& imap: table) {
        std::cout << imap.first;
        tree = add(tree, new Node(imap.first, imap.second));
    }

    return tree;
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
                std::cout << node->frequency << " ";
                temp = node;
                toDo.pop();
            }
        }
    }
}