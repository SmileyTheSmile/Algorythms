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


struct Customer {
    bool operator<=(const Customer &other) const {
        if (other.exitTime == exitTime)
            return enterTime <= other.enterTime;
        return exitTime <= other.exitTime;
    }

    int enterTime;
    int exitTime;
};


void merge(Customer *array1, Customer* array2, Customer *mergedArray, const int &halfOfTheSize) {
    size_t i = 0, j = 0, k = 0;
    while (i < halfOfTheSize && j < halfOfTheSize) {
        if (array1[i] <= array2[j])
            mergedArray[k] = array1[i++];
        else
            mergedArray[k] = array2[j++];
        k++;
    }
}


void mergeSortRecursive(Customer *customers, const size_t &customersNum) {
    if (customersNum <= 1)
        return;

    int halfOfTheSize = customersNum / 2;

    mergeSortRecursive(customers, halfOfTheSize);
    mergeSortRecursive(customers + halfOfTheSize, halfOfTheSize);

    Customer* temp = new Customer[customersNum];

    merge(customers, customers + halfOfTheSize, temp, halfOfTheSize);

    delete[] temp;
}


int requredAds(Customer *customers, const size_t &customersNum) {
    int sum = 0;
    int t0 = -1, t1 = -1;
    for (size_t i = 0; i < customersNum; i++) {
        Customer temp = customers[i];
        if (temp.enterTime > t1) {
            t0 = temp.exitTime - 1;
            t1 = temp.exitTime;
            sum += 2;
        } else if (temp.enterTime == t1 || (temp.enterTime > t0 && temp.exitTime > t1)) {
            t0 = t1;
            t1 = temp.exitTime;
            sum++;
        } else if (temp.enterTime > t0 && temp.enterTime < t1) {
            t0 = temp.exitTime;
            sum++;
        }
    }

    return sum;
}


int main() {
    size_t n;
    std::cin >> n;

    Customer *customers = new Customer[n];
    for (size_t i = 0; i < n; i++)
        std::cin >> customers[i].enterTime >> customers[i].exitTime;

    mergeSortRecursive(customers, n);

    std::cout << requredAds(customers, n);

    delete[] customers;

    return 0;
}