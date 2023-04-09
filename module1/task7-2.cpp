/*

Дан массив неотрицательных целых 64-разрядных чисел.
Количество чисел не больше 1000000.
Отсортировать массив методом MSD по битам (бинарный QuickSort). 

*/


#include <iostream>
#include <assert.h> 
 
 
bool getKthBit(unsigned long long num, int k) {
    return (num >> k) & 1;
}

void findNumsToSwap(int& i, int& j, int currentBit, unsigned long long *nums) {
    while (( getKthBit(nums[i], currentBit) == 0 ) && (i < j))
        i++;

    while (( getKthBit(nums[j], currentBit) == 1 ) && (j > i))
        j--;
}
 
void binaryQuickSort(unsigned long long *nums, int left, int right, int currentBit) {
    if (right <= left || currentBit < 0)
        return;

    int i = left, j = right;
    while (j != i) {
        findNumsToSwap(i, j, currentBit, nums);
        std::swap(nums[i], nums[j]);
    }

    if (getKthBit(nums[right], currentBit) == 0)
        j++;

    binaryQuickSort(nums, left, j - 1, currentBit - 1);
    binaryQuickSort(nums, j, right, currentBit - 1);
}
 
int main() {
    int size = 0;
    std::cin >> size;
    assert(size <= 1000000);
 
    unsigned long long *nums = new unsigned long long [size];
    for (int i = 0; i < size; i++) {
        std::cin >> nums[i];
        assert(0 <= nums[i]);
    }

    int currentBit = 63;
    binaryQuickSort(nums, 0, size - 1, currentBit);

    for (int i = 0; i < size; i++)
        std::cout << nums[i] << " ";
 
    delete[] nums;
}