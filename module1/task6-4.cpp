#include <cmath>
#include <ctime>
#include <iostream>


template <typename T>
bool IsLessDefault(const T &left, const T &right) {
    return left < right;
}

size_t RandomPivot(int *nums, const size_t &left, const size_t &right) {
    srand(time(NULL));
    return rand() % (right - left + 1) + left;
}

size_t Partition(int *nums, const size_t &left, const size_t &right,
                 bool isLess(const int&, const int&) = IsLessDefault<int>) {
    size_t pivotIndex = RandomPivot(nums, left, right);
    int pivot = nums[pivotIndex];
    std::swap(nums[pivotIndex], nums[right]);

    size_t i = left, j = left;
    while (true) {
        while (i < right && (isLess(nums[i], pivot) || nums[i] == pivot))
            i++;
            
        while ((j < right && isLess(pivot, nums[j])) || j < i)
            j++;
            
        if (j < right && isLess(nums[j], nums[i])) {
            std::swap(nums[i], nums[j]);
            i++;
            j++;
        } else {
            std::swap(nums[i], nums[right]);
            return i;
        }
    }
}

int KthStatistic(int* arr, size_t left, size_t right, const size_t& k,
               bool isLess(const int&, const int&) = IsLessDefault<int>) {
    size_t pivotIndex = Partition(arr, left, right, isLess);
    while (pivotIndex != k) {
        if (k > pivotIndex)
            left = pivotIndex + 1;
        else
            right = pivotIndex - 1;
            
        pivotIndex = Partition(arr, left, right, isLess);
    }

    return arr[pivotIndex];
}

int main() {
    size_t n = 0;
    std::cin >> n;

    int *nums = new int[n];
    for (size_t i = 0; i < n; i++)
        std::cin >> nums[i];
    
    for (float percentile : {0.1, 0.5, 0.9})
        std::cout << KthStatistic(nums, 0, n - 1, percentile * n, IsLessDefault) << "\n";
    
    delete[] nums;

    return 0;
}