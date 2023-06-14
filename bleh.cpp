
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

void printHuffmanTable(std::map<char, int> table) {
    for (auto const& imap: table)
        std::cerr << "Key: " << imap.first << ", value: " << imap.second << std::endl;
}

void printHuffmanMinHeap(MinHeap heap) {
    while (!heap.empty()) {
        Node* node = heap.top(); heap.pop();
        std::cerr << "Key: " << node->value << ", value: " << node->frequency << std::endl;
    }
}

void traversePostorder(Node* tree) {
    if (tree->left)
        traversePostorder(tree->left);
    std::cout << " " << tree->frequency << " " << std::endl;
    if (tree->right)
        traversePostorder(tree->right);
}
void visualizeBuffer(const std::vector<byte>& buffer) {
    for (auto &b: buffer)
        std::cout << std::bitset<8>(b) << "|";
    std::cout << std::endl;
}

