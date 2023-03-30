#include <functional>
#include <iostream>
#include <cmath>

template <typename T, typename Comparator = std::less<T>>
int MedianOfThreePosition(
    int left,
    int right,
    T* array,
    Comparator compare = Comparator()) {
  int middle = (left + right) / 2;
  if (compare(array[left], array[middle])) {
    if (compare(array[middle], array[right]))
      return middle;
    return compare(array[left], array[right]) ? right : left;
  } else {
    if (compare(array[left], array[right]))
      return left;
    return compare(array[middle], array[right]) ? right : middle;
  }
}

template <typename T, typename Comparator = std::less<T>>
int Partition(
    int left,
    int right,
    T* array,
    Comparator compare = Comparator()) {
  std::swap(
      array[MedianOfThreePosition<T, Comparator>(left, right, array, compare)],
      array[right]);
  
  auto pivot = array[right];
  auto i = right;
  for (auto j = right - 1; j >= left; j--) {
    if (array[j] > pivot)
      std::swap(array[--i], array[j]);
  }
  std::swap(array[i], array[right]);

  return i;
}

template <typename T, typename Comparator = std::less<T>>
int KthStatistics(
    double percentile,
    T* array,
    int n,
    Comparator compare = Comparator()) {
  auto k = int(std::ceil(n * percentile));
  auto left = 0;
  auto right = n - 1;
  while (true) {
    auto pivot_position = Partition<T, Comparator>(left, right, array, compare);
    if (pivot_position == k)
      return array[pivot_position];
    if (pivot_position > k)
      right = pivot_position - 1;
    else
      left = pivot_position + 1;
  }
}

int main() {
  constexpr int kPercentilesSize = 3;
  constexpr double kPercentiles[kPercentilesSize] { 0.1, 0.5, 0.9 };

  int n { 0 };
  std::cin >> n;
  
  int* array = new int[n];
  for (auto i = 0; i < n; i++)
    std::cin >> array[i];

  for (auto i = 0; i < kPercentilesSize; i++)
    std::cout << KthStatistics<int>(kPercentiles[i], array, n) << std::endl;

  delete[] array;
  return 0;
}
