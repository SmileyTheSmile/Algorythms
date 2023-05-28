/*

Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми.
Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество,
удаления строки из множества и проверки принадлежности данной строки множеству.

Вариант 1.
    Для разрешения коллизий используйте квадратичное пробирование.
    i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.

Формат ввода:
    Каждая строка входных данных задает одну операцию над множеством. Запись операции состоит из типа операции и следующей за ним через пробел строки, над которой проводится операция. Тип операции – один из трех символов: + означает добавление данной строки в множество; - означает удаление строки из множества; ? означает проверку принадлежности данной строки множеству. При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве. При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.

Формат вывода:
    Программа должна вывести для каждой операции одну из двух строк OK или FAIL.

    Для операции '?': OK, если элемент присутствует во множестве. FAIL иначе.
    Для операции '+': FAIL, если добавляемый элемент уже присутствует во множестве и потому не может быть добавлен. OK иначе.
    Для операции '-': OK, если элемент присутствовал во множестве и успешно удален. FAIL иначе.

*/

#include <iostream>


class Hashtable {
    public:
        Hashtable(
            size_t capacity = 8,
            size_t prime = 71,
            float threshold = 0.75,
            std::string nil = "NILL",
            std::string del = "DELETED"
        ):
        _elemsInTable(0), _seed(prime), _nil(nil), _del(del),
        _sizeIncreaseThreshold(threshold), _capacity(capacity) {
            _table = new std::string[capacity];
            for (size_t i = 0; i < capacity; i++)
                _table[i] = nil;
        }
        
        bool Add(const std::string &key);
        bool Has(const std::string &key);
        bool Delete(const std::string &key);
        
    private:
        void increaseSize();

        bool hasHash(const size_t& hash, const std::string& key);

        size_t initHash(const std::string& key);
        size_t ithProbe(const size_t&hash, const size_t& i);

        const std::string _nil;
        const std::string _del;

        const size_t _seed;

        const float _sizeIncreaseThreshold;
        
        std::string* _table;

        size_t _capacity;
        size_t _elemsInTable;
};


bool Hashtable::Has(const std::string& key) {
    if (key.empty() || key == _nil || key == _del)
        return false;

    size_t hash = initHash(key);
    for (size_t i = 0; i < _capacity; i++) {
        hash = ithProbe(hash, i);

        if (_table[hash] == _nil)
            return false;

        if (_table[hash] == key)
            return true;
    }

    return false;
}

bool Hashtable::Add(const std::string& key) {
    if (key.empty() || key == _nil || key == _del)
        return false;

    if (float(_elemsInTable) / float(_capacity) >= _sizeIncreaseThreshold)
        increaseSize();
    
    size_t addPos = -1;
    size_t hash = initHash(key);
    for (size_t i = 0; i < _capacity; i++) {
        hash = ithProbe(hash, i);

        if (_table[hash] == key)
            return false;
        if (_table[hash] == _del && addPos)
            addPos = hash;
        if (_table[hash] == _nil) {
            if (addPos == -1)
                addPos = hash;
            break;
        }
    }

    if (addPos != -1) {
        _table[addPos] = key;
        _elemsInTable++;
        return true;
    }

    return false;
}
       
bool Hashtable::Delete(const std::string& key) {
    if (key.empty() || key == _nil || key == _del)
        return false;

    size_t hash = initHash(key);
    for (size_t i = 0; i < _capacity; i++) {
        hash = ithProbe(hash, i);

        if (_table[hash] == _nil)
            return false;

        if (_table[hash] == key) {
            _table[hash] = _del;
            _elemsInTable--;
            return true;
        }
    }

    return false;
}
 

size_t Hashtable::initHash(const std::string &key) { 
    size_t hash = 0;
    for (const char& i : key)
        hash = (hash * _seed + i) % _capacity;
    return hash;
}

size_t Hashtable::ithProbe(const size_t& hash, const size_t& i) { 
    return (hash + i) % _capacity;
}

void Hashtable::increaseSize() {
    std::string *oldTable = _table;
    size_t oldCapacity = _capacity;

    _elemsInTable = 0;
    _capacity *= 2;
    _table = new std::string[_capacity];

    for (size_t i = 0; i < _capacity; i++)
        _table[i] = _nil;

    for (size_t i = 0; i < oldCapacity; i++)
        if (oldTable[i] != _del && oldTable[i] != _nil)
            Add(oldTable[i]);

    delete[] oldTable;
}


int main() {
    Hashtable table;
    
    char op;
    std::string key;

    while (std::cin >> op >> key) {
        switch (op) {
            case '?': {
                std::cout << (table.Has(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '+': {
                std::cout << (table.Add(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '-': {
                std::cout << (table.Delete(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
        }
    }
    
    return 0;
}