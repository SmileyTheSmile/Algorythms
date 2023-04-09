/*

В супермаркете решили оптимизировать показ рекламы. 
Известно расписание прихода и ухода покупателей (два целых числа).
Каждому покупателю необходимо показать минимум 2 рекламы. 
Рекламу можно транслировать только в целочисленные моменты времени.
Покупатель может видеть рекламу от момента прихода до момента ухода из магазина.
В каждый момент времени может показываться только одна реклама. Считается, что реклама показывается мгновенно. 
Если реклама показывается в момент ухода или прихода, то считается, что посетитель успел её посмотреть.
Требуется определить минимальное число показов рекламы. 

*/


#include <iostream>
#include <cstring>


struct Customer {
    int enterTime;
    int exitTime;
    int adsNum;

    Customer(int in, int out):
            enterTime(in), exitTime(out), adsNum(0) {}
    Customer():enterTime(0), exitTime(0), adsNum(0) {}

    bool operator<(const Customer&) const;
};


class Queue {
    public:
        explicit Queue(int N):
            bufferSize(0), buffer(new Customer[N]) {}
        ~Queue() { delete[] buffer; }

        Queue(const Queue&); // Копированиe
        Queue(Queue&&) noexcept; // Перемещение

        Queue& operator=(const Queue&); // Присваивание копированием
        Queue& operator=(Queue&&) noexcept; // Присваивание перемещением

        int size() const { return bufferSize; }
        int requredAds();

        void add(Customer);
        void mergeSortRecursive(int, int);
        void mergeSortIterative(int);

    private:
        int bufferSize;

        Customer *buffer;

        void merge(int, int, int);
        void addAdsFromTimePeriod(int, int);

};


bool Customer::operator<(const Customer &other) const {
    if (exitTime < other.exitTime)
        return true;

    if (exitTime == other.exitTime)
        return enterTime > other.enterTime;

    return false;
}

Queue& Queue::operator=(const Queue& queueToCopy) {
    Queue temp(queueToCopy);
    *this = std::move(temp);
    return *this;
}

Queue& Queue::operator=(Queue&& queueToMove) noexcept {
    if (this == &queueToMove)
        return *this;

    delete[] buffer;
    buffer = queueToMove.buffer;
    queueToMove.buffer = nullptr;
    bufferSize = queueToMove.bufferSize;

    return *this;
}

Queue::Queue(const Queue& queueToCopy) :
    bufferSize(queueToCopy.bufferSize), buffer(new Customer[bufferSize]) {
    for (int i = 0; i < bufferSize; i++)
        buffer[i] = queueToCopy.buffer[i];
}

Queue::Queue(Queue&& queueToReplace) noexcept:
    bufferSize(queueToReplace.bufferSize), buffer(queueToReplace.buffer) {
    queueToReplace.buffer = nullptr;
}

int Queue::requredAds() {
    int ads = 0;
    
    for (int customerIndex = 0; customerIndex < size(); customerIndex++) {
        if (buffer[customerIndex].adsNum == 0) {
            addAdsFromTimePeriod(customerIndex, buffer[customerIndex].exitTime - 1);
            ads++;
        }

        if (buffer[customerIndex].adsNum == 1) {
            addAdsFromTimePeriod(customerIndex, buffer[customerIndex].exitTime);
            ads++;
        }
    }
    return ads;
}

void Queue::addAdsFromTimePeriod(int customerIndex, int timePeriod) {
    for (
            int i = customerIndex;
            i < size() && buffer[i].enterTime <= timePeriod;
            i++
        )
        buffer[i].adsNum++;
}

void Queue::add(Customer customer) {
    buffer[bufferSize++] = customer;
}

void Queue::mergeSortRecursive(int start, int end) {
    if (start + 1 >= end)
        return;

    int middle = start + (end - start) / 2;

    mergeSortRecursive(start, middle);
    mergeSortRecursive(middle, end);

    merge(start, middle, end);
}

void Queue::mergeSortIterative(int n) {
    for (int i = 1; i < n; i *= 2)
        for (int j = 0; j < n - i; j += 2 * i)
            merge(j, j + i, std::min(j + 2 * i, n));
}

void Queue::merge(int start, int middle, int end) {
    Customer *temp = new Customer[middle - start];
    memcpy(temp, buffer + start, sizeof(Customer) * (unsigned long)(middle - start));

    int it1 = 0, it2 = 0;
    while ((start + it1 < middle) && (middle + it2 < end)) {
        if (temp[it1] < buffer[middle + it2]) {
            buffer[start + it1 + it2] = temp[it1];
            it1++;
        } else {
            buffer[start + it1 + it2] = buffer[middle + it2];
            it2++; 
        }
    }

    while (start + it1 < middle) {
        buffer[start + it1 + it2] = temp[it1];
        it1++;
    }
       
    delete[] temp;
}


int main() {
    int n = 0;
    std::cin >> n;

    Queue queue(n);
    for (int i = 0; i < n; i++)
    {
        int enterTime = 0, exitTime = 0;
        std::cin >> enterTime >> exitTime;

        queue.add(Customer(enterTime, exitTime));
    }
    queue.mergeSortIterative(n);

    std::cout << queue.requredAds();

    return 0;
}