/*

Дано число N и N строк. Каждая строка содержащит команду добавления или удаления натуральных чисел,
а также запрос на получение k-ой порядковой статистики.
Команда добавления числа A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
Запрос на получение k-ой порядковой статистики задается числом k. Требуемая скорость выполнения запроса - O(log n). 

Решение должно поддерживать передачу функции сравнения снаружи.

*/

#include <iostream>

template <typename T>
class IsLessDefault {
    public:
        bool operator()(const T& left, const T& right) { return left < right; }
};

template <typename T, typename IsLess>
class AvlTree {
    public:
        struct Node {
            Node(const T& v): 
                value(v), left(nullptr), right(nullptr),
                height(1), count(1){ }

            T value;
            Node* left;
            Node* right;

            size_t height;
            size_t count;
        };

        AvlTree(const IsLess& less = IsLessDefault<T>()): _root(nullptr), _isLess(less)  { }
        ~AvlTree() { destroyTree(_root); }; 


        void Add(const T& value) {
            _root = addInternal(_root, value);
        }

        void Delete(const T& value) {
            _root = deleteInternal(_root, value);
        }

        bool Has(const T& value) {
            Node *temp = _root;
            while (temp) {
                if (temp->value == value)
                    return true;
                else if (temp->value < value)
                    temp = temp->right;
                else
                    temp = temp->left;
            }

            return false;
        }

        T& GetKthStatistic(const size_t& k) {
            Node* node = _root;
            size_t index = getCount(_root->left);

            while (index != k) {
                if (k > index) {
                    node = node->right;
                    index += 1 + getCount(node->left);
                } else {
                    node = node->left;
                    index -= 1 + getCount(node->right);
                }
            }

            return node->value;
        }
   
    private:
        Node* _root;
        IsLess _isLess;

        Node* addInternal(Node* node, const T& value) {
            if (!node)
                return new Node(value);

            if (_isLess(value, node->value))
                node->left = addInternal(node->left, value);
            else
                node->right = addInternal(node->right, value);
            
            return doBalance(node);
        }

        Node* deleteInternal(Node* node, const T& value) {
            if (!node)
                return nullptr;

            if (_isLess(node->value, value)) {
                node->right = deleteInternal(node->right, value);
            } else if (_isLess(value, node->value)) {
                node->left = deleteInternal(node->left, value);
            } else {
                Node* left = node->left;
                Node* right = node->right;
                
                delete node;
                
                if (!right)
                    return left;
                
                return doBalance(findAndRemoveMin(left, right));
            }

            return doBalance(node);
        }

        Node* findAndRemoveMin(Node* left, Node* right) {
            Node* min = findMin(right);
            min->right = removeMin(right);
            min->left = left;
            return min;
        }

        Node* findMin(Node* node) {
            while (node->left)
                node = node->left;
            return node;
        }

        Node* findMax(Node* node) {
            while (node->right)
                node = node->right;
            return node;
        }

        Node* removeMin(Node* node) {
            if (!node)
                return nullptr;

            if (!node->left)
                return node->right;

            node->left = removeMin(node->left);
            return doBalance(node);
        }


        void destroyTree(Node* node) {
            if (node) {
                destroyTree(node->left);
                destroyTree(node->right);
                delete node;
            }
        }

        void fixHeight(Node* node) {
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        }

        void fixCount(Node* node) {
            node->count = 1 + getCount(node->left) + getCount(node->right);
        }

        Node* rotateLeft(Node* node) {
            Node* temp = node->right;
            node->right = temp->left;
            temp->left = node;

            fixHeight(node);
            fixCount(node);
            
            fixHeight(temp);
            fixCount(temp);

            return temp;
        }

        Node* rotateRight(Node* node) {
            Node* temp = node->left;
            node->left = temp->right;
            temp->right = node;

            fixHeight(node);
            fixCount(node);

            fixHeight(temp);
            fixCount(temp);

            return temp;
        }

        Node* doBalance(Node* node) {
            fixHeight(node);
            fixCount(node);
            
            switch (getBalance(node)) {
                case 2: {
                    if (getBalance(node->right) < 0)
                        node->right = rotateRight(node->right);
                    return rotateLeft(node);
                }
                case -2: {
                    if (getBalance(node->left) > 0)
                        node->left = rotateLeft(node->left);
                    return rotateRight(node);
                }
                default:
                    return node;
            }
        }
        

        int getBalance(const Node* node) {
            return node ? getHeight(node->right) - getHeight(node->left) : 0;
        }

        size_t getCount(const Node* node) {
            return node ? node->count : 0;
        }

        size_t getHeight(const Node* node) {
            return node ? node->height : 0;
        }
};


int main() {
	size_t count;
	std::cin >> count;

	AvlTree<size_t, IsLessDefault<size_t>> tree;

    int num, k;
	for (size_t i = 0; i < count; i++) {
		std::cin >> num >> k;

		if (num < 0)
			tree.Delete(-num);
		else 
			tree.Add(num);

		std::cout << tree.GetKthStatistic(k) << " ";
	}

	return 0;
}