/*

Если в числе содержится только один бит со значением 1, записать в выходной поток OK.
Иначе записать FAIL.
Необходимо использование битовых операций.
Использование арифметических операций запрещено.

Формат ввода:
Входное число лежит в диапазоне 0..(2^32)-1) и вводится в десятичном виде.

Формат вывода:
Строка OK или FAIL

*/


#include <iostream>
#include <assert.h> 
#include <math.h>


int bitwiseSubtraction(int num1, int num2) {
    while (num2 != 0) {
        int borrow = (~num1) & num2;
        num1 = num1 ^ num2;
        num2 = borrow << 1;
    }

    return num1;
}

std::string checkIfPowerOfTwo(int n) {
    if ((n & bitwiseSubtraction(n, 1)) == !n)
        return "OK";
    return "FAIL";
}

int main() {
    unsigned int n = 0;
    std::cin >> n;

    assert(0 <= n);
    assert(n <= std::pow(2, 32) - 1);

    std::cout << checkIfPowerOfTwo(n) << std::endl;

    return 0;
}