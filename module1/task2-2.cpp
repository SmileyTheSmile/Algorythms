/*

Дан массив целых чисел А[0..n-1]. Известно, что на интервале [0, m] значения массива строго возрастают, а на интервале [m, n-1] строго убывают.
Найти m за O( log m ).
2 ≤ n ≤ 10000. 

*/


#include <iostream>
#include <assert.h> 


int binarySearch(int* nums, int& start, int& end) {
    int mid = 0;
    
    do {
        mid = (end + start) / 2;
        
        if (nums[mid - 1] < nums[mid]) {
            if (nums[mid] > nums[mid + 1])
                return mid;
                
            start = mid;
        }
        else {
            end = mid;
        }
    } while (end - start > 1);
    
    return mid;
}

int exponentialSearch(int* nums, const int& n) {
    if (nums[0] > nums[1])
        return 0;
    else if (n == 2)
        return 1;
    
    const int arrayBorder = n - 1;
    if (nums[arrayBorder] > nums[arrayBorder - 1])
        return arrayBorder;
    
    int left = 1, right = 2;
    while (right != arrayBorder) {
        if (nums[right] < nums[right + 1]) { // Nums after right increase
            left = right;
            right = ((2 * left) < arrayBorder) ? 2 * left : arrayBorder;
            continue;   
        }

        if (nums[right - 1] < nums[right]) // Nums before right increase (Right is peak)
            return right;

        if ((nums[left - 1] < nums[left]) && (nums[left] > nums[left + 1])) // Left is peak
            return left;
            
        return binarySearch(nums, left, right);
    }
    
    return binarySearch(nums, left, right);
}

int main() {
    int n = 0;
    std::cin >> n;

    assert(2 <= n);
    assert(n <= 10000);

    int* nums = new int[n];
    for (int i = 0; i < n; i++) 
        std::cin >> nums[i];
    
    std::cout << exponentialSearch(nums, n);

    delete [] nums;

    return 0;
}