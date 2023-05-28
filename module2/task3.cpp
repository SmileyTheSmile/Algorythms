/*

Постройте B-дерево минимального порядка t и выведите его по слоям.
В качестве ключа используются числа, лежащие в диапазоне [0..232-1]

Требования:
    B-дерево должно быть реализовано в виде шаблонного класса.
    Решение должно поддерживать передачу функции сравнения снаружи.

Формат ввода
    Сначала вводится минимальный порядок дерева t.
    Затем вводятся элементы дерева.

Формат вывода
    Программа должна вывести B-дерево по слоям.
    Каждый слой на новой строке, элементы должны выводится в том порядке, в котором они лежат в узлах. 

*/

#include <iostream>
#include <vector>


template <typename T>
class IsLessDefault {
    public:
        bool operator()(const T& left, const T& right) { return left < right; }
};


template <typename T, typename IsLess>
class BTree {
    public:
        struct Node {
            Node(bool leaf): leaf(leaf) { }
            
            ~Node() {
                for (Node* child: children)
                    delete child;
            }
            
            std::vector<T> keys;
            std::vector<Node*> children;
            
            bool leaf;
        };
        
        BTree(size_t min_degree, const IsLess& less = IsLessDefault<T>()):
        _t(min_degree), _root(nullptr), _isLess(less) { }
        
        ~BTree() { if (_root) delete _root; }
        
        void Insert(const T& key) {
            if (!_root)
                _root = new Node(true);
            
            if (isNodeFull(_root)) {
                Node *newRoot = new Node(false);
                newRoot->children.push_back(_root);
                _root = newRoot;
                splitChild(_root, 0);
            }
            
            insertNonFull(_root, key);
        }
        
        void DebugPrint() {
            debugPrintInternal(_root, 0);
        }
        
    private:
        Node *_root;
        IsLess _isLess;
        size_t _t;

        void debugPrintInternal(Node* node, int indent) {
            if (!_root)
                return;

            std::vector<Node*> nodes;
            nodes.push_back(_root);
            
            while (!nodes.empty()) {
                Node* node = nodes.front();
                nodes.erase(nodes.begin());

                for (size_t i = 0; i < node->keys.size(); i++) {
                    bool isLastKey = (i == node->keys.size() - 1);
                    bool isLastNodeOrSmallerKey = (nodes.empty() || _isLess(nodes.front()->keys[0], node->keys[i]));
                    bool isEndOfLine = (isLastKey && isLastNodeOrSmallerKey);

                    std::cout << node->keys[i] << (isEndOfLine ? "\n": " ");
                }

                for (size_t i = 0; i < node->children.size(); i++)
                    nodes.push_back(node->children[i]);
            }
        }
            
        bool isNodeFull(Node* node) {
            return node->keys.size() == 2 * _t - 1;
        }
        
        void splitChild(Node* parent, size_t index) {
            Node* newNode = new Node(false);
            Node* oldNode = std::move(parent->children[index]);

            newNode->keys.insert(newNode->keys.begin(),
                std::make_move_iterator(oldNode->keys.begin() + _t), 
                std::make_move_iterator(oldNode->keys.end()));
            oldNode->keys.erase(oldNode->keys.begin() + _t - 1, oldNode->keys.end());
            
            newNode->leaf = oldNode->leaf;

            if (!oldNode->leaf) {
                newNode->children.insert(newNode->children.begin(),
                    std::make_move_iterator(oldNode->children.begin() + _t), 
                    std::make_move_iterator(oldNode->children.end()));
                oldNode->children.erase(oldNode->children.begin() + _t, oldNode->children.end());
            }

            parent->children.insert(parent->children.begin() + index + 1, std::move(newNode));
            parent->keys.insert(parent->keys.begin() + index, std::move(oldNode->keys[_t - 1]));
        }
        
        void insertNonFull(Node* node, const T& key) {
            int pos = node->keys.size() - 1;
            
            if (node->leaf) {
                node->keys.resize(node->keys.size() + 1);
                while (pos >= 0 && _isLess(key, node->keys[pos])) {
                    node->keys[pos + 1] = node->keys[pos];
                    pos--;
                }
                node->keys[pos + 1] = key;
            } else {
                while (pos >= 0 && _isLess(key, node->keys[pos]))
                    pos--;
                
                if (isNodeFull(node->children[pos + 1])) {
                    splitChild(node, pos + 1);
                    if (_isLess(node->keys[pos + 1], key))
                        pos++;
                }

                insertNonFull(node->children[pos + 1], key);
            }
        }
};

int main1() {
    size_t t = 2;
    BTree<int, IsLessDefault<int>> tree(t);
    std::vector<int> nums = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    std::cout << "Begin" << std::endl;

    for (size_t i = 0; i < nums.size(); i++) {
        tree.Insert(nums[i]);
    }

    tree.DebugPrint();

	return 0;
}

int main() {
    size_t t = 2;
    std::cin >> t;
    BTree<int, IsLessDefault<int>> tree(t);

    int num = 0;
    while (std::cin >> num) {
        tree.Insert(num);
    }

    tree.DebugPrint();

	return 0;
}