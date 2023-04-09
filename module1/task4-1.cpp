/*

Напишите программу, которая использует кучу для слияния K отсортированных массивов суммарной длиной N.
Требования:

    Время работы O(N * logK)
    Куча должна быть реализована в виде шаблонного класса.
    Решение должно поддерживать передачу функции сравнения снаружи.
    Куча должна быть динамической.

Формат ввода:
Сначала вводится количество массивов K.
Затем по очереди размер каждого массива и элементы массива.
Каждый массив упорядочен по возрастанию.

Формат вывода:
Итоговый отсортированный массив.

*/

#include <iostream>


struct Node {
    int value;
    int fromWhichArray;

    Node() = default;
    Node(const int& k, const int& arrayIndex) : value(k), fromWhichArray(arrayIndex) {}
    ~Node() = default;

    bool operator<(const Node&) const;
};


template <typename T>
class IsLessDefault {
    public:
        bool operator()(const T& left, const T& right) { return left < right; }
};

template <typename T, typename IsLess>
class Heap {
    public:
        Heap(const IsLess& less = IsLessDefault<T>())
            : size(0), memBufferSize(0), memBuffer(nullptr), isLess(less){};

        Heap(const int& heapSize, const int& heapBufferSize, const T* heap,
            const IsLess& less = IsLessDefault<T>()) = delete;

        ~Heap() { delete[] memBuffer; }

        void add(const T& element);

        T extract();

    private:
        int getParentIndex(const int& index) const;
        int getLeftChildIndex(const int& index) const;
        int getRightChildIndex(const int& index) const;

        void siftDown(const int& index);
        void siftUp(const int& index);

        int size;
        int memBufferSize;

        T* memBuffer;
        IsLess isLess;
};  


bool Node::operator<(const Node& other) const {
    return value < other.value;
}

template <typename T, typename IsLess>
void Heap<T, IsLess>::add(const T& element) {
    if (memBufferSize == 0) {
        memBufferSize = 1;
        memBuffer = new T[memBufferSize];
    } else if (memBufferSize == size) {
        memBufferSize *= 2;

        T* newArray = new T[memBufferSize];
        std::copy(memBuffer, memBuffer + size, newArray);
        delete[] memBuffer;

        memBuffer = newArray;
    }

    memBuffer[size] = element;
    size++;

    siftUp(size - 1);
}

template <typename T, typename IsLess>
T Heap<T, IsLess>::extract() {
    if (size == 0)
        throw std::invalid_argument("The heap is empty.");

    std::swap(memBuffer[0], memBuffer[size - 1]);
    T result = memBuffer[size - 1];
    size--;

    siftDown(0);

    return result;
}

template <typename T, typename IsLess>
void Heap<T, IsLess>::siftDown(const int& index) {
    int currIndex = index;
    int leftIndex = getLeftChildIndex(currIndex);
    int rightIndex = getRightChildIndex(currIndex);

    /*
        Я пытался вынести условия в булевы переменные, чтобы было по-понятнее и почище,
        но у меня выходила RE, а я недостаточно опытен в С++, чтобы понять почему, не смог разобраться.
    */
    while ((currIndex < size / 2) &&
            (isLess(memBuffer[leftIndex], memBuffer[currIndex]) ||
            ((rightIndex < size) &&
            isLess(memBuffer[rightIndex], memBuffer[currIndex])))) {

        if ((rightIndex >= size) || (isLess(memBuffer[leftIndex], memBuffer[rightIndex]))) {
            std::swap(memBuffer[leftIndex], memBuffer[currIndex]);
            currIndex = leftIndex;
        } else {
            std::swap(memBuffer[rightIndex], memBuffer[currIndex]);
            currIndex = rightIndex;
        }

        leftIndex = getLeftChildIndex(currIndex);
        rightIndex = getRightChildIndex(currIndex);
    }
}

template <typename T, typename IsLess>
void Heap<T, IsLess>::siftUp(const int& index) {
    int currentIndex = index;
    int parentIndex = getParentIndex(currentIndex);

    while ((currentIndex != 0) && isLess(memBuffer[currentIndex], memBuffer[parentIndex])) {
        std::swap(memBuffer[parentIndex], memBuffer[currentIndex]);
        currentIndex = parentIndex;
        parentIndex = getParentIndex(currentIndex);
    }
}

template <typename T, typename IsLess>
int Heap<T, IsLess>::getLeftChildIndex(const int& index) const {
    return 2 * index + 1;
}

template <typename T, typename IsLess>
int Heap<T, IsLess>::getRightChildIndex(const int& index) const {
    return 2 * index + 2;
}

template <typename T, typename IsLess>
int Heap<T, IsLess>::getParentIndex(const int& index) const {
    if (index == 0)
        return 0;
    return (index - 1) / 2;
}

int* merge(const int k, const int heapSize, const int* arraySizes, int** arrays) {
    Heap<Node, IsLessDefault<Node>> heap;
    for (int i = 0; i < k; i++)
        if (arraySizes[i] > 0)
            heap.add(Node(arrays[i][0], i));

    int *arraysElementCounter = new int[k];
    for (int i = 0; i < k; i++)
        arraysElementCounter[i] = 1;

    int* result = new int[heapSize];
    for (int i = 0; i < heapSize; i++) {
        Node minElement = heap.extract();

        int fromWhichArray = minElement.fromWhichArray;
        result[i] = minElement.value;

        if (arraysElementCounter[fromWhichArray] < arraySizes[fromWhichArray]) {
            heap.add(Node(
                     arrays[fromWhichArray][arraysElementCounter[fromWhichArray]],
                     fromWhichArray));

            arraysElementCounter[fromWhichArray]++;
        }
    }

    delete[] arraysElementCounter;

    return result;
}

int getHeapSize(int k, int* arraySizes) {
    int heapSize = 0;
    for (int i = 0; i < k; i++)
        heapSize += arraySizes[i];
    return heapSize;
}

int main() {
    int k;
    std::cin >> k;

    int *arraySizes, **arrays;
    arraySizes = new int[k];
    arrays = new int*[k];

    for (int i = 0; i < k; i++) {
        std::cin >> arraySizes[i];

        arrays[i] = new int[arraySizes[i]];
        for (int j = 0; j < arraySizes[i]; j++)
            std::cin >> arrays[i][j];
    }

    int heapSize = getHeapSize(k, arraySizes);
    int* mergedArray = merge(k, heapSize, arraySizes, arrays);
    
    for (int i = 0; i < heapSize; ++i)
        std::cout << mergedArray[i] << " ";

    for (int i = 0; i < k; ++i)
        delete[] arrays[i];
    delete[] arrays;
    delete[] arraySizes;
    delete[] mergedArray;
    
    return 0;
}