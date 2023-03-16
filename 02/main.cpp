#include <iostream>

/*
Дан отсортированный массив различных целых чисел A[0..n-1] и массив целых чисел
B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс
элемента массива A[k], ближайшего по значению к B[i]. n ≤ 110000, m ≤ 1000.
Время работы поиска для каждого элемента B[i]: O(log(k)).
*/

int ExponentialSearch(int target, int* array, int n) {
  int right = 1;
  while (right < n && target >= array[right])
    right <<= 1;

  return right;
}

int BinarySearch(int left, int right, int target, int* array, int n) {
  // угловой случай
  if (target >= array[n - 1])
    return n - 1;
  
  while (left + 1 < right) {
    int middle = (left + right) / 2;
    if (target > array[middle]) {
      if (middle + 1 < n && target < array[middle + 1]) {
        left = middle;
        right = left + 1;
        break;
      }
      left = middle + 1;
    } else {
      if (middle > 0 && target > array[middle - 1]) {
        left = middle - 1;
        right = left + 1;
        break;
      }
      right = middle;
    }
  }
  
  return (array[right] - target < target - array[left]) ? right : left;
}

int main() {
  int n = 0;
  std::cin >> n;

  int* A = new int[n];
  for (auto i = 0; i < n; i++)
    std::cin >> A[i];

  int m = 0;
  std::cin >> m;

  int* B = new int[m];
  for (auto i = 0; i < m; i++)
    std::cin >> B[i];

  for (auto i = 0; i < m; i++) {
    int right = ExponentialSearch(B[i], A, n);
    int k = BinarySearch(right >> 1, (right < n ? right : n), B[i], A, n);
    std::cout << k << ' ';
  }

  std::cout << std::endl;

  delete[] A;
  delete[] B;
  
  return 0;
}
