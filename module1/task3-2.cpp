/*

Реализовать дек с динамическим зацикленным буфером.
Обрабатывать команды push * и pop *.

Формат ввода:
В первой строке количество команд n. n ≤ 1000000.

Каждая команда задаётся как 2 целых числа: a b.

    a = 1 - push front
    a = 2 - pop front
    a = 3 - push back
    a = 4 - pop back

Если дана команда pop *, то число b - ожидаемое значение. Если команда pop * вызвана для пустой структуры данных, то ожидается “-1”.

*/

#include <iostream>
#include <assert.h> 


class Deque {
    public:
        explicit Deque(int size, int rate);
        ~Deque() { delete[] memBuffer; }

        void increaseSize();

        bool empty() { return !size; };
        bool pushFront(int x);
        bool pushBack(int x);

        int popFront();
        int popBack();

        int size;

    private:
        int head;
        int tail;
        int memBufferSize;
        int memIncreaseRate;
        int* memBuffer;
};

Deque::Deque(int buffSize, int rate):
head(0), tail(0), memBufferSize(buffSize), memIncreaseRate(rate) {
    memBuffer = new int [memBufferSize];
    size = 0;
}

void Deque::increaseSize() {
    int newBufferSize = memBufferSize * memIncreaseRate;
    int *temp = new int[memBufferSize * memIncreaseRate];

    if (head < tail) {
        std::copy(memBuffer + head, memBuffer + tail, temp + head);
    } else {
        std::copy(memBuffer + head, memBuffer + memBufferSize, temp + head);
        std::copy(memBuffer, memBuffer + tail, temp + memBufferSize);
        tail = memBufferSize + tail;
    }

    memBufferSize = newBufferSize;
    delete [] memBuffer;
    memBuffer = temp;

    size++;
}

bool Deque::pushFront(int x) {
    if ((size + 1) > memBufferSize)
        increaseSize();
        
    head = (head - 1 + memBufferSize) % memBufferSize;
    memBuffer[head] = x;

    return true;
}

bool Deque::pushBack(int x) {
    if ((size + 1) > memBufferSize)
        increaseSize();
    
    memBuffer[tail] = x;
    tail = (tail + 1) % memBufferSize;
    
    return true;
}

int Deque::popFront() {
    if (empty()) 
        return -1;
        
    int result = memBuffer[head];
    head = (head + 1) % memBufferSize;
    size--;
    
    return result;
}

int Deque::popBack() {
    if (empty())
        return -1;
    
    tail = (tail - 1 + memBufferSize) % memBufferSize;
    size--;
    
    return memBuffer[tail];
}
    

bool checkCommand(Deque& deque, const int command, const int argument) {
    switch (command) {
        case 1: {
            deque.pushFront(argument);
            return true;
        }
        case 2: {
            int result = deque.popFront();
            if (result != argument)
                return false;
            return true;
        }
        case 3: {
            deque.pushBack(argument);
            return true;
        }
        case 4: {
            int result = deque.popBack();
            if (result != argument)
                return false;
            return true;
        }
        default:
            return false;
    }
}

std::string checkCommands(const int n, const int* commands, const int* arguments) {
    Deque exampleDeque(n, 2);
    for (int i = 0; i < n; i++)
        if (!checkCommand(exampleDeque, commands[i], arguments[i]))
            return "NO";

    return "YES";
}

int main() {
    int n;
    std::cin >> n;

    assert(n <= 1000000);

    int *commands, *arguments;
    commands = new int[n];
    arguments = new int[n];

    for (int i = 0; i < n; i++)
        std::cin >> commands[i] >> arguments[i];

    std::cout << checkCommands(n, commands, arguments);

    delete [] commands;
    delete [] arguments;
    
    return 0;
}